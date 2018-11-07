//
// Created by Nolan Woods on 11/6/2018.
//

#include <thread>
#include <ctime>

#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/basic.h>

#include "BBHashKmerContainer.h"
#include "cmdline.h"

#include "thread_util.h"
#include "FastaRecord.h"
#include "SharedQueue.h"

using Record = FastaRecord;
using Queue = SharedQueue<Record>;
using KmerContainer = BBHashKmerContainer<KmerIterator<seqan::Dna5>, seqan::Dna5>;

const double update_interval = 1; //Seconds

void print_filter_status(unsigned long pending_records, unsigned long output_records, unsigned long total_records,
                         unsigned long total_output) {
    if (total_records)
        //Delete previous output
        std::cerr << "\033[F\033[K";
    std::cerr << "Pending filter: " << std::setw(10) << pending_records << " Pending output: " << std::setw(10) << output_records
            << " Total processed: " << std::setw(10) << total_records
            << " Total output: " << std::setw(10) << total_output << std::endl;
}

//Thread functions
void filter_thread(Queue &pending_records, Queue &output_records, KmerContainer &table, const ParametersFilter &params) {
    try {
        do {
            auto item = pending_records.pop();
            seqan::Dna5 *seq_c = toCString(item.seq);
            for (unsigned i = 0; i < length(item.seq) - params.kmer_length; i++) {
                if (table.contains(seq_c + i)) goto SKIP_OUTPUT;
            }
            output_records.push(std::move(item));
            SKIP_OUTPUT:
            continue;
        } while (true);
    } catch (Stop &e) {

    }
}

void output_thread(Queue &queue, const ParametersFilter &params, unsigned long &total_output) {

    seqan::SeqFileOut seqFileOut;
    if (params.output_filename == "-")
        seqan::open(seqFileOut, std::cout);
    else
        seqan::open(seqFileOut, params.output_filename.c_str());

    try {
        do {
            auto item = queue.pop();
            seqan::writeRecord(seqFileOut, item.id, item.seq); //TODO: add call for fastq sequences
            ++total_output;
        } while (true);
    } catch (Stop &e) {

    }
}

int filter(ParametersFilter &params) {
    //Parse and validate parameters
    std::cerr << "Filtering sequence......" << std::endl;

    //TODO replace with input parameters
    unsigned int queue_limit = 10; //Default soft limit for queue before thread pool increase, TODO replace with memory limit
    unsigned long total_records = 0, total_output = 0;

    KmerContainer table(params.threads, 2, 100, params.kmer_length); //TODO: properly instantiate and load from table file

    //Init thread pool
    Queue pending_records, output_records;
    std::vector<std::thread> thread_pool;
    auto t = thread_pool.emplace(thread_pool.end(), filter_thread, std::ref(pending_records), std::ref(output_records), std::ref(table), std::ref(params));
    increment_priority(*t, -1); //Lower priority of filter workers so not to interfere with IO
    std::thread output_thread_instance(output_thread, std::ref(output_records), std::ref(params), std::ref(total_output));

    //Read in records to queue
    seqan::CharString id;
    seqan::Dna5QString seq;
    seqan::CharString qual;

    //Call sequence stream function of seqan to read from the file
    seqan::SeqFileIn seqFileIn;
    if (params.seq_filename == "-") {
        seqan::open(seqFileIn, std::cin);
    } else {
        seqan::open(seqFileIn, params.seq_filename.c_str());
    }

    print_filter_status(pending_records.size(false), output_records.size(false), total_records, total_output);

    std::time_t last_time, now;
    //Push record into queue
    while (!atEnd(seqFileIn)) { // TODO: readRecord(id, seq, qual, seqStream) for fastq files
        try {
            // if(norc in args) TODO
            reverseComplement(seq);
            readRecord(id, seq, qual, seqFileIn);
            // else
            // readRecord(id, seq, qual, seqFileIn);
        } catch (std::exception const &e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
            return 1;
        }

        //construct a fasta/fastq object
        FastaRecord fa(id, seq);

        //push to the pending queue
        pending_records.push(std::move(fa));

        //Check queue size and increase thread pool to desaturate
        if (pending_records.size() > queue_limit) {
            if (thread_pool.size() < params.threads)
                //Increase thread pool by 1
                t = thread_pool.emplace(thread_pool.end(), filter_thread, std::ref(pending_records),
                                        std::ref(output_records), std::ref(table), std::ref(params));
            increment_priority(*t, -1); //Lower priority of filter workers so not to interfere with IO

            //Wait for pending records to desaturate (Non-blocking size check)
            while (pending_records.size(false) > queue_limit);
        }
        now = std::time(nullptr);
        if (std::difftime(last_time, now) > update_interval) {
            last_time = now;
            print_filter_status(pending_records.size(false), output_records.size(false), total_records, total_output);
        }

        ++total_records;
    }

    //Join thread pool
    //Signal threads to exit
    pending_records.signal_done();
    for (auto &thread : thread_pool) thread.join();
    output_records.signal_done();
    output_thread_instance.join();

    //Output final statistics
    print_filter_status(pending_records.size(false), output_records.size(false), total_records, total_output);

    return 0;
}

/*
int reuse_filter_singlecore(ParametersFilter &params){
    //Parse and validate parameters
    std::cerr << "Filtering sequence......"<< std::endl;
    std::cerr << "Single Core Run" << std::endl;

    std::cerr <<"paired? " << (params.seq_mate_filename.length()?"true":"false")<< std::endl;
    //TODO replace with input parameters



    BBHashKmerContainer<KmerIterator<seqan::Dna5>,seqan::Dna5> table(1,2,100,21); //TODO: properly instantiate and load from table file
    table.load(params.index_filename.c_str());

    //Read in records to queue
    seqan::CharString id;
    seqan::Dna5String seq;
    seqan::CharString qual;

    //Call sequence stream function of seqan to read from the file
    seqan::SeqFileIn seqFileIn;
    if (params.seq_filename == "-")
        seqan::open(seqFileIn, std::cin);
    else
        seqan::open(seqFileIn, params.seq_filename.c_str());

    seqan::SeqFileOut seqFileOut;
    if (params.output_filename == "-"){
        seqan::open(seqFileOut, std::cout);
    }
    else{
        seqan::open(seqFileOut, params.output_filename.c_str());
    }

    //Push record into queue
    while (!atEnd(seqFileIn)) { // TODO: readRecord(id, seq, qual, seqStream) for fastq files
        try {
            // if(norc in args) TODO

            readRecord(id, seq, qual, seqFileIn);
            // else
            // readRecord(id, seq, qual, seqFileIn);
        } catch (std::exception const & e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
            return 1;
        }
        bool flag = false;
        seqan::Dna5 *seq_c = toCString(seq);
        for(unsigned i = 0; i < length(seq)-params.kmer_length;i++){
            if(table.contains(seq_c+i)){
                flag = true;
                break;
            }

        }
        if(!flag){
            writeRecord(seqFileOut,id,seq,qual);
        }
    }



    return 0;
}
*/
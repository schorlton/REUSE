#include <iostream>
#include <vector>
#include <thread>
#include <cstring>

#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/basic.h>

#include <fstream>

#include "BBHashKmerContainer.h"
#include "cmdline.h"
#include "thread_util.h"
#include "fastaRecord.h"
#include "SharedQueue.h"


using namespace seqan;

using Record = FastaRecord;
using Queue = SharedQueue<Record>;
using KmerContainer = BBHashKmerContainer<KMerIterator<Dna5>,Dna5>;

void print_filter_status(unsigned long pending_records, unsigned long output_records, unsigned long total_records,
                         unsigned long total_output) {
    if (total_records)
        //Delete previous output
        std::cerr << "\033[F\033[K";
    std::cerr << "Pending filter: " << std::setw(10) << pending_records << " Pending output: " << std::setw(10) << output_records << " Total processed: " << std::setw(10) << total_records << " Total output: " << std::setw(10) << total_output << std::endl;
}

void print_build_status(unsigned long total_sequences, unsigned long total_kmers) {
    if (total_sequences)
        //Delete previous output
        std::cerr << "\033[F\033[K";
    std::cerr << " Total processed: " << std::setw(10) << total_sequences << " Total kmers: " << std::setw(10) << total_kmers << std::endl;
}

//Thread interface declaration
void filter(Queue& pending_records, Queue& output_records, KmerContainer& table, const ParametersFilter &params) {
    try {
        do {
            auto item = pending_records.pop();
            Dna5 *seq_c = toCString(item.seq);
            for(unsigned i = 0; i < length(item.seq)-params.kmer_length;i++) {
                if(table.contains(seq_c+i)) goto SKIP_OUTPUT;
            }
            output_records.push(std::move(item));
            SKIP_OUTPUT: continue;
        } while (true);
    } catch (Stop& e) {

    }
}

/*Program to output the */
void output(Queue &queue, const ParametersFilter &params, unsigned long& total_output){

	seqan::SeqFileOut seqFileOut;
	if (params.is_stdout)
	    seqan::open(seqFileOut, std::cout);
	else
	    seqan::open(seqFileOut, params.output_filename);

	try {
        do {
            auto item = queue.pop();
            seqan::writeRecord(seqFileOut, item.id, item.seq); //TODO: add call for fastq sequences
            ++total_output;
        } while (true);
    } catch (Stop& e) {

	}
}


int filter_test(int refTableLength){

    CharString seqFileName = "data/reads.fa";

    SeqFileIn seqfile;

    if( !open(seqfile, toCString(seqFileName))){
        std::cerr << "ERROR\n" << std::endl;
        return -1;

    }

    StringSet<CharString> ids;
    StringSet<Dna5String> seqs;

    try{
        readRecords(ids,seqs,seqfile);
    }catch(Exception const & e){
        std::cerr << "ERROR" << e.what() << std::endl;
        return 1;
    }

    KmerContainer table(1,2,refTableLength,21);
    try {
        std::ifstream inputFile("test_filter_table", std::ios::binary);
        while (inputFile.read((char *)&table,sizeof(table)));
        // read table data & store
        inputFile.close();
    } catch (Exception const & e){
        std::cerr << "FILE ERROR" << e.what() << std::endl;
    }

    try {
        std::ofstream outputFile("test_filter_output.fa");
        for(unsigned i = 0; i < length(ids) ; ++i) {
            // std::cout << table.contains(toCString(seqs[i]));
            if (!table.contains(toCString(seqs[i]))) {
                outputFile << toCString(seqs[i]) << " " << seqs[i] << std::endl;
                // writeRecord(outputFile, seqs[i])
                // outputFile.write(toCString(seqs[i]));
            }
        }
        outputFile.close();
    } catch (Exception const & e) {
        std::cerr << "FILE ERROR" << e.what() << std::endl;
    }

    return 0;


}

int reuse_build(int argc, char **argv){

    std::cerr << "Building reference......"<< std::endl;
    ParametersBuild params;
    if( 0!= parse_command_line_build( argc, argv, params)){
        return -1;
    }
    std::cerr << "number of thread "<<params.threads<< std::endl;
//    std::cout <<"input " << params.seq_filename<< std::endl;

    SeqFileIn seqfile;

    if( !open(seqfile, params.seq_filename)){
        std::cerr << "ERROR\n" << std::endl;
        return -1;

    }

    StringSet<CharString> ids;
    StringSet<Dna5String> seqs;

    try{
        readRecords(ids,seqs,seqfile);
    }catch(Exception const & e){
        std::cerr << "ERROR" << e.what() << std::endl;
        return 1;
    }

    int refTableLength = length(seqs[0])/2-21;
    KmerContainer table(1,2,refTableLength,21); //TODO replace hardcoded params

    for(unsigned i = 0; i < length(ids) ; ++i){

        KMerIterator<Dna5> _begin = get_begin(toCString(seqs[i]),21);
        KMerIterator<Dna5> _end = get_end(toCString(seqs[i]),length(seqs[i]),21);

        std::cerr << "Length : " << length(seqs[i]) << std::endl;
        table.addRange(_begin,_end);
    }

    try {
        std::ofstream outputFile("data/test_filter_table", std::ios::binary);
        outputFile.write((char *)&table,sizeof(table));
        outputFile.close();
    } catch (Exception const & e){
        std::cerr << "FILE ERROR" << e.what() << std::endl;
    }
    table.save(params.output_filename);
 

    return 0;
    // build hash table; binary encoding
}
int reuse_filter_singlecore(ParametersFilter &params){
    //Parse and validate parameters
    std::cerr << "Filtering sequence......"<< std::endl;
    std::cerr << "Single Core Run" << std::endl;

    std::cerr <<"paired? " << (params.paired?"true":"false")<< std::endl;
    //TODO replace with input parameters



    BBHashKmerContainer<KMerIterator<Dna5>,Dna5> table(1,2,100,21); //TODO: properly instantiate and load from table file
    table.load(params.index_filename);

    //Read in records to queue
    seqan::CharString id;
    seqan::Dna5String seq;
    seqan::CharString qual;

    //Call sequence stream function of seqan to read from the file
    seqan::SeqFileIn seqFileIn;
    if (params.is_stdin)
        seqan::open(seqFileIn, std::cin);
    else
        seqan::open(seqFileIn, params.seq_filename_1);

    seqan::SeqFileOut seqFileOut;
    if (params.is_stdout){
        seqan::open(seqFileOut, std::cout);
    }
    else{
        seqan::open(seqFileOut, params.output_filename);
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
        Dna5 *seq_c = toCString(seq);
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

int reuse_filter(int argc, char **argv){
    //Parse and validate parameters
    std::cerr << "Filtering sequence......"<< std::endl;
    ParametersFilter params;
    parse_command_line_filter( argc, argv, params);
    if(params.threads == 1){
        reuse_filter_singlecore(params);
        return 0;
    }
    std::cerr << "number of thread "<<params.threads<< std::endl;
    std::cerr <<"input " << params.seq_filename_1<< std::endl;
    std::cerr <<"paired? " << params.paired<< std::endl;
    //TODO replace with input parameters
    unsigned int queue_limit = 10; //Default soft limit for queue before thread pool increase, TODO replace with memory limit
    unsigned long total_records = 0, total_output = 0;

    KmerContainer table(1,2,100,21); //TODO: properly instantiate and load from table file

    //Init thread pool
    Queue pending_records, output_records;
    std::vector<std::thread> thread_pool;
    auto t = thread_pool.emplace(thread_pool.end(), filter, std::ref(pending_records), std::ref(output_records), std::ref(table), std::ref(params));
    increment_priority(*t, -1); //Lower priority of filter workers so not to interfere with IO
    std::thread output_thread(output, std::ref(output_records), std::ref(params), std::ref(total_output));

    //Read in records to queue
    seqan::CharString id;
    seqan::Dna5QString seq;
    seqan::CharString qual;

    //Call sequence stream function of seqan to read from the file
    seqan::SeqFileIn seqFileIn;
    if (params.is_stdin) {
        seqan::open(seqFileIn, std::cin);
    }
    else {
        seqan::open(seqFileIn, params.seq_filename_1);
    }

    print_filter_status(pending_records.size(false), output_records.size(false), total_records, total_output);

    //Push record into queue
    while (!atEnd(seqFileIn)) { // TODO: readRecord(id, seq, qual, seqStream) for fastq files
        try {
            // if(norc in args) TODO
            reverseComplement(seq);
            readRecord(id, seq, qual, seqFileIn);
            // else
            // readRecord(id, seq, qual, seqFileIn);
        } catch (std::exception const & e) {
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
                t = thread_pool.emplace(thread_pool.end(), filter, std::ref(pending_records), std::ref(output_records), std::ref(table), std::ref(params));
            increment_priority(*t, -1); //Lower priority of filter workers so not to interfere with IO

            //Wait for pending records to desaturate (Non-blocking size check)
            while (pending_records.size(false) > queue_limit);
        }
        if (total_records % 100 == 0)
            print_filter_status(pending_records.size(false), output_records.size(false), total_records, total_output);

        ++total_records;
    }

    //Join thread pool
    //Signal threads to exit
    pending_records.signal_done();
    for (auto& thread : thread_pool) thread.join();
    output_records.signal_done();
    output_thread.join();

    //Output final statistics
    print_filter_status(pending_records.size(false), output_records.size(false), total_records, total_output);

    return 0;
}

int main( int argc, char **argv) {
    //Parse and validate parameters

    if(argc == 1){
        std::cerr << "Please have \"build\" or \"filter\" as argument" << std::endl;
        print_help();
        reuse_exit(ExitSignal::IllegalArgumentError);
    }
    if(strcmp(argv[1], "build")==0) {
        reuse_build(argc-1, argv+1);
    }else if(strcmp(argv[1], "filter")==0){
        reuse_filter(argc-1, argv+1);
    }else{
        print_help();
        reuse_exit(ExitSignal::IllegalArgumentError);
    }
    return 0;
}

#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>
#include <seqan/basic.h>
#include <fastaRecord.h>
#include <fastqRecord.h>

#include "cmdline.h"
#include "SharedQueue.h"
#include "thread_util.h"

using Record = FastaRecord;
using Queue = SharedQueue<Record>;

//Thread interface declaration
void filter(bool&, Queue&, Queue&);
void output(bool&, Queue&);

int reuse_build(int argc, char **argv){


}

int reuse_filter(int argc, char **argv){
    //TODO replace with input parameters
    unsigned int max_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1; //If return 0, set to 1
    unsigned int queue_limit = 10; //Default soft limit for queue before thread pool increase

    //Parse and validate parameters


    //Init thread pool
    Queue pending_records, output_records;
    bool done = false;
    std::vector<std::thread> thread_pool;
    auto t = thread_pool.emplace(thread_pool.end(), filter, std::ref(done), std::ref(pending_records), std::ref(output_records));
    increment_priority(*t, -1); //Lower priority of filter workers so not to interfere with IO
    thread_pool.emplace(thread_pool.end(), output, std::ref(done), std::ref(output_records));

	//Read in records to queue
	seqan::CharString id;
	seqan::Dna5QString seq;
	seqan::CharString qual;

	//Call sequence stream function of seqan to read from the file
	seqan::CharString seqFileName = "data/chrY.fa"; //TODO: replace with param
	seqan::SeqFileIn seqFileIn(toCString(seqFileName));

	//Push record into queue
	while (!atEnd(seqFileIn)) { // TODO: readRecord(id, seq, qual, seqStream) for fastq files
		try {
			readRecord(id, seq, seqFileIn);
		} catch (std::exception const & e) {
			std::cout << "ERROR: " << e.what() << std::endl;
			return 1;
		}

		//construct a fasta/fastq object
		FastaRecord fa = FastaRecord(id, seq);

		//push to the pending queue
		pending_records.push(fa);

        //Check queue size and increase thread pool to desaturate
        if (pending_records.size() > queue_limit) {
            if (thread_pool.size() < max_threads)
                //Increase thread pool by 1
                t = thread_pool.emplace(thread_pool.end(), filter, std::ref(done), std::ref(pending_records), std::ref(output_records));
            increment_priority(*t, -1); //Lower priority of filter workers so not to interfere with IO

            //Wait for pending records to desaturate (Non-blocking size check)
            while (pending_records.size(false) > queue_limit);
        }
    }

    //Join thread pool
    done = true; //Signal threads to exit
    for (auto& thread : thread_pool) thread.join();

    //Output statistics

    return 0;
}

int main( int argc, char **argv) {
    std::cerr << "Hello, World!" << std::endl;
    //Parse and validate parameters

    if(argc == 1){

    }
    if(strcmp(argv[1], "build")==0) {
        reuse_build(argc-1, argv+1);
    }else if(strcmp(argv[1], "filter")==0){
        reuse_filter(argc-1, argv+1);
    }else{
//Print help for reuse funcs
    }
}

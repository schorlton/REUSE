#include <iostream>
#include <cstring>

#include "cmdline.h"
#include "BBHashKmerContainer.h"

#include <seqan/seq_io.h>


using namespace seqan;



int reuse_build(int argc, char **argv){

    std::cout << "Building reference......"<< std::endl;
    ParametersBuild p_build;
    parse_command_line_build( argc, argv, p_build);
    std::cout << "number of thread "<<p_build.threads<< std::endl;
    std::cout <<"input " << p_build.seq_filename<< std::endl;

    CharString seqFileName = "data/chrT.fa";

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

    BBHashKmerContainer<KMerIterator<Dna5>,Dna5String> table(1,1,150);

    for(unsigned i = 0; i < length(ids) ; ++i){

        KMerIterator<Dna5> _begin = get_begin(toCString(seqs[i]),21);
        KMerIterator<Dna5> _end = get_end(toCString(seqs[i]),length(seqs[i]),21);
        table.addRange(_begin,_end);
    }

    // build hash table; binary encoding
}

int reuse_filter(int argc, char **argv){
    std::cout << "Filtering sequence......"<< std::endl;
    ParametersFilter p_filter;
    parse_command_line_filter( argc, argv, p_filter);
    std::cout << "number of thread "<<p_filter.threads<< std::endl;
    std::cout <<"input " << p_filter.seq_filename_1<< std::endl;
    std::cout <<"paired? " << p_filter.paired<< std::endl;

    //Parse and validate parameters

    //Init thread pool


    //Read in records to queue


    //Join thread pool
    

    //Output statistics

    return 0;
}

int main( int argc, char **argv) {
    std::cerr << "Hello, World!" << std::endl;
    //Parse and validate parameters

    if(argc == 1){

    }
    if (argv[1]==NULL){
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

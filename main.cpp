#include <iostream>
#include <cstring>

#include "cmdline.h"
#include "BBHashKmerContainer.h"

#include <seqan/seq_io.h>


using namespace seqan;


int filter_test(BBHashKmerContainer<KMerIterator<Dna5>,Dna5> &table){
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

    for(unsigned i = 0; i < length(ids) ; ++i){
        std::cout << table.contains(toCString(seqs[i]));

    }
    return 0;

}

int reuse_build(int argc, char **argv){

    std::cout << "Building reference......"<< std::endl;
    ParametersBuild p_build;
    if( 0!= parse_command_line_build( argc, argv, p_build)){
        return -1;
    }
    std::cout << "number of thread "<<p_build.threads<< std::endl;
//    std::cout <<"input " << p_build.seq_filename<< std::endl;

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


    BBHashKmerContainer<KMerIterator<Dna5>,Dna5> table(1,2,length(seqs[0])/10-21,21);
    std::cout << "TEST" << std::endl;
    for(unsigned i = 0; i < length(ids) ; ++i){

        KMerIterator<Dna5> _begin = get_begin(toCString(seqs[i]),21);
        KMerIterator<Dna5> _end = get_end(toCString(seqs[i]),length(seqs[i]),21);

        std::cout << "Length : " << length(seqs[i]) << std::endl;
        table.addRange(_begin,_end);

    }

    filter_test(table);
    return 0;
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

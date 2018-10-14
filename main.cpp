#include <iostream>
#include <cstring>

#include "cmdline.h"
#include "BBHashKmerContainer.h"

#include <seqan/seq_io.h>


using namespace seqan;



int reuse_build(int argc, char **argv){
    
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
    if(strcmp(argv[1], "build")==0) {
        reuse_build(argc-1, argv+1);
    }else if(strcmp(argv[1], "filter")==0){
        reuse_filter(argc-1, argv+1);
    }else{
//Print help for reuse funcs
    }
    return 0;
}

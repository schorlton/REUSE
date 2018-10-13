#include <iostream>
#include <cstring>

#include "cmdline.h"

int reuse_build(int argc, char **argv){

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
}

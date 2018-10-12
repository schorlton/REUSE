#include <iostream>
#include <cstring>
#include "cmdline.h"

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

int reuse_build(int argc, char **argv){


}

int reuse_filter(int argc, char **argv){
    //Init thread pool

    //Read in records to queue

    //Check queue size and increase thread pool to desaturate

    //Join thread pool

    //Output statistics

    return 0;
}

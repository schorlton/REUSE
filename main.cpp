#include <iostream>
#include <cstring>

#include "cmdline.h"


int reuse_build(int argc, char **argv){
    std::cout << "Building reference......"<< std::endl;
    parameters_build p_build;
    parse_command_line_build( argc, argv, p_build);
    std::cout << "number of thread "<<p_build.threads<< std::endl;
    std::cout <<"input " << p_build.seq_filename<< std::endl;

}

int reuse_filter(int argc, char **argv){
    std::cout << "Filtering sequence......"<< std::endl;
    parameters_filter p_filter;
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
}

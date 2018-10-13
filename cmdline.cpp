#include <getopt.h>

#include <iostream>

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "cmdline.h"

CompressType parse_compress_type(char *text){
	if(strcmp(text,"gzip")==0){
		return CompressType::gzip;
	}
	else if(strcmp(text,"bzip2")==0){
		return CompressType::gzip;
	}
	else{
		//TODO make this c++ error output
		std::cerr << "Illegal compression type, Use gzip or bzip2!" << std::endl;
		reuse_exit(ExitSignal::IllegalArgumentError);
	}

}
int parse_command_line_build( int argc, char** argv, parameters_build& params)
{
	static struct option long_options[] = 
	{
			{"in"    				, required_argument, 0, 'i'},
			{"out"    				, required_argument, 0, 'o'},
			{"threads"				, required_argument, 0, 'p'},
			{"ram_limit"			, required_argument, 0, 'r'},
			{"k-mer_length"			, required_argument, 0, 'k'},
			{"mask"					, required_argument, 0, 'm'},
			{"compress"				, no_argument,		 0, 'g'},
			{"help"   				, no_argument,       0, 'h'},
			{"version"				, no_argument,       0, 'v'},

		{0        , 0,                   0,  0 }
	};
  
	if( argc == 1)
	{
		print_help();
		return -1;
	}
 	int o;
	int index;
	while( ( o = getopt_long( argc, argv, "o:p:r:k:c:m:ghv", long_options, &index)) != -1)
	{
		switch( o)
		{
			case 'i':
				set_str( &( params.seq_filename), optarg);
				break;
			case 'o':
				set_str( &( params.output_folder_name), optarg);
				break;
			case 'p':
				params.threads = atoi(optarg);
				break;
			case 'r':
				params.ram_limit = atoi(optarg);
				break;
			case 'k':
				params.min_kmer = atoi(optarg);
				break;
			case 'U':
				set_str( &( params.seq_filename) , optarg);
				break;
			case 'l':
				set_str( &( params.log_filename), optarg);
				break;
			case 'g':
				params.zip = parse_compress_type(optarg);
				break;

			case 's':
				params.paired = true;
			break;
			case 't':
				params.threads = atoi( optarg);
			break;
			case 'h':
				print_help();

				reuse_exit(ExitSignal::Success);
			break;
			case 'v':
				std::cerr <<  "\nREUSE: Rapid Elimination of Useless Sequences." << std::endl;
				reuse_exit(ExitSignal::Success);
			break; 
		}
	}
	if (params.seq_filename == NULL){
		std::cerr << "Sequence file not specified!" << std::endl;
		reuse_exit(ExitSignal::IllegalArgumentError);
	}
}

void print_help(){
	std::cerr << "Help" << std::endl;
	std::cout << "Usage: reuse <command> [options]"<< std::endl;
    std::cout << " "<< std::endl;
    std::cout << "Command"<< std::endl;
    std::cout << "      build	Identify all k-mers within a reference dataset and store that library to disk"<< std::endl;
    std::cout << "              options: reuse build [options] "<< std::endl;
    std::cout << "      filter"<< std::endl;



}


#include <getopt.h>

#include <iostream>

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "cmdline.h"

parameters_common::parameters_common(){
	output_folder_name = NULL;
	is_stdin =true;
	threads = 8;
	ram_limit=4000;
}

parameters_build::parameters_build(){
	parameters_common();
	seq_filename=NULL;
	kmer_length=21;
	mask = NULL;
}
parameters_filter::parameters_filter(){
	parameters_common();
	seq_filename_1=NULL;
	seq_filename_2=NULL;
	index_filename=NULL;
	log_filename=NULL;
	is_stdout=true;
	paired = false;
}

CompressType parse_compress_type(char *text){
	if(strcmp(text,"gzip")==0){
		return CompressType::gzip;
	}
	else if(strcmp(text,"bzip2")==0){
		return CompressType::bzip2;
	}
	else{
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
			{0       			 	, 0	,                0,  0 }
	};

	if( argc == 1)
	{
		print_help();
		return -1;
	}
 	int o;
	int index;
	while( ( o = getopt_long( argc, argv, "i:o:p:r:k:m:ghv", long_options, &index)) != -1)
	{
		switch( o)
		{
			case 'i':
				set_str( &( params.seq_filename), optarg);
				params.is_stdin = false;
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
				params.kmer_length = atoi(optarg);
				break;
			case 'm':
				set_str( &( params.mask), optarg);
				break;
			case 'g':
				params.zip = parse_compress_type(optarg);
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

int parse_command_line_filter( int argc, char** argv, parameters_filter& params)
{
	static struct option long_options[] =
			{
					{"index"    			, required_argument, 0, 'x'},
					{"in1"    				, required_argument, 0, '1'},
					{"in2"    				, required_argument, 0, '2'},
					{"out"    				, required_argument, 0, 'o'},
					{"threads"				, required_argument, 0, 'p'},
					{"ram_limit"			, required_argument, 0, 'r'},
					{"min_k-mer_len"        , required_argument, 0, 'k'},
					{"compress"				, required_argument, 0, 'g'},
					{"help"   				, no_argument,       0, 'h'},
					{"version"				, no_argument,       0, 'v'},
					{"log_file"				, required_argument, 0, 'l'},
					{0       			 	, 0	,                0,  0 }
			};

	if( argc == 1)
	{
		print_help();
		return -1;
	}
	int o;
	int index;
	while( ( o = getopt_long( argc, argv, "x:1:2:o:p:r:k:ghvl:", long_options, &index)) != -1)
	{
		switch( o)
		{
			case 'x':
				set_str( &( params.index_filename), optarg);
				break;
			case '1':
				set_str( &( params.seq_filename_1), optarg);
				params.is_stdin = false;
				break;
			case '2':
				set_str( &( params.seq_filename_2), optarg);
				params.paired = true;
				break;
			case 'o':
				set_str( &( params.output_folder_name), optarg);
				params.is_stdout = false;
				break;
			case 'p':
				params.threads = atoi(optarg);
				break;
			case 'r':
				params.ram_limit = atoi(optarg);
				break;
			case 'k':
				params.min_kmer_threshhold = atoi(optarg);
				break;
			case 'g':
				params.zip = parse_compress_type(optarg);
				break;
			case 'h':
				print_help();
				reuse_exit(ExitSignal::Success);
				break;
			case 'v':
				std::cerr <<  "\nREUSE: Rapid Elimination of Useless Sequences." << std::endl;
				reuse_exit(ExitSignal::Success);
				break;
			case 'l':
				set_str( &( params.log_filename), optarg);
				break;
		}
	}
	if (params.seq_filename_1 == NULL){
		if(params.seq_filename_2 == NULL){
			params.is_stdin=true;
		}else {
			std::cout << "you need to have filename for both -1 and -2" << std::endl;
			reuse_exit(ExitSignal::IllegalArgumentError);
		}
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


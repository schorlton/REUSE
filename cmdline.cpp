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
int parse_command_line( int argc, char** argv, parameters* params)
{
	static struct option long_options[] = 
	{
		{"input"  , required_argument,   0, 'U'},
		{"threads", required_argument,   0, 'p'},
		{"help"   , no_argument,         0, 'h'},
		{"version", no_argument,         0, 'v'},
		{"out"    , required_argument,	 0, 'o'},
		{"ram_limit", required_argument, 0, 'm'},
		{"log_file", required_argument,	 0, 'l'},
		{0        , 0,                   0,  0 }
	};
  
	if( argc == 1)
	{
		print_help();
		return -1;
	}
 	int o;
	int index;
	while( ( o = getopt_long( argc, argv, "U:p:hvo:m:l:", long_options, &index)) != -1)
	{
		switch( o)
		{

			case 'U':
				set_str( &( params->seq_filename) , optarg);
			break;
			case 'l':
				set_str( &( params->log_filename), optarg);
			break;
			case 'g':
				params->zip = parse_compress_type(optarg);
			break;
			case 'k':			
				params->min_kmer = atoi(optarg);
			break;
			case 'm':
				params->ram_limit = atoi(optarg);
			break;
			case 'o':
				set_str( &( params->output_filename), optarg);
			break;
			case 's':
				params->paired = true;
			break;
			case 't':
				params->threads = atoi( optarg);
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
	if (params->seq_filename == NULL){
		std::cerr << "Sequence file not specified!" << std::endl;
		reuse_exit(ExitSignal::IllegalArgumentError);
	}
}

void print_help(){ 
}


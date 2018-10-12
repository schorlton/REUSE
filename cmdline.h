#ifndef __COMMANDLINE
#define __COMMANDLINE

#include "common.h"

enum class CompressType { gzip, bzip2, uncompressed};



struct parameters{
	char *seq_filename;
	char *index_filename;
	char *output_filename;
	char *log_filename;

	int threads;
	long ram_limit;
	CompressType zip;

	int min_kmer;
	bool paired;
};


int parse_command_line( int, char**, parameters*);
void parse_bam_list( parameters** params);
void print_help( void);

#endif

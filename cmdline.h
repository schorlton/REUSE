#ifndef __COMMANDLINE
#define __COMMANDLINE

#include "common.h"

enum class CompressType { gzip, bzip2, uncompressed};

using string_param = char*;

struct parameters_common{

};
struct parameters_build:parameters_common{
	string_param seq_filename;
	string_param output_folder_name;
	string_param log_filename;

	int threads;
	long ram_limit;


	int min_kmer;
	bool command_line_input;
};
struct parameters_filter:parameters_common{
	string_param index_filename;
	CompressType zip;
	bool paired;
};
int parse_command_line_common(int, char**, parameters_common&);
int parse_command_line_build( int, char**, parameters_build&);
int parse_command_line_filter( int, char**, parameters_filter&);
void print_help( void);

#endif

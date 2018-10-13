#ifndef __COMMANDLINE
#define __COMMANDLINE

#include "common.h"

enum class CompressType { gzip, bzip2, uncompressed};

using string_param = char*;

struct parameters_common{

	string_param output_folder_name;
	int threads;
	long ram_limit;
	bool is_stdin;
	parameters_common();
};

struct parameters_build:parameters_common{
	string_param seq_filename;
	CompressType zip;
	int kmer_length;
	string_param mask;

	parameters_build();

};
struct parameters_filter:parameters_common{
	string_param log_filename;
	string_param seq_filename_1;
	string_param seq_filename_2;
	string_param index_filename;
	CompressType zip;
	bool paired;
	bool is_stdout;
	int min_kmer_threshhold;

	parameters_filter();
};
int parse_command_line_common(int, char**, parameters_common&);
int parse_command_line_build( int, char**, parameters_build&);
int parse_command_line_filter( int, char**, parameters_filter&);
void print_help( void);

#endif

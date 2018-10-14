#ifndef __COMMANDLINE
#define __COMMANDLINE

#include "common.h"

enum class CompressType { gzip, bzip2, uncompressed,err};

using string_param = char*;

struct ParametersCommon{

	string_param output_filename;
	int threads;
	long ram_limit;
	bool is_stdin;
	ParametersCommon();
};

struct ParametersBuild:ParametersCommon{
	string_param seq_filename;
	CompressType zip;
	int kmer_length;
	string_param mask;

	ParametersBuild();

};
struct ParametersFilter:ParametersCommon{
	string_param log_filename;
	string_param seq_filename_1;
	string_param seq_filename_2;
	string_param index_filename;
	CompressType zip;
	bool paired;
	bool is_stdout;
	int min_kmer_threshhold;

	ParametersFilter();
};
int parse_command_line_common(int, char**, ParametersCommon&);
int parse_command_line_build( int, char**, ParametersBuild&);
int parse_command_line_filter( int, char**, ParametersFilter&);
void print_help( void);

#endif

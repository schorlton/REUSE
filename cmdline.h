#ifndef __COMMANDLINE
#define __COMMANDLINE

#include <vector>
#include <string>

#include "common.h"

enum class CompressType { gzip, bzip2, uncompressed };
CompressType operator<<(CompressType& val, const std::string& str);

#include "bindopt.h"

struct ParametersCommon {
    std::string seq_filename;
    std::string output_filename;
    std::string log_filename;
    CompressType zip;
    unsigned int kmer_length;
    unsigned int threads;
    unsigned long ram_limit;
    bool verbose;
    bool help;

    ParametersCommon();
    ParametersCommon(bindopt::Options &options);
    void get_options(bindopt::Options &options);
};

struct ParametersBuild : ParametersCommon {
    std::string mask;

	ParametersBuild();
	ParametersBuild(bindopt::Options &options);
	void get_options(bindopt::Options &options);
};

struct ParametersFilter : ParametersCommon {
    std::string seq_mate_filename;
    std::string index_filename;
	unsigned int min_kmer_threshhold;

	ParametersFilter();
	ParametersFilter(bindopt::Options &options);
	void get_options(bindopt::Options &options);
};

void print_help( void);

#endif

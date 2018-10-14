#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <cstring>


enum class ExitSignal{
	Success,
	IllegalArgumentError
};


void set_str( char** target, char* source);
void reuse_exit(ExitSignal signal);

#endif

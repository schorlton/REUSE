#include "common.h"
#include <iostream>

#include <cstdlib>

void reuse_exit(ExitSignal signal){
        std::cout << "" << std::endl;
	exit(static_cast<int>(signal));
}

void set_str( char** target, char* source)
  {
          if( *target != NULL)
          {
                  free( ( *target));
          }

          if (source != NULL)
          {
                  ( *target) = ( char*) malloc( sizeof( char) * ( strlen( source) + 1));
                  strncpy( ( *target), source, ( strlen( source) + 1));
          }
          else
          {
                  ( *target) = NULL;
          }

}

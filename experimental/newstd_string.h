#ifndef NEWSTD_STRING
#define NEWSTD_STRING

#include "newstd_panic.h"

#include <stdlib.h>
#include <string.h>

typedef struct String {
    char* data;
    size_t count;
} String;

String new_String_CSTR(char const * const str){

}

#endif //NEWSTD_STRING

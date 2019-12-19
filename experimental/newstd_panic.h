#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

void _Noreturn panic_impl(
    const char * const message,
    const char * const filename,
    const char * const function,
    uintmax_t const line){
    fprintf(stderr,"panic[File=\"%s\",Func=\"%s\",Line=%" PRIuMAX "]: %s\n",
        filename, function, line, message);
    exit(1);
}

#define panic(message) panic_impl(message,__FILE__,__func__,__LINE__)

#define static_panic(message) _Static_assert(0,message)

#ifndef NEWSTD_PANIC
#define NEWSTD_PANIC

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

#define panic_if(message, ...) ((__VA_ARGS__) ? panic_impl(message,__FILE__,__func__,__LINE__) : (void)0) 

//from: https://stackoverflow.com/a/58263525
#define static_panic(message)\
    sizeof(struct{_Static_assert(0,message); int _;})

#define static_panic_if(message, ...)\
    sizeof(struct{_Static_assert(!(__VA_ARGS__),message); int _;})
#endif //NEWSTD_PANIC

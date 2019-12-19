#include <stdio.h>
#include <stdlib.h>

#define STRINGIFY_S1(x) #x
#define STRINGIFY(x) STRINGIFY_S1(x)

void _Noreturn panic(const char * const function_name, const char * const message){
    fprintf(stderr,"%s: %s\n",function_name,message);
    exit(1);
}

#define panic(message) panic(__func__,message)

#define comptime_panic(message) _Static_assert(0,__FILE__ " at line " STRINGIFY(__LINE__) ": " message)

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>
#include "tostring.h"

//NOTE: typeof((0,X)) forces X to decay from an array to a pointer
//which is necessary for type matching against _Generic, and for assigning
//to a local variable to prevent unintended side effects (arrays cannot be assigned).

#ifndef MINISTR_STRUCT
#define MINISTR_STRUCT
#include <stdint.h>
///A string type with a maximum size of 255 characters
///Uses no allocation
typedef struct ministr {
	char data[255];
	uint8_t count;
} ministr;
#endif // MINISTR_STRUCT

//If tralloc.h is not included, removes the calls
//to AddrList__remove to avoid errors and optimize.
#ifndef TRALLOC_H
#define AddrList__remove(...)
#endif //TRALLOC_H

#ifndef tool_SUPPRESS_NO_EFFECT_FILE
#define tool_SUPPRESS_NO_EFFECT_FILE
static inline FILE* tool_SuppressNoEffect_FILE(FILE* f){ return f; }
#endif // tool_SUPPRESS_NO_EFFECT_FILEPTR

static inline void print_string(string const str, FILE* strm){
    char const* s = str.data;
    if(str.data==((void*)0)){
		fprintf(strm,"(null)");
		return;
    }
    while(s!=(str.data+str.count)){
		fputc(*(s++),strm);
    }
}

static inline void print_string_in(char const * fmt, string const str, FILE* strm){
    while(*fmt!='\0'){
        if(*fmt=='{' && *(fmt+1)=='}'){
			fmt+=2;
			print_string(str,strm);
        }else fputc(*(fmt++),strm);
    }
}

static inline void print_all_string(FILE* strm, ...){
    va_list vl;
    va_start(vl,strm);
    string s = va_arg(vl,string);
    while(s.data!=((void*)0)){
        print_string(s,strm);
        s = va_arg(vl,string);
    }
    va_end(vl);
}

//Simplified and optimized printing system
#define print_string(str,strm...) print_string((str),(tool_SuppressNoEffect_FILE(stdout),##strm))
#define print_string_in(fmt,str,strm...) print_string_in((fmt),(str),(tool_SuppressNoEffect_FILE(stdout),##strm))
#define print_all(strm,strings...) print_string_all((strm),##strings,(string){.data = ((void*)0)})

#ifndef TOOL_COERCE_CP
///Forces result to be a char* solely to avoid irrelevant _Generic complaints
#define tool_CoerceCP(x)\
_Generic((x),\
	char*: (x),\
	default: ((char*)1)\
)
#endif //TOOL_COERCE_CP

#ifndef TOOL_PRINT_CP_IN
static inline void tool_print_CP_in(char const * fmt, char const * str, FILE* strm){
	while(*fmt!='\0'){
        if(*fmt=='{' && *(fmt+1)=='}'){
			fmt+=2;
			if(str)
				fputs(str,strm);
			else fputs("(null)",strm);
        }else fputc(*(fmt++),strm);
    }
}
///Efficiently prints a char* without allocation using print_in format
#define tool_print_CP_in(fmt,str,strm...) tool_print_CP_in((fmt),(str),(tool_SuppressNoEffect_FILE(stdout),##strm))
#endif //TOOL_PRINT_CP_IN

///Converts base type value x to string then, prints without newline.
#define print(X,strm...)\
do{\
_Bool _done_;\
typeof((0,X)) x = X;\
_Generic((x),\
	char*: (fputs(tool_CoerceCP(x),(tool_SuppressNoEffect_FILE(stdout),##strm)),fflush((tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	string: (print_string(ToString((x)),##strm),fflush((tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	_Bool: (fputs((!!(*(char*)&(x)) ? "true" : "false"),(tool_SuppressNoEffect_FILE(stdout),##strm)),fflush((tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	default: (_done_=0)\
);\
if(!_done_){\
	string _s_ = ToString((x));\
	print_string(_s_,##strm);\
	fflush((tool_SuppressNoEffect_FILE(stdout),##strm));\
	AddrList__remove(_s_.data);\
	free(_s_.data);\
}\
}while(0)

///Converts base type value x to string, then prints with newline.
#define println(X,strm...)\
do{\
_Bool _done_;\
typeof((0,X)) x = X;\
_Generic((x),\
    char*: (fputs(tool_CoerceCP(x),(tool_SuppressNoEffect_FILE(stdout),##strm)),fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
    string: (print_string(ToString((x)),##strm),fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
    _Bool: (fputs((!!(*(char*)&(x)) ? "true\n" : "false\n"),(tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
    default: (_done_=0)\
);\
if(!_done_){\
    string _s_ = ToString((x));\
    print_string(_s_,##strm);\
    fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm));\
	AddrList__remove(_s_.data);\
    free(_s_.data);\
}\
}while(0)

///Prints format cstring with base type value x inserted at every '{}' as a string without a newline
#define print_in(fmt,X,strm...)\
do{\
_Bool _done_;\
typeof((0,X)) x = X;\
_Generic((x),\
	char*: (tool_print_CP_in((fmt),tool_CoerceCP(x),(tool_SuppressNoEffect_FILE(stdout),##strm)),fflush((tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	string: (print_string_in((fmt),ToString((x)),##strm),fflush((tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	ministr: (print_string_in((fmt),ToString((x)),##strm),fflush((tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	_Bool: (tool_print_CP_in((fmt),(!!(*(char*)&(x)) ? "true" : "false"),(tool_SuppressNoEffect_FILE(stdout),##strm)),fflush((tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	default: (_done_=0)\
);\
if(!_done_){\
	string _s_ = ToString((x));\
	print_string_in((fmt),_s_,##strm);\
	fflush((tool_SuppressNoEffect_FILE(stdout),##strm));\
	AddrList__remove(_s_.data);\
	free(_s_.data);\
}\
}while(0)

///Prints format cstring with base type value x inserted at every '{}' as a string with a newline
#define println_in(fmt,X,strm...)\
do{\
_Bool _done_;\
typeof((0,X)) x = X;\
_Generic((x),\
	char*: (tool_print_CP_in((fmt),tool_CoerceCP(x),(tool_SuppressNoEffect_FILE(stdout),##strm)),fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	string: (print_string_in((fmt),ToString((x)),##strm),fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	ministr: (print_string_in((fmt),ToString((x)),##strm),fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	_Bool: (tool_print_CP_in((fmt),(!!(*(char*)&(x)) ? "true" : "false"),(tool_SuppressNoEffect_FILE(stdout),##strm)),fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)),_done_=1),\
	default: (_done_=0)\
);\
if(!_done_){\
	string _s_ = ToString((x));\
	print_string_in((fmt),_s_,##strm);\
	fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm));\
	AddrList__remove(_s_.data);\
	free(_s_.data);\
}\
}while(0)

///Converts custom type value x to string using type##_ToString function, then prints without newline
#define print_custom(x,type,strm...) do{ string _s_ = type##_ToString((x)); print_string(_s_,##strm); AddrList__remove(_s_.data); free(_s_.data); }while(0)
///Converts custom type value x to string using type##_ToString function, then prints with newline
#define println_custom(x,type,strm...)do{ string _s_ = type##_ToString((x)); print_string(_s_,##strm);\
									      fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); AddrList__remove(_s_.data); free(_s_.data); }while(0)
///Prints format cstring with custom type value x inserted at every '{}' as a string using type##_ToString function, not terminated by newline
#define print_in_custom(fmt,x,type,strm...) do{ string _s_ = type##_ToString((x)); print_string_in((fmt),_s_,##strm); AddrList__remove(_s_.data); free(_s_.data); }while(0)
///Prints format cstring with custom type value x inserted at every '{}' as a string using type##_ToString function, terminated by newline
#define println_in_custom(fmt,x,type,strm...)do{ string _s_ = type##_ToString((x)); print_string_in((fmt),_s_,##strm);\
									      fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); AddrList__remove(_s_.data); free(_s_.data); }while(0)

#ifndef TRALLOC_H
#undef AddrList__remove
#endif //TRALLOC_H

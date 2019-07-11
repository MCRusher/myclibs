#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "tostring.h"

#ifndef tool_SUPPRESS_NO_EFFECT_FILE
#define tool_SUPPRESS_NO_EFFECT_FILE
static inline FILE* tool_SuppressNoEffect_FILE(FILE* f){ return f; }
#endif // tool_SUPPRESS_NO_EFFECT_FILEPTR

static inline void print_string(string str, FILE* strm){
    char const* s = str.data;
    if(str.data==((void*)0)){
		fprintf(strm,"(null)");
		return;
    }
    while(s!=(str.data+str.count)){
		fputc(*(s++),strm);
    }
}

static inline void print_string_in(char * fmt, string str, FILE* strm){
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

#define print_string(str,strm...) print_string((str),(tool_SuppressNoEffect_FILE(stdout),##strm))
#define print_string_in(fmt,str,strm...) print_string_in((fmt),(str),(tool_SuppressNoEffect_FILE(stdout),##strm))
#define print_all(strm,strings...) print_string_all((strm),##strings,(string){.data = ((void*)0)})

#ifndef TRALLOC_REPLACE
#define print(x,strm...) print_string(ToString((x)),##strm)
#define println(x,strm...) do{ print_string(ToString((x)),##strm); fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); }while(0)
#define print_in(fmt,x,strm...) print_string_in((fmt),ToString((x)),##strm)
#define println_in(fmt,x,strm...) do{ print_string_in((fmt),ToString((x)),##strm);\
									  fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); }while(0)
#define print_custom(x,type,strm...) print_string(type##_ToString((x)),##strm)
#define println_custom(x,type,strm...)do{ print_string(type##_ToString((x)),##strm);\
									      fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); }while(0)
#define print_in_custom(fmt,x,type,strm...) print_string_in((fmt),type##_ToString((x)),##strm)
#define println_in_custom(fmt,x,type,strm...)do{ print_string_in((fmt),type##_ToString((x)),##strm);\
									      fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); }while(0)
#else
#define print(x,strm...) do{ trset_scope(); print_string(ToString((x)),##strm); trdealloc_scope(); }while(0)
#define println(x,strm...) do{ trset_scope(); print_string(ToString((x)),##strm); fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); trdealloc_scope(); }while(0)
#define print_in(fmt,x,strm...) do{ trset_scope(); print_string_in((fmt),ToString((x)),##strm); trdealloc_scope(); }while(0)
#define println_in(fmt,x,strm...) do{ trset_scope(); print_string_in((fmt),ToString((x)),##strm);\
									  fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); trdealloc_scope(); }while(0)
#define print_custom(x,type,strm...) do{ trset_scope(); print_string(type##_ToString((x)),##strm); trdealloc_scope(); }while(0)
#define println_custom(x,type,strm...)do{ trset_scope(); print_string(type##_ToString((x)),##strm);\
									      fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); trdealloc_scope(); }while(0)
#define print_in_custom(fmt,x,type,strm...) do{ trset_scope(); print_string_in((fmt),type##_ToString((x)),##strm); trdealloc_scope(); }while(0)
#define println_in_custom(fmt,x,type,strm...)do{ trset_scope(); print_string_in((fmt),type##_ToString((x)),##strm);\
									      fputc('\n',(tool_SuppressNoEffect_FILE(stdout),##strm)); trdealloc_scope(); }while(0)
#endif // TRALLOC_REPLACE

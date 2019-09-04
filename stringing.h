#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tralloc_override_start.h"

#ifndef STRING_STRUCT
#define STRING_STRUCT
typedef struct string{
	char* data;
	typeof(sizeof 1) count;
} string;
#define string_GetEnd(str) ((str).data+(str).count)
#endif // STRING_STRUCT

#ifndef STRING_READLN_BUFSIZE
#define STRING_READLN_BUFSIZE 200
#endif // STRING_READLN_BUFSIZE

#ifndef tool_SUPPRESS_NO_EFFECT_FILE
#define tool_SUPPRESS_NO_EFFECT_FILE
static inline FILE* tool_SuppressNoEffect_FILE(FILE* f){ return f; }
#endif // tool_SUPPRESS_NO_EFFECT_FILEPTR

static inline string string_Combine(string const str1, string const str2){
    char* s = malloc(str1.count+str2.count);
    if(!s){
		fputs("Could not combine strings.\n",stderr);
		exit(-1);
    }
	memcpy(s,str1.data,str1.count);
	memcpy(s+str1.count,str2.data,str2.count);
	return (string){.data = s, .count = str1.count+str2.count};
}

static inline string string_Create(char const * const str){
	typeof(sizeof 1) count = strlen(str);
	char* s = malloc(count);
	if(!s){
		fputs("Could not create string.\n",stderr);
		exit(-1);
    }
    memcpy(s,str,count);
    return (string){.data = s, .count = count};
}

static inline string const string_CreateConst(char const * const str){
    //because C=bad, string const has char * const data
    //instead of char const * const,
    //and assigning characters is not a comptime failure,
    //but a runtime issue if data was a string literal.
	return (string const){.data = (char*)str, .count = strlen(str)};
}

static inline string string_Copy(string const str){
    char* s = malloc(str.count);
    if(!s){
		fputs("Could not copy string.\n",stderr);
		exit(-1);
    }
    memcpy(s,str.data,str.count);
    return (string){.data = s, .count = str.count};
}

static inline string string_Empty(void){
	return (string){.data = ((void*)0), .count = 0};
}

static inline void string_Delete(string * const s){
    free(s->data);
    s->count = 0;
	s->data = ((void*)0);
}

///Returns str.count if none of char array s is not found in string str
static inline typeof(sizeof 1) string_Find(string const str, char const * const s){
    for(typeof(sizeof 1) index = 0; index != str.count; index++){
        for(char const* c = s; *c != '\0'; c++){
            if(str.data[index] == *c)
				return index;
        }
    }
    //str.count should always be past the valid indexing range.
    return str.count;
}

static inline string string_ReadLn(FILE* strm){
	typeof(sizeof 1) bufsize = STRING_READLN_BUFSIZE, count = 0;
    char* buf = malloc(STRING_READLN_BUFSIZE);
    if(!buf){
		quick_fail:
		fputs("Could not create string from stream input.\n",stderr);
		exit(-1);
	}
	do{
		buf[count++] = fgetc(strm);
		if(count==bufsize){
				bufsize *= 2;
			buf = realloc(buf,bufsize);
			if(!buf) goto quick_fail;
		}
	}while(buf[count-1]!='\n');
	char* str = malloc(count-1);
	if(!str) goto quick_fail;
	memcpy(str,buf,count-1);
	free(buf);
	return (string){.data = str, .count = count-1};
}
#define string_ReadLn(strm...) string_ReadLn((tool_SuppressNoEffect_FILE(stdin),##strm))

///Returns an unallocated string const with adjusted data and count from s to shrink away whitespace
///The result will not be null terminated
string const string_TrimConst(string const s){
    if(s.count != 0){
		typeof(sizeof 1) b = 0, e = s.count;
		for(char const* c = &s.data[0]; isspace(*c); c++)
			++b;
		for(char const* c = &s.data[s.count-1]; isspace(*c); c--)
			--e;
		return (string const){.data = &s.data[b], .count = e-b};
    }else return (string const)s;
}

///Returns a new string with all end spacing removed at both ends
///String will no longer be null terminated.
static inline string string_Trim(string const s){
    if(s.count != 0){
        typeof(sizeof 1) b = 0, e = s.count;
        for(char const* c = &s.data[0]; isspace(*c); c++)
			++b;
		for(char const* c = &s.data[s.count-1]; isspace(*c); c--)
			--e;
        char* tmp = malloc(e-b);
        if(!tmp){
			fputs("Could not trim string.\n",stderr);
			exit(-1);
        }
        return (string){.data = memcpy(tmp,&s.data[b],e-b), .count = e-b};
    }else return string_Copy(s);
}

_Bool string_IsZero(string const s){
	if(s.count==1 && s.data[0]=='0')
		return 1;
	else if(s.count==2 && s.data[1] && (s.data[0] == '+' || s.data[0]=='-'))
		return 1;
	else return 0;
}

///Modifies s, then returns it by value
string string_NullTerminate(string * const s){
    s->data = realloc(s->data,s->count+1);
    if(!s->data){
		fputs("Could not null terminate string.\n",stderr);
		exit(-1);
    }
    s->data[s->count] = '\0';
    return (string)*s;
}

#undef STRING_READLN_BUFSIZE

#include "tralloc_override_end.h"

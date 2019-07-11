#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tralloc_override_start.h"

#ifndef STRING_STRUCT
#define STRING_STRUCT
typedef struct string{
	char* data;
	typeof(sizeof 1) count;
} string;
#define string_GetEnd(str) ((str).data+(str).count)
#endif // STRING_STRUCT

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

static inline string string_Create(char const * str){
	typeof(sizeof 1) count = strlen(str);
	char* s = malloc(count);
	if(!s){
		fputs("Could not creat string.\n",stderr);
		exit(-1);
    }
    memcpy(s,str,count);
    return (string){.data = s, .count = count};
}

static inline string string_Copy(string const str){
    char* s = malloc(str.count);
    if(!s){
		fputs("Could not combine strings.\n",stderr);
		exit(-1);
    }
    memcpy(s,str.data,str.count);
    return (string){.data = s, .count = str.count};
}

static inline string string_Empty(void){
	return (string){.data = ((void*)0), .count = 0};
}

static inline void string_Delete(string* s){
    free(s->data);
    s->count = 0;
}

static inline typeof(sizeof 1) string_Find(string const str, char const * const s){
	typeof(sizeof 1) s_len = strlen(s);
    for(typeof(sizeof 1) index = 0; index != str.count; index++){
        for(char const* c = s; c != s+s_len; c++){
            if(str.data[index] == *c)
				return index;
        }
    }
    //str.count should always be past the valid indexing range.
    return str.count;
}

#include "tralloc_override_end.h"

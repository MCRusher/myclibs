#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#ifndef STRING_STRUCT
#define STRING_STRUCT
typedef struct string{
	char* data;
	typeof(sizeof 1) count;
} string;
#define string_GetEnd(str) ((str).data+(str).count)
#endif // STRING_STRUCT

static inline int64_t string_ToI64(string const s){
	if(s.count==0) goto quick_fail;
	int64_t result = 0;
    uint64_t multiplier = (uint64_t)pow(10,s.count-2);
    //typeof(sizeof 1) const count = s.count;
    //does not allow decimals
	for(typeof(sizeof 1) index = 1; index < s.count; index++){
		if(!isdigit(s.data[index]))
			goto quick_fail;
        result += (s.data[index]-'0')*multiplier;
        multiplier /= 10;
	}
	switch(s.data[0]){
        case '-': return -result;
        case '0'...'9': result+= (s.data[0]-'0')*(uint64_t)pow(10,s.count-1);
        case '+': return +result;
        default:{
			quick_fail:
			fputs("Could not convert string to iX.\n",stderr);
			exit(-1);
        }
	}
}
static inline int8_t string_ToI8(string const s){ return string_ToI64(s); }
static inline int16_t string_ToI16(string const s){ return string_ToI64(s); }
static inline int32_t string_ToI32(string const s){ return string_ToI64(s); }
static inline uint64_t string_ToU64(string const s){
	if(s.count==0) goto quick_fail;
	uint64_t result = 0;
    uint64_t multiplier = (uint64_t)pow(10,s.count-2);
    //does not allow decimals
	for(typeof(sizeof 1) index = 1; index < s.count; index++){
		if(!isdigit(s.data[index]))
			goto quick_fail;
        result += (s.data[index]-'0')*multiplier;
        multiplier /= 10;
	}
	switch(s.data[0]){
        case '0'...'9': result += (s.data[0]-'0')*(uint64_t)pow(10,s.count-1);
        case '+': return +result;
        default:{//char is a negative or not a digit
			quick_fail:
			fputs("Could not convert string to iX.\n",stderr);
			exit(-1);
        }
	}
}
static inline uint8_t string_ToU8(string const s){ return string_ToU64(s); }
static inline uint16_t string_ToU16(string const s){ return string_ToU64(s); }
static inline uint32_t string_ToU32(string const s){ return string_ToU64(s); }

static inline long double string_ToFlong(string const s){
    if(s.count==3 &&
	  (s.data[0]=='N' && s.data[1]=='a' && s.data[2]=='N'))
		return 0.0/0.0;
	else if(s.count==4 &&
	  (s.data[1]=='i' && s.data[2]=='n' && s.data[3]=='f') && (s.data[0]=='+' || s.data[0]=='-'))
		return s.data[0]=='-' ? -1.0/0.0 : 1.0/0.0;
	else{
		char* buf = malloc(s.count+1);
		if(!buf){
			fputs("Could not convert string to fX.\n",stderr);
			exit(-1);
		}
		memcpy(buf,s.data,s.count);
		buf[s.count] = '\0';
		long double result = strtold(buf,((void*)0));
		free(buf);
		return result;
	}
}
static inline float string_ToF32(string const s){ return string_ToFlong(s); }
static inline float string_ToF64(string const s){ return string_ToFlong(s); }

static inline char string_ToChar(string const s){
	if(s.count!=1){
		fputs("Could not convert string to char.\n",stderr);
		exit(-1);
	}
}

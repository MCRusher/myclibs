#pragma once
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

//Allows overriding tostring allocations with
//tralloc equivalents to allow scoped memory management
//for use with trprinting.h and others that rely
//on temporary dynamic allocation
#include "tralloc_override_start.h"


#ifndef STRING_STRUCT
#define STRING_STRUCT
typedef struct string{
	char* data;
	typeof(sizeof 1) count;
} string;
#define string_GetEnd(str) ((str).data+(str).count)
#endif // STRING_STRUCT

#ifndef MINISTR_STRUCT
#define MINISTR_STRUCT
#include <stdint.h>
///A string type with a maximum size of 255 characters
///Uses no allocation
typedef struct ministr {
	char data[255];
	uint8_t count;
} ministr;
#define ministr_GetEnd(mstr) ((mstr).data+(mstr).count)
#endif // MINISTR_STRUCT



#ifndef TOOL_GET_DIGITS
#define TOOL_GET_DIGITS
//math expression from John Bollinger
//https://stackoverflow.com/questions/43787672/the-max-number-of-digits-in-an-int-based-on-number-of-bits
#define tool_GetDigits(type...) (241*sizeof(type)/100+1)
#endif // TOOL_GET_DIGITS

//obsolete
//#if defined(__SIZEOF_INT128__) && !defined(TOOL_ABS64)
//#define TOOL_ABS64
////goal is to portably get the
////absolute value of num (INT64_MAX),
////which requires at least 19 digits
////of precision/storage
//static inline __int128 tool_abs64(int64_t num){
//    if(num<0){
//		__int128 tmp = num;
//		tmp *= -1;
//		return tmp;
//    }else return num;
//}
//#endif // TOOL_ABS64

#ifndef tool_SUPPRESS_NO_EFFECT_UINT8_T
#define tool_SUPPRESS_NO_EFFECT_UINT8_T
static inline uint8_t tool_SuppressNoEffect_uint8_t(uint8_t i){ return i; }
#endif // tool_SUPPRESS_NO_EFFECT

#if defined(__MINGW32__) && (!defined(__USE_MINGW_ANSI_STDIO) || __USE_MINGW_ANSI_STDIO != 1)
#define snprintf __mingw_snprintf
#endif

static inline string bool_ToString(_Bool b){
	char* str;
	str = malloc(4+!b);
	if(!str){
		fputs("Could not convert bool to string.\n",stderr);
    	exit(-1);
	}
	if(b){
		str[0] = 't';
		str[1] = 'r';
		str[2] = 'u';
		str[3] = 'e';
	}else{
		str[0] = 'f';
		str[1] = 'a';
		str[2] = 'l';
		str[3] = 's';
		str[4] = 'e';
	}
	return (string){.data = str, .count = 4+!b};
}

static inline string i32_ToString(int32_t d){
	char str[tool_GetDigits(int32_t)+2] = {0};
	typeof(sizeof 1) index = 0, count;
	str[index++] = (d>=0 ? '+' : '-');
    int32_t shift = d;
    do{
		++index;
		shift /= 10;
    }while(shift);
    //printf("index: %zu\n",index);
    count = index;
    do{
		//printf("d%%10 = %d\n",d%10);
        str[--index] = (llabs(d)%10)+'0';
        d /=10;
    }while(d);
	char* tmp = malloc(count);
	if(!tmp){
		fputs("Could not convert iX to string.\n",stderr);
		exit(-1);
	}
	return (string){.data = memcpy(tmp,str,count), .count = count};
}
static inline string u32_ToString(uint32_t d){
	char str[tool_GetDigits(uint32_t)+2] = {'+'};
	typeof(sizeof 1) index = 1, count;
    uint32_t shift = d;
    do{
		++index;
		shift /= 10;
    }while(shift);
    //printf("index: %zu\n",index);
    count = index;
    do{
		//printf("d%%10 = %d\n",d%10);
        str[--index] = (d%10)+'0';
        d /=10;
    }while(d);
	char* tmp = malloc(count);
	if(!tmp){
		fputs("Could not convert uX to string.\n",stderr);
		exit(-1);
	}
	return (string){.data = memcpy(tmp,str,count), .count = count};
}
static inline string i8_ToString(int8_t d){ return i32_ToString(d); }
static inline string u8_ToString(uint8_t d){ return u32_ToString(d); }
static inline string i16_ToString(int16_t d){ return i32_ToString(d); }
static inline string u16_ToString(uint16_t d){ return u32_ToString(d); }

static inline string i64_ToString(int64_t d){
	char str[tool_GetDigits(int64_t)+2] = {0};
	typeof(sizeof 1) index = 0, count;
	str[index++] = (d>=0 ? '+' : '-');
    int64_t shift = d;
    do{
		++index;
		shift /= 10;
    }while(shift);
    //printf("index: %zu\n",index);
    count = index;
    do{
		//printf("d%%10 = %d\n",d%10);

        str[--index] =
		//all below code was made obsolete by changing operator order so that
		//the int passed to llabs is never greater than INT64_MAX
		//(LLONG_MAX guaranteed at least 64 bits)
			llabs(d % 10)+'0';
        /*#if LDBL_DIG >= 19
		//	(((int64_t)fabsl((long double)d))%10)+'0';
		//#elif defined(TOOL_ABS64)
		//	(tool_abs64(d)%10)+'0';
		//#else
		//	#error No system supported way of getting the absolute value of an int64_t\
		//		   is supported/could be implemented, so no implementation of i64_ToString is available.
		//#endif*/
        d /=10;
    }while(d);
	char* tmp = malloc(count);
	if(!tmp){
		fputs("Could not convert i64 to string.\n",stderr);
		exit(-1);
	}
	return (string){.data = memcpy(tmp,str,count), .count = count};
}
static inline string u64_ToString(uint64_t d){
	char str[tool_GetDigits(uint64_t)+2] = {'+'};
	typeof(sizeof 1) index = 1, count;
    uint64_t shift = d;
    do{
		++index;
		shift /= 10;
    }while(shift);
    //printf("index: %zu\n",index);
    count = index;
    do{
		//printf("d%%10 = %d\n",d%10);
        str[--index] = (d%10)+'0';
        d /=10;
    }while(d);
	char* tmp = malloc(count);
	if(!tmp){
		fputs("Could not convert u64 to string.\n",stderr);
		exit(-1);
	}
	return (string){.data = memcpy(tmp,str,count), .count = count};
}

static inline string flong_ToString(long double f/*, uint8_t digs*/){
	char* str;
	int64_t count;
	if(isnan(f)){
		str = malloc(3);
		str[0] = 'N';
		str[1] = 'a';
		str[2] = 'N';
		count = 3;
	}else if(isinf(f)){
		str = malloc(4);
		str[0] = (signbit(f) ? '-' : '+');
		str[1] = 'i';
		str[2] = 'n';
		str[3] = 'f';
		count = 4;
	}else{//should be a normal value
		//this version uses huge buffer,
		//allowing snprintf of 128-bit IEEE754 LDBL_MAX value.
		char* buf = calloc(1,4942);
		if(!buf) goto quick_err;
        if(f >= +0.0 || f >= -0.0) buf[0] = '+';
        count = snprintf(&buf[(f >= +0.0 || f >= -0.0)],4941,"%Lf",f);
        if(count>=0 && count<4941){
			str = malloc(count);
			memcpy(str,buf,count);
        }else str = ((void*)0);
        free(buf);
	}
	if(!str || count<0){
		quick_err:
		fputs("Could not convert flong to string.\n",stderr);
		exit(-1);
	}
	return (string){.data = str, .count = count};
}
static inline string f64_ToString(double f){
	char* str;
	int64_t count;
	if(isnan(f)){
		str = malloc(3);
		str[0] = 'N';
		str[1] = 'a';
		str[2] = 'N';
		count = 3;
	}else if(isinf(f)){
		str = malloc(4);
		str[0] = (signbit(f) ? '-' : '+');
		str[1] = 'i';
		str[2] = 'n';
		str[3] = 'f';
		count = 4;
	}else{//should be a normal value
		//just picked a big number that should hold
		//all IEEE754 f32 and f64 values.
		char buf[320] = {0};
        if(f >= +0.0 || f >= -0.0) buf[0] = '+';
        count = snprintf(&buf[(f >= +0.0 || f >= -0.0)],501,"%lf",f);
        if(count>=0 && count<501){
			str = malloc(count);
			memcpy(str,buf,count);
        }else str = ((void*)0);
	}
	if(!str || count<0){
		fputs("Could not convert fX to string.\n",stderr);
		exit(-1);
	}
	return (string){.data = str, .count = count};
}
static inline string f32_ToString(float f){ return f64_ToString(f); }

static inline string cstring_ToString(char const* s){
	typeof(sizeof 1) count = strlen(s);
	char* tmp = malloc(count);
	if(!tmp){
		fputs("Could not convert cstring to string.\n",stderr);
		exit(-1);
	}
	return (string){.data = memcpy(tmp,s,count), .count = count};
}

static inline string char_ToString(char c){
	char* tmp = malloc(1);
	if(!tmp){
		fputs("Could not convert char to string.\n",stderr);
		exit(-1);
	}
	*tmp = c;
	return (string){.data = tmp, .count = 1};
}

static inline string ministr_ToString(ministr ms){
	char* tmp = malloc(ms.count);
	if(!tmp){
		fputs("Could not convert char to string.\n",stderr);
		exit(-1);
	}
	memcpy(tmp,ms.data,ms.count);
	return (string){.data = tmp, .count = ms.count};
}

static inline string string_ToString(string s){ return s; }

static inline string invalid_ToString(){ return (string){.data = ((void*)0), .count = 0}; }

#define ToString(x)\
_Generic((x),\
	long double: flong_ToString,\
	default: _Generic((x),\
	_Bool: bool_ToString,\
	int8_t: i8_ToString,\
	uint8_t: u8_ToString,\
	int16_t: i16_ToString,\
	uint16_t: u16_ToString,\
	int32_t: i32_ToString,\
	uint32_t: u32_ToString,\
	int64_t: i64_ToString,\
	uint64_t: u64_ToString,\
	float: f32_ToString,\
	double: f64_ToString,\
	char*: cstring_ToString,\
	char: char_ToString,\
	ministr: ministr_ToString,\
	string: string_ToString,\
	default: invalid_ToString\
))((x))

#ifdef snprintf
#undef snprintf
#endif // snprintf

#include "tralloc_override_end.h"

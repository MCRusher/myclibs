#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

static inline char i64_ToChar(int64_t i){
	if((i>SCHAR_MAX || i<SCHAR_MIN)
    && (i>UCHAR_MAX || i<0)){
        fputs("Could not convert iX to char.\n",stderr);
        exit(-1);
	}
	return (char)i;
}
static inline char u64_ToChar(uint64_t i){
	if(i>UCHAR_MAX){
        fputs("Could not convert uX to char.\n",stderr);
        exit(-1);
	}
	return (char)i;
}
static inline char i8_ToChar(int8_t i){ return i64_ToChar(i); }
static inline char u8_ToChar(uint8_t i){ return u64_ToChar(i); }
static inline char i16_ToChar(int16_t i){ return i64_ToChar(i); }
static inline char u16_ToChar(uint16_t i){ return u64_ToChar(i); }
static inline char i32_ToChar(int32_t i){ return i64_ToChar(i); }
static inline char u32_ToChar(uint32_t i){ return u64_ToChar(i); }
static inline char flong_ToChar(long double f){
	f = roundl(f);
	if((f>SCHAR_MAX || f<SCHAR_MIN)
    && (f>UCHAR_MAX || f<0)){
		fputs("Could not convert fX to char.\n",stderr);
        exit(-1);
    }
    return (char)f;
}
static inline char f32_ToChar(float f){ return flong_ToChar(f); }
static inline char f64_ToChar(double f){ return flong_ToChar(f); }
static inline char char_ToChar(char c){ return c; }
static inline char invalid_ToChar(){ return (char)0;}
#define ToChar(x)\
_Generic((x),\
	int8_t: i8_ToChar,\
	uint8_t: u8_ToChar,\
	int16_t: i16_ToChar,\
	uint16_t: u16_ToChar,\
	int32_t: i32_ToChar,\
	uint32_t: u32_ToChar,\
	int64_t: i64_ToChar,\
	uint64_t: u64_ToChar,\
	float: f32_ToChar,\
	double: f64_ToChar,\
	long double: flong_ToChar,\
	char: char_ToChar,\
	default: invalid_ToChar\
)((x))

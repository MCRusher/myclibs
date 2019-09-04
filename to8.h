#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static inline int8_t i64_ToI8(int64_t i){
    if(i>INT8_MAX || i<INT8_MIN){
		//fputs("Could not convert iX to i8.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (int8_t)i;
}
static inline int8_t u64_ToI8(uint64_t i){
    if(i>INT8_MAX){
		//fputs("Could not convert uX to i8.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (int8_t)i;
}
static inline int8_t i8_ToI8(int8_t i){ return i; }
static inline int8_t u8_ToI8(uint8_t i){ return u64_ToI8(i); }
static inline int8_t i16_ToI8(int16_t i){ return i64_ToI8(i); }
static inline int8_t u16_ToI8(uint16_t i){ return u64_ToI8(i); }
static inline int8_t i32_ToI8(int32_t i){ return i64_ToI8(i); }
static inline int8_t u32_ToI8(uint32_t i){ return u64_ToI8(i); }
static inline int8_t flong_ToI8(long double f){
	f = roundl(f);
	if(f>INT8_MAX || f<INT8_MIN){
		//fputs("Could not convert fX to i8.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (int8_t)f;
}
static inline int8_t f32_ToI8(float f){ return flong_ToI8(f); }
static inline int8_t f64_ToI8(double f){ return flong_ToI8(f); }
static inline int8_t invalid_ToI8(){ return (int8_t)0; }
#define ToI8(x)\
_Generic((x),\
	int8_t: i8_ToI8,\
	uint8_t: u8_ToI8,\
	int16_t: i16_ToI8,\
	uint16_t: u16_ToI8,\
	int32_t: i32_ToI8,\
	uint32_t: u32_ToI8,\
	int64_t: i64_ToI8,\
	uint64_t: u64_ToI8,\
	float: f32_ToI8,\
	double: f64_ToI8,\
    long double: flong_ToI8,\
    default: invalid_ToI8\
)((x))
//unsigned
static inline uint8_t i64_ToU8(int64_t i){
    if(i>UINT8_MAX || i<0){
		//fputs("Could not convert iX to u8.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (uint8_t)i;
}
static inline uint8_t u64_ToU8(uint64_t i){
    if(i>UINT8_MAX){
		//fputs("Could not convert uX to u8.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (uint8_t)i;
}
static inline uint8_t i8_ToU8(int8_t i){ return i64_ToU8(i); }
static inline uint8_t u8_ToU8(uint8_t i){ return i; }
static inline uint8_t i16_ToU8(int16_t i){ return i64_ToU8(i); }
static inline uint8_t u16_ToU8(uint16_t i){ return u64_ToU8(i); }
static inline uint8_t i32_ToU8(int32_t i){ return i64_ToU8(i); }
static inline uint8_t u32_ToU8(uint32_t i){ return u64_ToU8(i); }
static inline uint8_t flong_ToU8(long double f){
	f = roundl(f);
	if(f>UINT8_MAX || f<0.0){
		//fputs("Could not convert fX to u8.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (uint8_t)f;
}
static inline uint8_t f32_ToU8(float f){ return flong_ToU8(f); }
static inline uint8_t f64_ToU8(double f){ return flong_ToU8(f); }
static inline uint8_t invalid_ToU8(){ return (uint8_t)0; }
#define ToU8(x)\
_Generic((x),\
	int8_t: i8_ToU8,\
	uint8_t: u8_ToU8,\
	int16_t: i16_ToU8,\
	uint16_t: u16_ToU8,\
	int32_t: i32_ToU8,\
	uint32_t: u32_ToU8,\
	int64_t: i64_ToU8,\
	uint64_t: u64_ToU8,\
	float: f32_ToU8,\
	double: f64_ToU8,\
    long double: flong_ToU8,\
    default: invalid_ToU8\
)((x))

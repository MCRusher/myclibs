#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static inline int64_t i64_ToI64(int64_t i){ return i; }
static inline int64_t u64_ToI64(uint64_t i){
	if(i>INT64_MAX){
		fputs("Could not convert u64 to i64.\n",stderr);
		exit(-1);
    }
    return (int64_t)i;
}
static inline int64_t i8_ToI64(int8_t i){ return (int64_t)i; }
static inline int64_t u8_ToI64(uint8_t i){ return (int64_t)i; }
static inline int64_t i16_ToI64(int16_t i){ return (int64_t)i; }
static inline int64_t u16_ToI64(uint16_t i){ return (int64_t)i; }
static inline int64_t i32_ToI64(int32_t i){ return (int64_t)i; }
static inline int64_t u32_ToI64(uint32_t i){ return (int64_t)i; }
static inline int64_t flong_ToI64(long double f){
	f = roundl(f);
	if(f>INT64_MAX || f<INT64_MIN){
		fputs("Could not convert fX to i64.\n",stderr);
		exit(-1);
    }
    return (int64_t)f;
}
static inline int64_t f32_ToI64(float f){ return flong_ToI64(f); }
static inline int64_t f64_ToI64(double f){ return flong_ToI64(f); }
static inline int64_t invalid_ToI64(){ return (int64_t)0; }
#define ToI64(x)\
_Generic((x),\
	int8_t: i8_ToI64,\
	uint8_t: u8_ToI64,\
	int16_t: i16_ToI64,\
	uint16_t: u16_ToI64,\
	int32_t: i32_ToI64,\
	uint32_t: u32_ToI64,\
	int64_t: i64_ToI64,\
	uint64_t: u64_ToI64,\
	float: f32_ToI64,\
	double: f64_ToI64,\
	long double: flong_ToI64,\
	default: invalid_ToI64\
)((x))
//unsigned
static inline uint64_t i64_ToU64(int64_t i){
	if(i<0){
		fputs("Could not convert i64 to u64.\n",stderr);
		exit(-1);
    }
    return (uint64_t)i;
}
static inline uint64_t u64_ToU64(uint64_t i){ return i; }
static inline uint64_t i8_ToU64(int8_t i){ return i64_ToU64(i); }
static inline uint64_t u8_ToU64(uint8_t i){ return (uint64_t)i; }
static inline uint64_t i16_ToU64(int16_t i){ return i64_ToU64(i); }
static inline uint64_t u16_ToU64(uint16_t i){ return (uint64_t)i; }
static inline uint64_t i32_ToU64(int32_t i){ return i64_ToU64(i); }
static inline uint64_t u32_ToU64(uint32_t i){ return (uint64_t)i; }
static inline uint64_t flong_ToU64(long double f){
	f = roundl(f);
	if(f>UINT64_MAX || f<0.0){
		fputs("Could not convert fX to u64.\n",stderr);
		exit(-1);
    }
    return (uint64_t)f;
}
static inline uint64_t f32_ToU64(float f){ return flong_ToU64(f); }
static inline uint64_t f64_ToU64(double f){ return flong_ToU64(f); }
static inline uint64_t invalid_ToU64(){ return (uint64_t)0; }
#define ToU64(x)\
_Generic((x),\
	int8_t: i8_ToU64,\
	uint8_t: u8_ToU64,\
	int16_t: i16_ToU64,\
	uint16_t: u16_ToU64,\
	int32_t: i32_ToU64,\
	uint32_t: u32_ToU64,\
	int64_t: i64_ToU64,\
	uint64_t: u64_ToU64,\
	float: f32_ToU64,\
	double: f64_ToU64,\
    long double: flong_ToU64,\
    default: invalid_ToU64\
)((x))

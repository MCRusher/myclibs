#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static inline int32_t i64_ToI32(int64_t i){
    if(i>INT32_MAX || i<INT32_MIN){
		//fputs("Could not convert iX to i32.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (int32_t)i;
}
static inline int32_t u64_ToI32(uint64_t i){
    if(i>INT32_MAX){
		//fputs("Could not convert uX to i32.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (int32_t)i;
}
static inline int32_t i8_ToI32(int8_t i){ return (int32_t)i; }
static inline int32_t u8_ToI32(uint8_t i){ return (int32_t)i; }
static inline int32_t i16_ToI32(int16_t i){ return (int32_t)i; }
static inline int32_t u16_ToI32(uint16_t i){ return (int32_t)i; }
static inline int32_t i32_ToI32(int32_t i){ return i; }
static inline int32_t u32_ToI32(uint32_t i){ return u64_ToI32(i); }
static inline int32_t flong_ToI32(long double f){
	f = roundl(f);
	if(f>INT32_MAX || f<INT32_MIN){
		//fputs("Could not convert fX to i32.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (int32_t)f;
}
static inline int32_t f32_ToI32(float f){ return flong_ToI32(f); }
static inline int32_t f64_ToI32(double f){ return flong_ToI32(f); }
static inline int32_t invalid_ToI32(){ return (int32_t)0; }
#define ToI32(x)\
_Generic((x),\
	int8_t: i8_ToI32,\
	uint8_t: u8_ToI32,\
	int16_t: i16_ToI32,\
	uint16_t: u16_ToI32,\
	int32_t: i32_ToI32,\
	uint32_t: u32_ToI32,\
	int64_t: i64_ToI32,\
	uint64_t: u64_ToI32,\
	float: f32_ToI32,\
	double: f64_ToI32,\
    long double: flong_ToI32,\
    default: invalid_ToI32\
)((x))
//unsigned
static inline uint32_t i64_ToU32(int64_t i){
    if(i>UINT32_MAX || i<0){
		//fputs("Could not convert iX to u32.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (uint32_t)i;
}
static inline uint32_t u64_ToU32(uint64_t i){
    if(i>UINT32_MAX){
		//fputs("Could not convert uX to u32.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (uint32_t)i;
}
static inline uint32_t i8_ToU32(int8_t i){ return i64_ToU32(i); }
static inline uint32_t u8_ToU32(uint8_t i){ return (uint32_t)i; }
static inline uint32_t i16_ToU32(int16_t i){ return i64_ToU32(i); }
static inline uint32_t u16_ToU32(uint16_t i){ return (uint32_t)i; }
static inline uint32_t i32_ToU32(int32_t i){ return i64_ToU32(i); }
static inline uint32_t u32_ToU32(uint32_t i){ return i; }
static inline uint32_t flong_ToU32(long double f){
	f = roundl(f);
	if(f>UINT32_MAX || f<0.0){
		//fputs("Could not convert fX to u8.\n",stderr);
		//exit(-1);
		return 0;
    }
    return (uint32_t)f;
}
static inline uint32_t f32_ToU32(float f){ return flong_ToU32(f); }
static inline uint32_t f64_ToU32(double f){ return flong_ToU32(f); }
static inline uint32_t invalid_ToU32(){ return (uint32_t)0; }
#define ToU32(x)\
_Generic((x),\
	int8_t: i8_ToU32,\
	uint8_t: u8_ToU32,\
	int16_t: i16_ToU32,\
	uint16_t: u16_ToU32,\
	int32_t: i32_ToU32,\
	uint32_t: u32_ToU32,\
	int64_t: i64_ToU32,\
	uint64_t: u64_ToU32,\
	float: f32_ToU32,\
	double: f64_ToU32,\
    long double: flong_ToU32,\
    default: invalid_ToU32\
)((x))

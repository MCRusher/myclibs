#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static inline int16_t i64_ToI16(int64_t i){
    if(i>INT16_MAX || i<INT16_MIN){
		fputs("Could not convert iX to i16.\n",stderr);
		exit(-1);
    }
    return (int16_t)i;
}
static inline int16_t u64_ToI16(uint64_t i){
    if(i>INT16_MAX){
		fputs("Could not convert uX to i16.\n",stderr);
		exit(-1);
    }
    return (int16_t)i;
}
static inline int16_t i8_ToI16(int8_t i){ return (int16_t)i; }
static inline int16_t u8_ToI16(uint8_t i){ return (int16_t)i; }
static inline int16_t i16_ToI16(int16_t i){ return i; }
static inline int16_t u16_ToI16(uint16_t i){ return u64_ToI16(i); }
static inline int16_t i32_ToI16(int32_t i){ return i64_ToI16(i); }
static inline int16_t u32_ToI16(uint32_t i){ return u64_ToI16(i); }
static inline int16_t flong_ToI16(long double f){
	f = roundl(f);
	if(f>INT16_MAX || f<INT16_MIN){
		fputs("Could not convert fX to i16.\n",stderr);
		exit(-1);
    }
    return (int16_t)f;
}
static inline int16_t f32_toI16(float f){ return flong_ToI16(f); }
static inline int16_t f64_toI16(double f){ return flong_ToI16(f); }
static inline int16_t invalid_toI16(){ return (int16_t)0; }
#define ToI16(x)\
_Generic((x),\
	int8_t: i8_ToI16,\
	uint8_t: u8_ToI16,\
	int16_t: i16_ToI16,\
	uint16_t: u16_ToI16,\
	int32_t: i32_ToI16,\
	uint32_t: u32_ToI16,\
	int64_t: i64_ToI16,\
	uint64_t: u64_ToI16,\
	float: f32_ToI16,\
	double: f64_ToI16,\
    long double: flong_ToI16,\
    default: invalid_ToI16\
)((x))
//unsigned
static inline uint16_t i64_ToU16(int64_t i){
    if(i>UINT16_MAX || i<0){
		fputs("Could not convert iX to u16.\n",stderr);
		exit(-1);
    }
    return (uint16_t)i;
}
static inline uint16_t u64_ToU16(uint64_t i){
    if(i>UINT16_MAX){
		fputs("Could not convert uX to u16.\n",stderr);
		exit(-1);
    }
    return (uint16_t)i;
}
static inline uint16_t i8_ToU16(int8_t i){ return i64_ToU16(i); }
static inline uint16_t u8_ToU16(uint8_t i){ return (uint16_t)i; }
static inline uint16_t i16_ToU16(int16_t i){ return i64_ToU16(i); }
static inline uint16_t u16_ToU16(uint16_t i){ return i; }
static inline uint16_t i32_ToU16(int32_t i){ return i64_ToU16(i); }
static inline uint16_t u32_ToU16(uint32_t i){ return u64_ToU16(i); }
static inline uint16_t flong_ToU16(long double f){
	f = roundl(f);
	if(f>UINT16_MAX || f<0.0){
		fputs("Could not convert fX to u8.\n",stderr);
		exit(-1);
    }
    return (uint16_t)f;
}
static inline uint16_t f32_toU16(float f){ return flong_ToU16(f); }
static inline uint16_t f64_toU16(double f){ return flong_ToU16(f); }
static inline uint16_t invalid_toU16(){ return (uint16_t)0; }
#define ToU16(x)\
_Generic((x),\
	int8_t: i8_ToU16,\
	uint8_t: u8_ToU16,\
	int16_t: i16_ToU16,\
	uint16_t: u16_ToU16,\
	int32_t: i32_ToU16,\
	uint32_t: u32_ToU16,\
	int64_t: i64_ToU16,\
	uint64_t: u64_ToU16,\
	float: f32_ToU16,\
	double: f64_ToU16,\
    long double: flong_ToU16,\
    default: invalid_ToU16\
)((x))


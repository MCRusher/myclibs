#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

//lossless
static inline long double i8_ToFlong(int8_t i){ return (long double)i; }
static inline long double u8_ToFlong(uint8_t i){ return (long double)i; }
static inline long double i16_ToFlong(int16_t i){ return (long double)i; }
static inline long double u16_ToFlong(uint16_t i){ return (long double)i; }
static inline long double i32_ToFlong(int32_t i){ return (long double)i; }
static inline long double u32_ToFlong(uint32_t i){ return (long double)i; }
static inline long double f32_ToFlong(float f){ return (long double)f; }
static inline long double f64_ToFlong(double f){ return (long double)f; }
static inline long double flong_ToFlong(long double f){ return f; }
//potentially lossy
static inline long double i64_ToFlong(int64_t i){//this might be correct I think (maybe)
	//represents the limit value of exactness for integers
	//to convert to floats with no loss.
	//past this point exist possible holes
	//that cannot represent values exactly
    if(i>(int64_t)pow(2,LDBL_MANT_DIG) || i<(int64_t)-pow(2,LDBL_MANT_DIG)){
        //fputs("Could not convert i64 to flong.\n",stderr);
        //exit(-1);
        return 0;
    }
    return (long double)i;
}
static inline long double u64_ToFlong(uint64_t i){
    if(i>(uint64_t)pow(2,LDBL_MANT_DIG)){
        //fputs("Could not convert u64 to flong.\n",stderr);
        //exit(-1);
        return 0;
    }
    return (long double)i;
}
static inline long double invalid_ToFlong(){ return (long double)0.0; }
#define ToFlong(x)\
_Generic((x),\
	int8_t: i8_ToFlong,\
	uint8_t: u8_ToFlong,\
	int16_t: i16_ToFlong,\
	uint16_t: u16_ToFlong,\
	int32_t: i32_ToFlong,\
	uint32_t: u32_ToFlong,\
	int64_t: i64_ToFlong,\
	uint64_t: u64_ToFlong,\
	float: f32_ToFlong,\
	double: f64_ToFlong,\
	long double: flong_ToFlong,\
	default: invalid_ToFlong\
)((x))

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

//lossless
static inline float i8_ToF32(int8_t i){ return (float)i; }
static inline float u8_ToF32(uint8_t i){ return (float)i; }
static inline float i16_ToF32(int16_t i){ return (float)i; }
static inline float u16_ToF32(uint16_t i){ return (float)i; }
//potentially lossy
static inline float i64_ToF32(int64_t i){//this might be correct I think (maybe)
	//represents the limit value of exactness for integers
	//to convert to floats with no loss.
	//past this point exist possible holes
	//that cannot represent values exactly
    if(i>(int64_t)powf(2,FLT_MANT_DIG) || i<(int64_t)-powf(2,FLT_MANT_DIG)){
        //fputs("Could not convert i(>16) to f32.\n",stderr);
        //exit(-1);
        return 0;
    }
    return (float)i;
}
static inline float u64_ToF32(uint64_t i){
    if(i>(uint64_t)powf(2,FLT_MANT_DIG)){
        //fputs("Could not convert u(>16) to f32.\n",stderr);
        //exit(-1);
        return 0;
    }
    return (float)i;
}
static inline float i32_ToF32(int32_t i){ return i64_ToF32(i); }
static inline float u32_ToF32(uint32_t i){ return u64_ToF32(i); }
static inline float f32_ToF32(float f){ return f; }
//from mch:
//https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
//(a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon); ge
//(b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon); lt
static inline float flong_ToF32(long double f){
	f = fabsl(f);
	long double max = FLT_MAX;
	long double min = FLT_MIN;
	_Bool result1 = (f-max)>(
					((fabsl(f)<fabsl(max)) ? fabsl(max) : fabsl(f))
							* LDBL_EPSILON),//bigger than max lossless int value
		  result2 = (min-f)>(
					((fabsl(f)<fabsl(min)) ? fabsl(min) : fabsl(f))
							* LDBL_EPSILON);//smaller than min lossless int value
	if(result1 || result2){
        //fputs("Could not convert f(>32) to f32.\n",stderr);
        //exit(-1);
        return 0;
    }
    return (float)f;
}
static inline float f64_ToF32(double f){ return flong_ToF32(f); }
static inline float invalid_ToF32(){ return (float)0.0; }
#define ToF32(x)\
_Generic((x),\
	int8_t: i8_ToF32,\
	uint8_t: u8_ToF32,\
	int16_t: i16_ToF32,\
	uint16_t: u16_ToF32,\
	int32_t: i32_ToF32,\
	uint32_t: u32_ToF32,\
	int64_t: i64_ToF32,\
	uint64_t: u64_ToF32,\
	float: f32_ToF32,\
	double: f64_ToF32,\
	long double: flong_ToF32,\
	default: invalid_ToF32\
)((x))

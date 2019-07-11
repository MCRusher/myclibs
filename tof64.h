#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

//lossless
static inline double i8_ToF64(int8_t i){ return (double)i; }
static inline double u8_ToF64(uint8_t i){ return (double)i; }
static inline double i16_ToF64(int16_t i){ return (double)i; }
static inline double u16_ToF64(uint16_t i){ return (double)i; }
static inline double i32_ToF64(int32_t i){ return (double)i; }
static inline double u32_ToF64(uint32_t i){ return (double)i; }
static inline double f32_ToF64(float f){ return (double)f; }
static inline double f64_ToF64(double f){ return f; }
//potentially lossy
static inline double i64_ToF64(int64_t i){//this might be correct I think (maybe)
	//represents the limit value of exactness for integers
	//to convert to floats with no loss.
	//past this point exist possible holes
	//that cannot represent values exactly
    if(i>(int64_t)pow(2,DBL_MANT_DIG) || i<(int64_t)-pow(2,DBL_MANT_DIG)){
        fputs("Could not convert i64 to f64.\n",stderr);
        exit(-1);
    }
    return (double)i;
}
static inline double u64_ToF64(uint64_t i){
    if(i>(uint64_t)pow(2,DBL_MANT_DIG)){
        fputs("Could not convert u64 to f64.\n",stderr);
        exit(-1);
    }
    return (double)i;
}
//from mch:
//https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
//(a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon); ge
//(b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon); lt
static inline double flong_ToF64(long double f){
	f = fabsl(f);
	long double max = DBL_MAX;
	long double min = DBL_MIN;
	_Bool result1 = (f-max)>(
					((fabsl(f)<fabsl(max)) ? fabsl(max) : fabsl(f))
							* LDBL_EPSILON),//bigger than max lossless int value
		  result2 = (min-f)>(
					((fabsl(f)<fabsl(min)) ? fabsl(min) : fabsl(f))
							* LDBL_EPSILON);//smaller than min lossless int value
	if(result1 || result2){
        fputs("Could not convert flong to f64.\n",stderr);
        exit(-1);
    }
    return (double)f;
}
static inline double invalid_ToF64(){ return (double)0.0; }
#define ToF64(x)\
_Generic((x),\
	int8_t: i8_ToF64,\
	uint8_t: u8_ToF64,\
	int16_t: i16_ToF64,\
	uint16_t: u16_ToF64,\
	int32_t: i32_ToF64,\
	uint32_t: u32_ToF64,\
	int64_t: i64_ToF64,\
	uint64_t: u64_ToF64,\
	float: f32_ToF64,\
	double: f64_ToF64,\
	long double: flong_ToF64,\
	default: invalid_ToF64\
)((x))

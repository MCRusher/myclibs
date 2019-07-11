#pragma once

#include "option.h"

#include "random.h"

#include "sleep_for.h"

#include "threading.h"

#include "tralloc.h"

//string impl
#define TRALLOC_REPLACE
#include "stringing.h"

#define TRALLOC_REPLACE
#include "tostring.h"

//generic printing impl
#define TRALLOC_REPLACE
#include "printing.h"

//rebinds all base generic conversions to include conversions from string
#include "stringto.h"

#include "to8.h"
#undef ToI8
#undef ToU8
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
    string: string_ToI8,\
    default: invalid_ToI8\
)((x))
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
    string: string_ToU8,\
    default: invalid_ToU8\
)((x))

#include "to16.h"
#undef ToI16
#undef ToU16
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
    string: string_ToI16,\
    default: invalid_ToI16\
)((x))
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
    string: string_ToU16,\
    default: invalid_ToU16\
)((x))

#include "to32.h"
#undef ToI32
#undef ToU32
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
    string: string_ToI32,\
    default: invalid_ToI32\
)((x))
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
    string: string_ToU32,\
    default: invalid_ToU32\
)((x))

#include "to64.h"
#undef ToI64
#undef ToU64
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
	string: string_ToI64,\
	default: invalid_ToI64\
)((x))
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
    string: string_ToU64,\
    default: invalid_ToU64\
)((x))

#include "tof32.h"
#undef ToF32
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
	string: string_ToF32,\
	default: invalid_ToF32\
)((x))

#include "tof64.h"
#undef ToF64
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
	string: string_ToF64,\
	default: invalid_ToF64\
)((x))

#include "toflong.h"
#undef ToFlong
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
	string: string_ToFlong,\
	default: invalid_ToFlong\
)((x))


#include "tochar.h"
#undef ToChar
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
	string: string_ToChar,\
	default: invalid_ToChar\
)((x))

#include "mydef.h"

/* Stripped down manual setup
	#define __USE_MINGW_ANSI_STDIO 1
	#include "mydef.h"
	#define TRALLOC_REPLACE
	#include "tostring.h" //tr version of string conversions
	#define TRALLOC_REPLACE
	#include "printing.h" //tr "tostring" compatible version of printing functions
	#define TRALLOC_REPLACE //tr version of string functions
	#include "stringing.h"
	#include "tochar.h"
	#include "stringto.h"
*/

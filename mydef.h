#pragma once
#include <stdint.h>
#include <inttypes.h>
#include <float.h>
#include <math.h>

//FLT_ROUNDS should not be a constant, as it can be changed by calls from <fenv.h>
//mingw-w64 8.1.0 does make it a constant for some stupid reason even they apparently don't understand:
//* ??? This is supposed to change with calls to fesetround in <fenv.h>.  */
//	#undef FLT_ROUNDS
//	#define FLT_ROUNDS 1

typedef int8_t i8;
#define i8_MAX ((i8)INT8_MAX)
#define i8_MIN ((i8)INT8_MIN)
#define i8_PRI PRId8
#define i8_SCN SCNd8
typedef uint8_t u8;
#define u8_MAX ((u8)UINT8_MAX)
#define u8_MIN ((u8)0)
#define u8_PRI PRIu8
#define u8_SCN SCNu8

typedef int16_t i16;
#define i16_MAX ((i16)INT16_MAX)
#define i16_MIN ((i16)INT16_MIN)
#define i16_PRI PRId16
#define i16_SCN SCNd16
typedef uint16_t u16;
#define u16_MAX ((u16)UINT16_MAX)
#define u16_MIN ((u16)0)
#define u16_PRI PRIu16
#define u16_SCN SCNu16

typedef int32_t i32;
#define i32_MAX ((i32)INT32_MAX)
#define i32_MIN ((i32)INT32_MIN)
#define i32_PRI PRId32
#define i32_SCN SCNd32
typedef uint32_t u32;
#define u32_MAX ((u32)UINT32_MAX)
#define u32_MIN ((u32)0)
#define u32_PRI PRIu32
#define u32_SCN SCNu32

typedef int64_t i64;
#define i64_MAX ((i64)INT64_MAX)
#define i64_MIN ((i64)INT64_MIN)
#define i64_PRI PRId64
#define i64_SCN SCNd64
typedef uint64_t u64;
#define u64_MAX ((u64)UINT64_MAX)
#define u64_MIN ((u64)0)
#define u64_PRI PRIu64
#define u64_SCN SCNu64

typedef float f32;
#define f32_MAX ((f32)FLT_MAX)
#define f32_NMAX ((f32)-FLT_MAX)
#define f32_MIN ((f32)FLT_MIN)
#define f32_NMIN ((f32)-FLT_MIN)
#define f32_PRI "f"
#define f32_SCN "f"
#define f32_EXPMAX FLT_MAX_EXP
#define f32_EXPMIN FLT_MIN_EXP
#define f32_EXP10MAX FLT_MAX_10_EXP
#define f32_EXP10MIN FLT_MIN_10_EXP
#define f32_EPSILON ((f32)FLT_EPSILON)
#define f32_INF ((f32)(1/0.0))
#define f32_NINF ((f32)(-1/0.0))
#define f32_NAN_DIV0 ((f32)(0.0/0.0))
#define f32_GetRoundingBehavior() fX_GetRoundingBehavior()
#define f32_EvaluationMethod() fX_EvaluationMethod()
#define f32_GetDecimalDigits() fX_GetDecimalDigits()
#define f32_GetRadix() fX_GetRadix()
#define f32_GetNaN(f) isnan(f)
#define f32_GetInf(f) isinf(f)

typedef double f64;
#define f64_MAX ((f64)DBL_MAX)
#define f64_NMAX ((f64)-DBL_MAX)
#define f64_MIN ((f64)DBL_MIN)
#define f64_NMIN ((f64)-DBL_MIN)
#define f64_PRI "lf"
#define f64_SCN "lf"
#define f64_EXPMAX DBL_MAX_EXP
#define f64_EXPMIN DBL_MIN_EXP
#define f64_EXP10MAX DBL_MAX_10_EXP
#define f64_EXP10MIN DBL_MIN_10_EXP
#define f64_EPSILON ((f64)DBL_EPSILON)
#define f64_INF ((f64)(1/0.0))
#define f64_NINF ((f64)(-1/0.0))
#define f64_NAN_DIV0 ((f64)(0.0/0.0))
#define f64_GetRoundingBehavior() fX_GetRoundingBehavior()
#define f64_EvaluationMethod() fX_EvaluationMethod()
#define f64_GetDecimalDigits() fX_GetDecimalDigits()
#define f64_GetRadix() fX_GetRadix()
#define f64_GetNaN(f) isnan(f)
#define f64_GetInf(f) isinf(f)

typedef long double flong;
#define flong_MAX ((flong)LDBL_MAX)
#define flong_NMAX ((flong)-LDBL_MAX)
#define flong_MIN ((flong)LDBL_MIN)
#define flong_NMIN ((flong)-LDBL_MIN)
#define flong_PRI "lf"
#define flong_SCN "lf"
#define flong_EXPMAX LDBL_MAX_EXP
#define flong_EXPMIN LDBL_MIN_EXP
#define flong_EXP10MAX LDBL_MAX_10_EXP
#define flong_EXP10MIN LDBL_MIN_10_EXP
#define flong_EPSILON ((flong)LDBL_EPSILON)
#define flong_INF ((flong)(1/0.0))
#define flong_NINF ((flong)(-1/0.0))
#define flong_NAN_DIV0 ((flong)(0.0/0.0))
#define flong_GetRoundingBehavior() fX_GetRoundingBehavior()
#define flong_EvaluationMethod() fX_EvaluationMethod()
#define flong_GetDecimalDigits() fX_GetDecimalDigits()
#define flong_GetRadix() fX_GetRadix()
#define flong_GetNaN(f) isnan(f)
#define flong_GetInf(f) isinf(f)

static inline int fX_GetRoundingBehavior(void){ return FLT_ROUNDS; }
static inline int fX_GetEvaluationMethod(void){ return FLT_EVAL_METHOD; }
static inline int fX_GetDecimalDigits(void){ return DECIMAL_DIG; }
static inline int fX_GetRadix(void){ return FLT_RADIX; }

typedef void* addr;
#define addr_MAX ((addr)UINTPTR_MAX)
#define addr_MIN ((addr)0)
#define addr_NULL ((addr)0)
#define addr_PRI "p"
#define addr_SCN "p"

typedef intptr_t iaddr;
#define iaddr_MAX ((iaddr)INTPTR_MAX)
#define iaddr_MIN ((iaddr)INTPTR_MIN)
#define iaddr_PRI PRIdPTR
#define iaddr_SCN SCNdPTR

typedef uintptr_t uaddr;
#define uaddr_MAX ((uaddr)UINTPTR_MAX)
#define uaddr_MIN ((uaddr)0)
#define uaddr_PRI PRIuPTR
#define uaddr_SCN SCNuPTR

typedef typeof(sizeof 1) usize;
#define usize_MAX ((uaddr)SIZE_MAX)
#define usize_MIN ((uaddr)0)
#define usize_PRI "zu"
#define usize_SCN "zu"

typedef _Bool bool;
#define true ((bool)1)
#define false ((bool)0)
#define bool_TRUE ((bool)1)
#define bool_FALSE ((bool)0)

typedef struct{i8 _;} tribool;
#define tribool_TRUE (tribool){true}
#define tribool_FALSE (tribool){false}
#define tribool_MAYBE (tribool){-1}

typedef uint8_t byte;
#define byte_MAX ((u8)UINT8_MAX)
#define byte_MIN ((u8)0)
#define byte_PRI PRIu8
#define byte_SCN SCNu8

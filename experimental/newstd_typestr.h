#ifndef NEWSTD_TYPESTR
#include "newstd_panic.h"
#define typestr_compatible(T)\
((_Bool)_Generic((T){0},\
    char: 1,\
    int8_t: 1,\
    uint8_t: 1,\
    int16_t: 1,\
    uint16_t: 1,\
    int32_t: 1,\
    uint32_t: 1,\
    int64_t: 1,\
    uint64_t: 1,\
    float: 1,\
    double: 1,\
    long double: 1,\
    default: 0\
))

#define typestr(T)\
_Generic((T){0},\
    char: "char",\
    int8_t: "int8_t",\
    uint8_t: "uint8_t",\
    int16_t: "int16_t",\
    uint16_t: "uint16_t",\
    int32_t: "int32_t",\
    uint32_t: "uint32_t",\
    int64_t: "int64_t",\
    uint64_t: "uint64_t",\
    float: "float",\
    double: "double",\
    long double: "long double",\
    default: static_panic_if("T is incompatible with typestr",!typestr_compatible(T))\
)
#endif //NEWSTD_TYPESTR

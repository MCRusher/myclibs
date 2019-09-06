#pragma once
#include <stdio.h>
#include <stdlib.h>

//explicitly handles TRALLOC_REPLACE due to macro expansion issues
//#include "tralloc_override_start.h"

//NOTE: typeof((0,X)) forces X to decay from an array to a pointer
//which is necessary for type matching against _Generic, and for assigning
//to a local variable to prevent unintended side effects (arrays cannot be assigned).

#ifndef tool_SUPPRESS_NO_EFFECT_ZERO
#define tool_SUPPRESS_NO_EFFECT_ZERO
static inline int tool_SuppressNoEffect_ZERO(void){ return 0; }
#endif // tool_SUPPRESS_NO_EFFECT_ZERO

#define array(X...)\
struct{\
	X* data;\
	typeof(sizeof 1) count;\
}

#define array_InitNull() {.data = ((void*)0), .count = 0}

///Initializes a new array with an existing carray into an array without a deep copy
///Array should not be deleted, but freed in the originally intended way (not at all if on stack).
#define array_Init(ptr,size) {.data = (ptr), .count = (size)}

///returns a "reference" to the array's data at index
#define array_At(arr_ptr,index) ((arr_ptr)->data[(index)])

///Checks bounds of access, an index with side effects will have unintended results.
#define array_CheckedAt(arr_ptr,index) ((arr_ptr)->count > (index) ? ((arr_ptr)->data) : (fputs("Array accessed out of bounds.\n",stderr),exit(-1),((arr_ptr)->data)))[(index)]

#ifndef TRALLOC_REPLACE
///Resizes array's data to be at least size long
#define array_Resize(arr_ptr,size)\
do{\
	if((arr_ptr)->count < size){\
		void* tmp = realloc((arr_ptr)->data,((typeof(sizeof 1))(size)));\
		if(!tmp){\
			fputs("Could not resize array.\n",stderr);\
			exit(-1);\
		}\
		(arr_ptr)->data = tmp;\
		(arr_ptr)->count = size;\
	}\
}while(0)

///Sets array to the contents of ptr up to size, resizing if (size > array.count).
///It is safe to pass a ptr and size with side effects
#define array_SetWith(arr_ptr,ptr,size)\
do{\
	typeof((size)) _size = (size);\
	typeof((tool_SuppressNoEffect_ZERO(),(ptr))) _ptr = (ptr);\
	array_Resize((arr_ptr),(_size));\
    memcpy((arr_ptr)->data,_ptr,((_size)*sizeof((arr_ptr)->data[0])));\
}while(0)

///Frees memory associated with array and sets it to empty
#define array_Delete(arr_ptr)\
do{\
    free((arr_ptr)->data);\
    (arr_ptr)->data = ((void*)0);\
    (arr_ptr)->count = 0;\
}while(0)
#else //TRALLOC_REPLACE is defined
///Resizes array's data to be at least size long
#define array_Resize(arr_ptr,size)\
do{\
	if((arr_ptr)->count < size){\
		trrealloc(&(arr_ptr)->data,((typeof(sizeof 1))(size)));\
		(arr_ptr)->count = size;\
	}\
}while(0)

///Sets array to the contents of ptr up to size, resizing if (size > array.count).
///It is safe to pass a ptr and size with side effects
#define array_SetWith(arr_ptr,ptr,size)\
do{\
	typeof((size)) _size = (size);\
	typeof((tool_SuppressNoEffect_ZERO(),(ptr))) _ptr = (ptr);\
	array_Resize((arr_ptr),(_size));\
    memcpy((arr_ptr)->data,_ptr,((_size)*sizeof((arr_ptr)->data[0])));\
}while(0)

///Frees memory associated with array and sets it to empty
#define array_Delete(arr_ptr)\
do{\
    trdealloc(&(arr_ptr)->data);\
    (arr_ptr)->count = 0;\
}while(0)
#endif // TRALLOC_REPLACE

#undef TRALLOC_REPLACE
//#include "tralloc_override_end.h"

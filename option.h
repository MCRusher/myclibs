#pragma once
#include <stdio.h>
#include <stdlib.h>

#define option(x...) struct{ _Bool none; x value;} __attribute__ ((packed))
#define option_INIT {1}
#define option_None(opt_ptr) do{(opt_ptr)->none = 1;}while(0)
#define option_Set(opt_ptr,x...) do{(opt_ptr)->value = x; (opt_ptr)->none = 0;}while(0)
#define option_IsNone(opt) ((opt).none)
static inline void option_UnboxError(void){
	fputs("Attempt to unwrap option type with none=true.\n",stderr);
	exit(-1);
}
#define option_Unbox(opt) (((opt).none) ? (option_UnboxError(),(opt).value) : (opt).value)

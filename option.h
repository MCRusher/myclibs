#pragma once
#include <stdio.h>
#include <stdlib.h>

#define option(x...) struct{ _Bool none; x value;}
#define option_InitNone() {.none = 1}
#define option_Init(val...) {.value = val; .none = 0}
#define option_SetNone(opt) do{(opt).none = 1;}while(0)
#define option_Set(opt,val...) do{(opt).value = val; (opt).none = 0;}while(0)
#define option_IsNone(opt) ((_Bool)(opt).none)
static inline void option_UnboxError(void){
	fputs("Attempt to unwrap option type with none=true.\n",stderr);
	exit(-1);
}
#define option_Unbox(opt) (((opt).none) ? (option_UnboxError(),(opt).value) : (opt).value)

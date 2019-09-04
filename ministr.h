#pragma once
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#ifndef MINISTR_STRUCT
#define MINISTR_STRUCT
#include <stdint.h>
///A string type with a maximum size of 255 characters
///Uses no allocation
typedef struct ministr {
	char data[255];
	uint8_t count;
} ministr;
#define ministr_GetEnd(mstr) ((mstr).data+(mstr).count)
#endif // MINISTR_STRUCT

static inline ministr ministr_Create(char const * const s){
	typeof(sizeof 1) s_len = strlen(s);
	if(s_len>255)
		s_len = 255;
	ministr ms;
	memcpy(ms.data,s,s_len);
	ms.count = s_len;
	return ms;
}

///eheheheheh ehhhhhhhhhhhhhhhhhhhhhhhhhhhhh
static inline ministr ministr_Combine(ministr const * const m1, ministr const * const m2){
    ministr ms;
    memcpy(ms.data,m1->data,m1->count);
    memcpy(&ms.data[m1->count],m2->data,(m1->count + m2->count > 255) ? 255-m1->count : m2->count);
    ms.count = m1->count + m2->count;
    return ms;
}

static inline ministr ministr_Copy(ministr const * const m){
	ministr ms;
	memcpy(ms.data,m->data,m->count);
	ms.count = m->count;
	return ms;
}

static inline ministr ministr_NullTerminate(ministr const * const m){
	ministr ms;
	ms.count = m->count == 255 ? 254 : m->count;
	memcpy(ms.data,m->data,ms.count);
	ms.data[ms.count] = '\0';
	return ms;
}

static inline ministr ministr_Empty(void){
	return (ministr){.count = 0};
}

///Sets ministr's count to zero,
///not required since no allocation was performed
static inline void ministr_Delete(ministr* mp){
    mp->count = 0;
}

static inline uint8_t ministr_Find(ministr const * const ms, char const * const s){
    for(uint8_t index = 0; index != ms->count; index++){
		for(char const * c = s; *c != '\0'; c++){
			if(ms->data[index] == *c)
				return index;
		}
    }
    return ms->count;
}

static inline ministr ministr_ReadLn(void){
	ministr ms;
	uint8_t i = 0;
    for(int c; i < 255 && (c=fgetc(stdin)) !='\n' && c !=-1; i++){
        ms.data[i] = c;
    }
    ms.count = i;
    return ms;
}

static inline ministr ministr_Trim(ministr const * const m){
	uint8_t b = 0, e = 0;
	for(char const* c = &m->data[0]; isspace(*c); c++)
		++b;
	for(char const* c = &m->data[m->count-1]; isspace(*c); c--)
		--e;
	ministr ms;
	memcpy(ms.data,&m->data[b], e-b);
	ms.count = e-b;
	return ms;
}

static inline _Bool ministr_IsZero(ministr const * const ms){
	if(ms->count==1 && ms->data[0]=='0')
		return 1;
	else if(ms->count==2 && ms->data[1] && (ms->data[0] == '+' || ms->data[0]=='-'))
		return 1;
	else return 0;
}

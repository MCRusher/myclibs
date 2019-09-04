#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifndef STRING_STRUCT
#define STRING_STRUCT
typedef struct string{
	char* data;
	typeof(sizeof 1) count;
} string;
#define string_GetEnd(str) ((str).data+(str).count)
#endif // STRING_STRUCT

#ifndef EXCEPTION_STRUCT
#define EXCEPTION_STRUCT
typedef struct exception {
	string what;
	int32_t type;
} exception;
#endif // EXCEPTION_STRUCT

static inline exception* exception_Create_string(string const mess, int32_t type){
	exception* ex = malloc(sizeof(exception));
	char* tmp = malloc(mess.count);
	if(!ex || !tmp){
		fputs("Could not create exception.\n",stderr);
		exit(-1);
	}
	ex->what = (string){.data = memcpy(tmp,mess.data,mess.count), .count = mess.count};
	ex->type = type;
	//exception ex = (exception){.what = {.data = memcpy(tmp,mess.data,mess.count), .count = mess.count}, .type = type};
    return ex;
}
static inline exception* exception_Create_cstring(char* mess, int32_t type){
	string s = (string){.data = mess, .count = strlen(mess)};
	return exception_Create_string(s,type);
}

static inline exception* exception_Create_invalid(){
	fputs("Could not create exception from invalid message type.\n",stderr);
	exit(-1);
	return ((void*)0);
}

///Creates an exception from a string or cstring
///Note that exceptions from -1 to -100 are reserved
#define exception_Create(mess,type)\
_Generic((mess),\
	char*: exception_Create_cstring,\
	string: exception_Create_string,\
	default: exception_Create_invalid\
)((mess),(type))

///Frees data associated with the exception
static inline void exception_Delete(exception** ex){
    free((*ex)->what.data);
    free(*ex);
    *ex = ((void*)0);
    //ex->what.data = ((void*)0);
    //ex->what.count = 0;
    //ex->type = 0;
}

struct ExLink_ {
	exception* ex;
	struct ExLink_ *next;
};

struct{
    struct ExLink_ *first;
    typeof(sizeof 1) count;
} ExList_ = {.first = ((void*)0),.count = 0};

static inline _Bool ExList__pop(void){
    if(ExList_.first!=((void*)0)){
        struct ExLink_* old = ExList_.first;
        ExList_.first = ExList_.first->next;
        free(old);
        --ExList_.count;
        return 1;
    }
    return 0;
}

static inline void ExList__push(exception* ex){
	struct ExLink_* prev = ExList_.first;
	ExList_.first = malloc(sizeof(struct ExLink_));
	if(!ExList_.first){
        fputs("Could not throw exception.\n",stderr);
        exit(-1);
	}
	ExList_.first->ex = ex;
	ExList_.first->next = prev;
	++ExList_.count;
}

static inline exception* ExList__find(int32_t type){
	if(ExList_.first!=((void*)0)){
		struct ExLink_* curr = ExList_.first;
		for(typeof(sizeof 1) i = 0; i < ExList_.count; i++){
            if(curr->ex->type==type){
				return curr->ex;
            }
            curr = curr->next;
		}
	}
	return ((void*)0);
}

static inline exception* ExList__remove(int32_t type){
	if(ExList_.first!=((void*)0)){
		struct ExLink_* prev = ((void*)0);
		struct ExLink_* curr = ExList_.first;
		for(typeof(sizeof 1) i = 0; i < ExList_.count; i++){
            if(curr->ex->type==type){
				exception* ep = curr->ex;
				if(prev!=((void*)0))
					prev->next = curr->next;
				else ExList_.first = curr->next;
				free(curr);
				--ExList_.count;
				return ep;
            }
            prev = curr;
            curr = curr->next;
		}
	}
	return ((void*)0);
}

#ifndef EXCEPTIONS_NOTHREADSAFE
#include "mutexing.h"
#include <errno.h>
#define EEXCEPTIONS (6942)
mutex_t exceptions_mutex;
NEWSTD_CONSTRUCTOR static inline void exceptions_start(void){
    _Bool bvar = 0;
    mutex_create_constructor(exceptions_mutex,bvar);
    if(!bvar){
		fputs("Could not start threadsafe exceptions.\n",stderr);
		errno = EEXCEPTIONS;
    }
}
#endif // EXCEPTIONS_NOTHREADSAFE

static inline void Throw(exception* ex){
#ifndef EXCEPTIONS_NOTHREADSAFE
	mutex_lock(&exceptions_mutex);
#endif // EXCEPTIONS_NOTHREADSAFE
    ExList__push(ex);
#ifndef EXCEPTIONS_NOTHREADSAFE
	mutex_unlock(&exceptions_mutex);
#endif // EXCEPTIONS_NOTHREADSAFE
}

static inline _Bool Catch(exception** ex, int32_t type){
#ifndef EXCEPTIONS_NOTHREADSAFE
	mutex_lock(&exceptions_mutex);
#endif // EXCEPTIONS_NOTHREADSAFE
	exception* ep = ExList__remove(type);
	_Bool retval = 0;
	if(ep){
		*ex = ep;
		retval = 1;
	}
#ifndef EXCEPTIONS_NOTHREADSAFE
	mutex_unlock(&exceptions_mutex);
#endif // EXCEPTIONS_NOTHREADSAFE
	return retval;
}

NEWSTD_DESTRUCTOR static inline void exceptions__cleanup(void){
#ifndef EXCEPTIONS_NOTHREADSAFE
	mutex_lock(&exceptions_mutex);
#endif // EXCEPTIONS_NOTHREADSAFE
    if(ExList_.count!=0){
        #ifndef NDEBUG
        fprintf(stderr,"\nExceptions remaining: %"PRIuMAX"\nExceptions:\n",(uintmax_t)ExList_.count);
        #endif //NDEBUG
        while(ExList_.count!=0){
			exception* ex = ExList_.first->ex;
			#ifndef NDEBUG
			fputs("\t\"",stderr);
			for(typeof(sizeof 1) i = 0; i < ex->what.count; i++)
				fputc(ex->what.data[i],stderr);
			fprintf(stderr,"\" with type %d\n",ex->type);
			#endif //NDEBUG
            exception_Delete(&ex);
			ExList__pop();
        }
    }
#ifndef EXCEPTIONS_NOTHREADSAFE
	mutex_unlock(&exceptions_mutex);
	_Bool bvar = 0;
	mutex_delete_destructor(exceptions_mutex,bvar);
	if(!bvar){
		fputs("Could not end threadsafe exceptions.\n",stderr);
		errno = EEXCEPTIONS;
	}
#endif // EXCEPTIONS_NOTHREADSAFE
}

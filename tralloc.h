#ifndef TRALLOC_H
#define TRALLOC_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#ifndef COUNT_VECTOR_INIT_MAX
#define COUNT_VECTOR_INIT_MAX 20
#endif // COUNT_VECTOR_INIT_MAX

#ifndef TRALLOC_NOTHREADSAFE
#include <errno.h>
#include "mutexing.h"
#define ETRALLOC (4269)
mutex_t tralloc_mutex;
NEWSTD_CONSTRUCTOR static inline void tralloc_start(void){
	_Bool bvar = 0;
	mutex_create_constructor(tralloc_mutex,bvar);
	if(!bvar){
		fputs("Could not start threadsafe tralloc.\n",stderr);
		errno = ETRALLOC;
	}
}
#endif // TRALLOC_NOTHREADSAFE

struct AddrLink_ {
	void* addr;
	typeof(sizeof 1) count;
	struct AddrLink_ *next;
};

struct{
    struct AddrLink_ *first;
    typeof(sizeof 1) count;
} AddrList_ = {.first = ((void*)0),.count = 0};

static inline void AddrList__push(void* addr, typeof(sizeof 1) count){
	struct AddrLink_* prev = AddrList_.first;
	AddrList_.first = malloc(sizeof(struct AddrLink_));
	if(!AddrList_.first){
        fputs("Could not push address.\n",stderr);
        exit(-1);
	}
	AddrList_.first->addr = addr;
	AddrList_.first->count = count;
	AddrList_.first->next = prev;
	++AddrList_.count;
}

static inline _Bool AddrList__pop(void){
    if(AddrList_.first!=((void*)0)){
        struct AddrLink_* old = AddrList_.first;
        AddrList_.first = AddrList_.first->next;
        free(old);
        --AddrList_.count;
        return 1;
    }
    return 0;
}

static inline _Bool AddrList__remove(void* addr){
	if(AddrList_.first!=((void*)0)){
		struct AddrLink_* prev = ((void*)0);
		struct AddrLink_* curr = AddrList_.first;
		for(typeof(sizeof 1) i = 0; i < AddrList_.count; i++){
            if(curr->addr==addr){
				if(prev!=((void*)0))
					prev->next = curr->next;
				else AddrList_.first = curr->next;
				free(curr);
				--AddrList_.count;
				return 1;
            }
            prev = curr;
            curr = curr->next;
		}
	}
	return 0;
}

static inline _Bool AddrList__set(void* addr, typeof(sizeof 1) count){
	if(AddrList_.first!=((void*)0)){
		struct AddrLink_* curr = AddrList_.first;
		for(typeof(sizeof 1) i = 0; i < AddrList_.count; i++){
            if(curr->addr==addr){
				curr->count = count;
				return 1;
            }
		}
	}
	return 0;
}

//makes safety checks redundant
static inline void* tralloc(typeof(sizeof 1) count){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
	void* addr = calloc(1,count);
	if(!addr){
		fputs("Could not allocate.\n",stderr);
		exit(-1);
	}
	AddrList__push(addr,count);
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
    return addr;
}

//requires a **, null inits
//makes safety checks redundant
static inline void trdealloc(void** addr){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
	if(!AddrList__remove(*addr)){
		fputs("Could not dealllocate.\n",stderr);
		exit(-1);
	}
	free(*addr);
	*addr = ((void*)0);
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
}
#define trdealloc(addr) trdealloc((void**)(addr))

//makes safety checks redundant
//requires a **, addr is automatically set to new address,
static inline void trrealloc(void** addr, typeof(sizeof 1) count){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
    void* tmp = realloc(*addr,count);
    if(!tmp && count!=0){
        fputs("Could not reallocate.\n",stderr);
        exit(-1);
    }
    if(tmp!=*addr){
        AddrList__remove(*addr);
        AddrList__push(tmp,count);
        *addr = tmp;
    }else{
        AddrList__set(*addr,count);
    }
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
}
#define trrealloc(addr,count) trrealloc((void**)(addr),(count))

static inline void trdealloc_all(void){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
    while(AddrList_.count!=0){
		free(AddrList_.first->addr);
		AddrList__pop();
	}
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
}

//Automatically runs at the end of the program (if gcc destructors are supported)
//and frees all remaining memory.
//when not NDEBUG, gives visible warnings of automatically handled memory leaks.
NEWSTD_DESTRUCTOR static inline void trcheckalloc(void){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
    if(AddrList_.count!=0){
        #ifndef NDEBUG
        fprintf(stderr,"\nAllocated blocks remaining: %"PRIuMAX"\nAddresses:\n",(uintmax_t)AddrList_.count);
        #endif //NDEBUG
        while(AddrList_.count!=0){
			#ifndef NDEBUG
            fprintf(stderr,"\t%p of size %"PRIuMAX"\n",AddrList_.first->addr,(uintmax_t)AddrList_.first->count);
			#endif //NDEBUG
            free(AddrList_.first->addr);
			AddrList__pop();
        }
    }
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
	_Bool bvar = 0;
	mutex_delete_destructor(tralloc_mutex,bvar);
	if(!bvar){
		fputs("Could not end threadsafe tralloc.\n",stderr);
		errno = ETRALLOC;
	}
#endif // TRALLOC_NOTHREADSAFE
}

struct{
	typeof(sizeof 1)* data;
	typeof(sizeof 1) count;
	typeof(sizeof 1) max;
} CountVector_ = {.data = ((void*)0), .max = 0, .count = 0};

static inline void CountVector__push(typeof(sizeof 1) count){
    if(CountVector_.max == CountVector_.count){
    	if(CountVector_.max==0) CountVector_.max = COUNT_VECTOR_INIT_MAX;
		else CountVector_.max *= 2;
		CountVector_.data = realloc(CountVector_.data,sizeof(typeof(sizeof 1))*CountVector_.max);
		if(!CountVector_.data){
			fputs("Could not set scope.\n",stderr);
			exit(-1);
		}
    }
    CountVector_.data[CountVector_.count++] = count;
}

static inline void CountVector__pop(void){
    if(CountVector_.count!=0){
		--CountVector_.count;
		//only deallocated when count hits zero
		if(CountVector_.count==0 && CountVector_.max!=0){
            CountVector_.max = 0;
            free(CountVector_.data);
            CountVector_.data = ((void*)0);
		}
    }
}

static inline void trset_scope(void){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
    CountVector__push(AddrList_.count);
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
}

static inline void trdealloc_scope(void){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
    if(CountVector_.count!=0){
		typeof(sizeof 1) count = CountVector_.data[CountVector_.count-1];
		//should compare correctly even if count=0
		while(AddrList_.count!=count){
            AddrList__pop();
		}
		CountVector__pop();
    }
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
}

//Does no allocation, frees a previously allocated buffer at the end of the current scope.
static inline void tradd_to_scope(void* addr, typeof(sizeof 1) count){
#ifndef TRALLOC_NOTHREADSAFE
	mutex_lock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
    AddrList__push(addr,count);
    if(CountVector_.count>0)
		++CountVector_.data[CountVector_.count-1];
	else{
		fputs("Attempt to add to scope when no scopes exist.\n",stderr);
		exit(-1);
	}
#ifndef TRALLOC_NOTHREADSAFE
	mutex_unlock(&tralloc_mutex);
#endif // TRALLOC_NOTHREADSAFE
}

//pretty sure this won't always work without getting the position of the address
//in AddrList_ and then comparing with counts in CountVector_ to know which scope
//to subtract from since managed scopes can be nested
__attribute__ ((deprecated))
static inline void* trkeep(void* addr){
	//prevents trdealloc_scope from overfreeing
	//if addr is in addrlist & count is being used
	if(AddrList__remove(addr) && CountVector_.count!=0){
        --CountVector_.data[CountVector_.count-1];
	}
	return addr;
}

//C compatible wrappers
static inline void* tr_malloc(typeof(sizeof 1) count){
	return tralloc(count);
}
static inline void* tr_calloc(typeof(sizeof 1) size, typeof(sizeof 1) count){
	return tralloc(size*count);
}
static inline void* tr_realloc(void* old, typeof(sizeof 1) count){
    trrealloc(&old,count);
    return old;
}
static inline void tr_free(void* addr){
	trdealloc(&addr);
}

#define MANAGE {trset_scope();
#define UNMANAGE trdealloc_scope();}

#endif // TRALLOC_H
//#ifdef TRALLOC_REPLACE
//#define malloc(x) tr_malloc((x))
//#define calloc(x,y) tr_calloc((x),(y))
//#define realloc(x,size) tr_realloc((x),(size))
//#define free(x) tr_free((x))
//#undef TRALLOC_REPLACE
//#endif // TRALLOC_REPLACE

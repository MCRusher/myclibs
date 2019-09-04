#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef MUTEXING_USE_EXCEPTIONS
#define EX_MUTEX (-1)
#include "exceptions.h"
#endif // MUTEXING_USE_EXCEPTIONS

#ifdef _WIN32
#include <windows.h>
typedef HANDLE mutex_t;
static inline mutex_t mutex_create(void){
    mutex_t mtx = CreateMutexA(((void*)0),0,((void*)0));
	if(mtx==0){
		fputs("Could not create mutex.\n",stderr);
        exit(-1);
	}
	return mtx;
}
static inline void mutex_lock(mutex_t* mtx){
    if(WaitForSingleObject(*mtx,INFINITE) == WAIT_FAILED){
		fputs("Could not lock mutex.\n",stderr);
		exit(-1);
    }
}
///Returns false if timed out and true if locked.
///@ms millisecond range from 0 to UINT32_MAX-1
static inline _Bool mutex_timedlock(mutex_t* mtx, uint32_t ms){
	//sets wait time to one less than specified to avoid infinite wait.
	//INFINITE == -1 (UINT32_MAX)
	DWORD status;;
	if(ms==INFINITE) --ms;
	if((status=WaitForSingleObject(*mtx,ms)) == WAIT_FAILED){
		fputs("Could not timed lock mutex.\n",stderr);
		exit(-1);
	}
	return status != WAIT_TIMEOUT;
}
static inline void mutex_unlock(mutex_t* mtx){
    if(ReleaseMutex(*mtx)==0){
		fputs("Could not unlock mutex.\n",stderr);
		exit(-1);
    }
}
static inline void mutex_delete(mutex_t* mtx){
	if(CloseHandle(*mtx) == 0){
		fputs("Could not delete mutex.\n",stderr);
		exit(-1);
	}
}
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#include <time.h>
#include <errno.h>
typedef pthread_mutex_t mutex_t;
static inline mutex_t mutex_create(void){
    mutex_t mtx;
    if(pthread_mutex_init(&mtx,((void*)0)) != 0){
        fputs("Could not create mutex.\n",stderr);
        exit(-1);
    }
    return mtx;
}
static inline void mutex_lock(mutex_t* mtx){
    if(pthread_mutex_lock(mtx) != 0){
		fputs("Could not lock mutex.\n",stderr);
		exit(-1);
    }
}
///Returns false if timed out and true if locked
///@ms millisecond range from 0 to UINT32_MAX-1
static inline _Bool mutex_timedlock(mutex_t* mtx, uint32_t ms){
	if(ms==UINT32_MAX) --ms;
	int status;
	if((status=pthread_mutex_timedlock(mtx,(const struct timespec[]){{(ms/1000),(ms%1000)*1000000L}})) != 0 && status != ETIMEDOUT){
		fputs("Could not timed lock mutex.\n",stderr);
		exit(-1);
	}
    return status != ETIMEDOUT;
}
static inline void mutex_unlock(mutex_t* mtx){
    if(pthread_mutex_unlock(mtx) != 0){
		fputs("Could not unlock mutex.\n",stderr);
		exit(-1);
    }
}
static inline void mutex_delete(mutex_t* mtx){
    if(pthread_mutex_destroy(mtx) != 0){
		fputs("Could not delete mutex.\n",stderr);
		exit(-1);
    }
}
#elif __STDC_VERSION__ >=  201112L && !defined( __STDC_NO_THREADS__)
#include <threads.h>
typedef mtx_t mutex_t;
static inline mutex_t mutex_create(void){
	mutex_t mtx;
	if(mtx_init(&mtx,mtx_timed | mtx_recursive) != thrd_success){
		fputs("Could not create mutex.\n",stderr);
        exit(-1);
	}
}
static inline void mutex_lock(mutex_t* mtx){
    if(mtx_lock(mtx) != thrd_success){
		fputs("Could not lock mutex.\n",stderr);
		exit(-1);
    }
}
///Returns false if timed out and true if locked
///@ms millisecond range from 0 to UINT32_MAX-1
static inline _Bool mutex_timedlock(mutex_t* mtx, uint32_t ms){
	if(ms==UINT32_MAX) --ms;
	int status;
	if(mtx_timedlock(mtx,(const struct timespec[]){{(ms/1000),(ms%1000)*1000000L}}) == thrd_error){
		fputs("Could not lock timed mutex.\n",stderr);
		exit(-1);
	}
	return status != thrd_timedout;
}
static inline void mutex_unlock(mutex_t* mtx){
	if(mtx_unlock(mtx) != thrd_success){
		fputs("Could not unlock mutex.\n",stderr);
		exit(-1);
	}
}
static inline void mutex_delete(mutex_t* mtx){
	//no return value to check for failure
	//so it better just work I guess
	mtx_destroy(mtx);
}
#else
	#error OS not detected as modern windows or unix, and compiler does not support C11 threads, so no implementation of\
		   mutexing.h is available
#endif // _WIN32

//specialized mutex_create that is
//safe to call in a constructor fn
#ifdef _WIN32
#include <windows.h>
#define mutex_create_constructor(mtx,bvar)\
do{\
	(mtx) = CreateMutexA(((void*)0),0,((void*)0));\
	(bvar) = (mtx != 0);\
}while(0)
#define mutex_delete_destructor(mtx,bvar)\
do{\
	(bvar) = (CloseHandle((mtx)) != 0);\
}while(0)
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <unistd.h>
#define mutex_create_constructor(mtx,bvar)\
do{\
	(bvar) = (pthread_mutex_init(&(mtx),((void*)0)) == 0);\
}while(0)
#define mutex_delete_destructor(mtx,bvar)\
do{\
	(bvar) = (pthread_mutex_lock(&(mtx)) == 0);\
}while(0)
#else
#define mutex_create_constructor(mtx,bvar)\
do{\
	(bvar) = (mtx_init(&(mtx)) == thrd_success);\
}while(0)
#define mutex_delete_destructor(mtx,bvar)\
do{\
	mtx_destroy(&(mtx));\
	(bvar) = 1;\
}while(0)
#endif // _WIN32

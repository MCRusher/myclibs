#pragma once
#include <stdio.h>
#include <stdlib.h>
/*!helper function to avoid false -Wall "statement with no effect"
warning for default arguments via variadic macros*/
#ifndef TOOL_NULL_FN
#define TOOL_NULL_FN
static inline void* tool_NULL_FN(void){ return NULL;}
#endif // TOOL_NULL_FN

#ifdef _WIN32
#include <stdint.h>
#include <windows.h>
#include <process.h>
#define thread_id uintptr_t
#define threaded __stdcall
static inline thread_id thread_start(unsigned(*func)(void*),void* args){
	//was _beginthread(func,0,args), this wasn't detachable
	thread_id id = _beginthreadex(NULL,0,func,args,0,NULL);
	if(id==0){
        fputs("Could not start thread.\n",stderr);
        exit(-1);
	}
	return id;
}
#define thread_start(func,args_ptr...) thread_start((unsigned(*)(void*))(func),(tool_NULL_FN(),##args_ptr))
static inline thread_id thread_this(void){ return (thread_id)GetCurrentThread(); }
static inline void thread_kill(thread_id id){
	if(TerminateThread((HANDLE)id,0)==0){
		fputs("Could not kill thread.\n",stderr);
        exit(-1);
	}
}
static inline void thread_join(thread_id id, void* ret){
	if(WaitForSingleObject((HANDLE)id,INFINITE)==WAIT_FAILED || GetExitCodeThread((HANDLE)id,ret)==0){
		fputs("Could not join thread.\n",stderr);
        exit(-1);
	}
}
#define thread_join(id,ret_ptr...) thread_join((id),(tool_NULL_FN(),##ret_ptr))
static inline void thread_detach(thread_id id){
	if(CloseHandle((HANDLE)id)==0){
		fputs("Could not detach thread.\n",stderr);
        exit(-1);
	}
}
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#include <unistd.h>
#define thread_id pthread_t
#define threaded
static inline thread_id thread_start(void *(*func)(void*), void* args){
	thread_id id;
	if(pthread_create(&id,NULL,func,args)!=0){
		fputs("Could not start thread.\n",stderr);
        exit(-1);
	}
	return id;
}
#define thread_start(func,args_ptr...) thread_start((void*(*)(void*))(func),(tool_NULL_FN(),##args_ptr))
static inline thread_id thread_this(void){ return pthread_self(); }
static inline void thread_kill(thread_id id){
	if(pthread_cancel(id)!=0){
		fputs("Could not kill thread.\n",stderr);
        exit(-1);
	}
}
static inline void thread_join(thread_id id, void* ret){
	if(pthread_join(id,(void**)ret)!=0){
		fputs("Could not join thread.\n",stderr);
        exit(-1);
	}
}
#define thread_join(id,ret_ptr...) thread_join((id),(tool_NULL_FN(),##ret_ptr))
static inline void thread_detach(thread_id id){
    if(pthread_detach(id)!=0){
		fputs("Could not detach thread.\n",stderr);
        exit(-1);
    }
}
#elif __STDC_VERSION__ >=  201112L && !defined( __STDC_NO_THREADS__)
#include <threads.h>
#define thread_id thrd_t;
#defined threaded
static inline thread_id thread_start(thrd_start_t func, void* args){
	thread_id id;
	if(thrd_create(&id,func,args)!=thrd_success){
		fputs("Could not start thread.\n",stderr);
        exit(-1);
	}
	return id;
}
#define thread_start(func,args_ptr...) thread_start(thrd_start_t)(func),(tool_NULL_FN(),##args_ptr))
static inline thread_id thread_this(void){ return thrd_current();
#warning C11 has no equivalent of win32 TerminateThread or posix pthread_cancel, so\
		thread_kill is a forced exit. An atomic condition variable should be preferred anyways.
static inline void thread_kill(thread_id id){
	fputs("Killing another thread is not available using current C11 <threads.h> features.\n",stderr);
	exit(-1);
}
static inline void thread_join(thread_id id, void* ret){
	if(thrd_join(id,(int*)ret)!=thrd_success){
		fputs("Could not join thread.\n",stderr);
        exit(-1);
	}
}
#define thread_join(id,ret_ptr...) thread_join((id),(tool_NULL_FN(),##ret_ptr))
static inline void thread_detach(thread_id id){
	if(thrd_detach(id)!=thrd_success){
		fputs("Could not detach thread.\n",stderr);
        exit(-1);
	}
}
#else
	#error OS not detected as modern windows or unix, and compiler does not support C11 threads, so no implementation of\
		   thread_start(func,args) is available
#endif // _WIN32

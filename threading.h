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
typedef uintptr_t thread_t;
///Required attribute for a function used in thread_start
#define threaded __stdcall
static inline thread_t thread_start(int(threaded *func)(),void* args){
	//was _beginthread(func,0,args), this wasn't detachable
	thread_t id = _beginthreadex(NULL,0,(unsigned(threaded *)(void*))func,args,0,NULL);
	if(id==0){
        fputs("Could not start thread.\n",stderr);
        exit(-1);
	}
	return id;
}
///Creates a thread, starts it with or without a single pointer arg, then returns an associated thread_t
#define thread_start(func,args_ptr...) thread_start((func),(tool_NULL_FN(),##args_ptr))
///Returns a thread_t associated with the current thread
static inline thread_t thread_this(void){ return (thread_t)GetCurrentThread(); }
///Hints the processor to allow other threads to run before the current thread
static inline void thread_yield(void){ SwitchToThread(); }
static inline void thread_join(thread_t id, int* ret){
	if(WaitForSingleObject((HANDLE)id,INFINITE)==WAIT_FAILED || !(!ret || GetExitCodeThread((HANDLE)id,(LPDWORD)ret)!=0)){
		fputs("Could not join thread.\n",stderr);
        exit(-1);
	}
}
///Waits for the specified thread to end, then stores the return int value at ret if specified
#define thread_join(id,ret_ptr...) thread_join((id),(tool_NULL_FN(),##ret_ptr))
///Separates the execution of the specified thread from main thread
static inline void thread_detach(thread_t id){
	if(CloseHandle((HANDLE)id)==0){
		fputs("Could not detach thread.\n",stderr);
        exit(-1);
	}
}
static inline void thread_sleep(uint32_t ms){ Sleep(ms); }
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
typedef pthread_t thread_t;
///required attribute for a function used in thread_start
#define threaded
static inline thread_t thread_start(int(threaded *func)(), void* args){
	thread_t id;
	if(pthread_create(&id,NULL,(void*(threaded *)(void*))(func),args)!=0){
		fputs("Could not start thread.\n",stderr);
        exit(-1);
	}
	return id;
}
///Creates a thread, starts it with or without a single pointer arg, then returns an associated thread_t
#define thread_start(func,args_ptr...) thread_start((func),(tool_NULL_FN(),##args_ptr))
///Returns a thread_t associated with the current thread
static inline thread_t thread_this(void){ return pthread_self(); }
///Hints the processor to allow other threads to run before the current thread
static inline void thread_yield(void){ sched_yield(); }
static inline void thread_join(thread_t id, int* ret){
	if(pthread_join(id,(void**)ret)!=0){
		fputs("Could not join thread.\n",stderr);
        exit(-1);
	}
}
///Waits for the specified thread to end, then stores the return int value at ret if specified
#define thread_join(id,ret_ptr...) thread_join((id),(tool_NULL_FN(),##ret_ptr))
///Separates the execution of the specified thread from main thread
static inline void thread_detach(thread_t id){
    if(pthread_detach(id)!=0){
		fputs("Could not detach thread.\n",stderr);
        exit(-1);
    }
}
static inline void thread_sleep(uint32_t ms){ nanosleep((const struct timespec[]){{(ms/1000),(ms%1000)*1000000L}},((void*)0)); }
#elif __STDC_VERSION__ >=  201112L && !defined( __STDC_NO_THREADS__)
#include <threads.h>
typedef thrd_t thread_t;
///required attribute for a function used in thread_start
#define threaded
static inline thread_t thread_start(int(threaded *func)(), void* args){
	thread_t id;
	if(thrd_create(&id,(thrd_start_t)func,args)!=thrd_success){
		fputs("Could not start thread.\n",stderr);
        exit(-1);
	}
	return id;
}
///Creates a thread, starts it with or without a single pointer arg, then returns an associated thread_t
#define thread_start(func,args_ptr...) thread_start((func),(tool_NULL_FN(),##args_ptr))
///Returns a thread_t associated with the current thread
static inline thread_t thread_this(void){ return thrd_current(); }
///Hints the processor to allow other threads to run before the current thread
static inline void thread_yield(void){ thrd_yield(); }
static inline void thread_join(thread_t id, int* ret){
	if(thrd_join(id,ret)!=thrd_success){
		fputs("Could not join thread.\n",stderr);
        exit(-1);
	}
}
///Waits for the specified thread to end, then stores the return int value at ret if specified
#define thread_join(id,ret_ptr...) thread_join((id),(tool_NULL_FN(),##ret_ptr))
///Separates the execution of the specified thread from main thread
static inline void thread_detach(thread_t id){
	if(thrd_detach(id)!=thrd_success){
		fputs("Could not detach thread.\n",stderr);
        exit(-1);
	}
}
static inline void thread_sleep(uint32_t ms){ thrd_sleep((const struct timespec[]){{(ms/1000),(ms%1000)*1000000L}},((void*)0));}
#else
	#error OS not detected as modern windows or unix, and compiler does not support C11 threads, so no implementation of\
		   threading.h is available
#endif // _WIN32


#pragma once
#include <stdint.h>
#ifdef _WIN32
	#include <windows.h>
	static inline void sleep_for(uint32_t ms) {Sleep(ms);}
#elif defined(unix) || defined(__unix) || defined(__unix__)
	#include <unistd.h>
	#include <time.h>
	int nanosleep(const struct timespec*,struct timespec*);
	static inline void sleep_for(uint32_t ms) {nanosleep((const struct timespec[]){{(ms/1000),(ms%1000)*1000000L}},((void*)0));}
#else
	#error OS was not detected as either modern windows or unix, so a sleep(uint_ms) function cannot be selected
#endif // _WIN32

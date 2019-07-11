#pragma once
#include <stdlib.h>

#ifdef _WIN32
static inline void clear_screen(void){ system("cls"); }
#elif defined(unix) || defined(__unix) || defined(__unix__)
static inline clear_screen(void){ system("clear"); }
#else
#error OS was not detected as modern windows or unix,\
	   so an implementation of clear_screen is not available
#endif // _WIN32

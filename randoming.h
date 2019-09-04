#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//rand_start and rand_end set errno instead of exiting since destructors may be called from an exit,
//exiting twice seems bad and when tested it at least prevented other destructors from running afterwards.
#include <errno.h>
#define ERANDOMING (1337)

#ifdef RANDOMING_USE_RAND
#undef RANDOMING_USE_RAND
#include <stdlib.h>
#include <time.h>
#include "mutexing.h" //required for thread safety

mutex_t rand_mutex;

NEWSTD_CONSTRUCTOR static inline void rand_start(void){
#ifdef _WIN32
	srand(time(((void*)0)) * GetCurrentProcessId());
#elif defined(unix) || defined(__unix) || defined (__unix__)
	srand(time(((void*)0)) * getpid());
#elif __STDC_VERSION__ >=  201112L && !defined( __STDC_NO_THREADS__)
	//uses the current thread's id to seed
	srand(time(((void*)0)) * thrd_current());
#else
	//abuses allocation as a source of entropy
	void* v = malloc(1);
	free(v);
	srand(time(((void*)0)) * (uintptr_t)v);
#endif // _WIN32
    _Bool res = 0;
    mutex_create_constructor(rand_mutex,res);
    if(!res){
		fputs("Could not start randoming module.\n",stderr);
		errno = ERANDOMING;
    }
}
static inline _Bool rand_bool(void){
	mutex_lock(&rand_mutex);
	_Bool val = (rand() >= UINT8_MAX/2);
	mutex_unlock(&rand_mutex);
	return val;
}
static inline uint8_t rand_u8(void){
	mutex_lock(&rand_mutex);
	uint8_t val = (uint8_t)((((double)rand()/RAND_MAX) * (UINT8_MAX-0+1)) + 0);
	mutex_unlock(&rand_mutex);
	return val;
}
static inline uint16_t rand_u16(void){
	mutex_lock(&rand_mutex);
	uint16_t val = (uint16_t)((((double)rand()/RAND_MAX) * (UINT16_MAX-0+1)) + 0);
	mutex_unlock(&rand_mutex);
	return val;
}
static inline uint32_t rand_u32(void){
	mutex_lock(&rand_mutex);
	uint32_t val = (uint32_t)((((double)rand()/RAND_MAX) * (UINT32_MAX-0+1)) + 0);
	mutex_unlock(&rand_mutex);
	return val;
}
static inline uint64_t rand_u64(void){
	mutex_lock(&rand_mutex);
	uint64_t val = (uint64_t)((((double)rand()/RAND_MAX) * (UINT64_MAX-0+1)) + 0);
	mutex_unlock(&rand_mutex);
	return val;
}
static inline void rand_abstract(void* buf, typeof(sizeof 1) bufsize){
	mutex_lock(&rand_mutex);
    for(typeof(sizeof 1) i = 0; i < bufsize; i++){
        ((uint8_t*)buf)[i] = rand_u8();
    }
    mutex_unlock(&rand_mutex);
}
#define rand_abstract(buf,buf_size...) rand_abstract((buf),(sizeof(typeof(*(buf))),##buf_size))
NEWSTD_DESTRUCTOR static inline void rand_end(void){
	_Bool res = 0;
    mutex_delete_destructor(rand_mutex,res);
    if(!res){
		fputs("Could not end randoming module.\n",stderr);
		errno = ERANDOMING;
    }
}
#elif defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
BCRYPT_ALG_HANDLE bch = ((void*)0);
NEWSTD_CONSTRUCTOR static inline void rand_start(void){
	if(bch==((void*)0)){
		if(!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(
			&bch,
			BCRYPT_RNG_ALGORITHM,
			NULL,
			0
		))){
			fputs("Could not start randoming module.\n",stderr);
			errno = ERANDOMING;
		}
	}
}
static inline _Bool rand_bool(void){
	uint8_t rnum;
	if(!BCRYPT_SUCCESS(BCryptGenRandom(
		bch,
		(PUCHAR)&rnum,
		sizeof(rnum),
        0
	))){
		fputs("Could not generate random boolean.\n",stderr);
		exit(-1);
	}
	return rnum >= UINT8_MAX/2;
}
static inline uint8_t rand_u8(void){
	uint8_t rnum;
	if(!BCRYPT_SUCCESS(BCryptGenRandom(
		bch,
		(PUCHAR)&rnum,
		sizeof(rnum),
        0
	))){
		fputs("Could not generate random x8.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline uint16_t rand_u16(void){
	uint16_t rnum;
	if(!BCRYPT_SUCCESS(BCryptGenRandom(
		bch,
		(PUCHAR)&rnum,
		sizeof(rnum),
        0
	))){
		fputs("Could not generate random x16.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline uint32_t rand_u32(void){
	uint32_t rnum;
	if(!BCRYPT_SUCCESS(BCryptGenRandom(
		bch,
		(PUCHAR)&rnum,
		sizeof(rnum),
        0
	))){
		fputs("Could not generate random x32.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline uint64_t rand_u64(void){
	uint64_t rnum;
	if(!BCRYPT_SUCCESS(BCryptGenRandom(
		bch,
		(PUCHAR)&rnum,
		sizeof(rnum),
        0
	))){
		fputs("Could not generate random x64.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline void rand_abstract(void* buf, typeof(sizeof 1) buf_size){
	if(!BCRYPT_SUCCESS(BCryptGenRandom(
		bch,
		(PUCHAR)buf,
		buf_size,
		0
	))){
		fputs("Could not generate random abstract.\n",stderr);
		exit(-1);
	}
}
#define rand_abstract(buf,buf_size...) rand_abstract((buf),(sizeof(typeof(*(buf))),##buf_size))
NEWSTD_DESTRUCTOR static inline void rand_end(void){
	if(bch){
		if((!BCRYPT_SUCCESS(BCryptCloseAlgorithmProvider(
			bch,
			0
		)))){
			fputs("Could not end randoming module.\n",stderr);
			errno = ERANDOMING;
		}
		bch = ((void*)0);
	}
}
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int dev_random = -1;

NEWSTD_CONSTRUCTOR static inline void rand_start(void){
	//prevents recalling from opening additional files
	if(dev_random == -1){
		if(!((dev_random = open("/dev/urandom", O_RDONLY)) != -1 || (dev_random = open("/dev/random", O_RDONLY)) != -1)){
			fputs("Could not start randoming module.\n",stderr);
			errno = ERANDOMING;
		}
	}
}
static inline _Bool rand_bool(void){
	uint8_t rnum;
	if(read(dev_random,&rnum,sizeof(rnum)) == -1){
		fputs("Could not generate random boolean.\n",stderr);
		exit(-1);
	}
	return rnum >= UINT8_MAX/2;
}
static inline uint8_t rand_u8(void){
	uint8_t rnum;
	if(read(dev_random,&rnum,sizeof(rnum)) == -1){
		fputs("Could not generate random x8.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline uint8_t rand_u16(void){
	uint16_t rnum;
	if(read(dev_random,&rnum,sizeof(rnum)) == -1){
		fputs("Could not generate random x16.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline uint32_t rand_u32(void){
	uint32_t rnum;
	if(read(dev_random,&rnum,sizeof(rnum)) == -1){
		fputs("Could not generate random x32.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline uint64_t rand_u64(void){
	uint64_t rnum;
	if(read(dev_random,&rnum,sizeof(rnum)) == -1){
		fputs("Could not generate random x64.\n",stderr);
		exit(-1);
	}
	return rnum;
}
static inline void rand_abstract(void* buf, typeof(sizeof 1) buf_size){
	if(read(dev_random,buf,buf_size) == -1){
		fputs("Could not generate random abstract.\n",stderr);
		exit(-1);
	}
}
#define rand_abstract(buf,buf_size...) rand_abstract((buf),(sizeof(typeof(*(buf))),##buf_size))
NEWSTD_DESTRUCTOR static inline void rand_end(void){
	if(dev_random != -1 && close(dev_random) != 0){
		fputs("Could not end randoming module.\n",stderr);
		errno = ERANDOMING;
	}
	dev_random = -1;
}
#else
	#error OS was not detected as either modern windows or unix, so there is no\
		   decent implementation of random.h available, consider using RANDOM_USE_RAND for an inneficient implementation
#endif // __WIN32
#define rand_i8() ((int8_t)rand_u8())
#define rand_i16() ((int16_t)rand_u16())
#define rand_i32() ((int32_t)rand_u32())
#define rand_i64() ((int64_t)rand_u64())
static inline uint8_t randb_u8(uint8_t min, uint8_t max){
	return (uint8_t)((((double)rand_u8()/UINT8_MAX) * (max-min+1)) + min);
}
static inline uint16_t randb_u16(uint16_t min, uint16_t max){
	return (uint16_t)((((double)rand_u8()/UINT8_MAX) * (max-min+1)) + min);
}
static inline uint32_t randb_u32(uint32_t min, uint32_t max){
	return (uint32_t)((((double)rand_u8()/UINT8_MAX) * (max-min+1)) + min);
}
static inline uint64_t randb_u64(uint64_t min, uint64_t max){
	return (uint64_t)((((double)rand_u8()/UINT8_MAX) * (max-min+1)) + min);
}
#define randb_i8(min,max) ((int8_t)randb_u8((uint8_t)(min),(uint8_t)(max)))
#define randb_i16(min,max) ((int16_t)randb_u16((uint16_t)(min),(uint16_t)(max)))
#define randb_i32(min,max) ((int32_t)randb_u32((uint32_t)(min),(uint32_t)(max)))
#define randb_i64(min,max) ((int64_t)randb_u64((uint64_t)(min),(uint64_t)(max)))

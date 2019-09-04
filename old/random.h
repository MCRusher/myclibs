#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//rand_start and rand_end set errno instead of exiting since destructors may be called from an exit,
//exiting twice seems bad and when tested it at least prevented other destructors from running afterwords.
#include <errno.h>
#define RAND_ERRNO (1337)

#ifdef __WIN32
#include <windows.h>
#include <bcrypt.h>
BCRYPT_ALG_HANDLE bch = ((void*)0);
__attribute__ ((constructor)) static inline void rand_start(void){
	if(!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(
		&bch,
		BCRYPT_RNG_ALGORITHM,
		NULL,
		0
	))){
		fputs("Could not start RNG module.\n",stderr);
		errno = 1337;
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
static inline void rand_abstract(void* buf, size_t buf_size){
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
__attribute__ ((destructor)) static inline void rand_end(void){
	if((bch == ((void*)0)) || !BCRYPT_SUCCESS(BCryptCloseAlgorithmProvider(
		bch,
		0
	))){
		fputs("Could not end RNG module.\n",stderr);
		errno = 1337;
	}
	bch = ((void*)0);
}
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int dev_random = -1;

__attribute__ ((constructor)) static inline void rand_start(void){
	if((dev_random = open("/dev/random", O_RDONLY)) == -1){
		fputs("Could not start RNG module.\n",stderr);
		errno = 1337;
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
static inline void rand_abstract(void* buf, size_t buf_size){
	if(read(dev_random,buf,buf_size) == -1){
		fputs("Could not generate random abstract.\n",stderr);
		exit(-1);
	}
}
#define rand_abstract(buf,buf_size...) rand_abstract((buf),(sizeof(typeof(*(buf))),##buf_size))
__attribute__ ((destructor)) static inline void rand_end(void){
	if(dev_random != -1 || close(dev_random) != 0){
		fputs("Could not end RNG module.\n",stderr);
		errno = 1337;
	}
	dev_random = -1;
}
#else
	#error OS was not detected as either modern windows or unix, so there is no\
		   available implementation of random.h
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
#define randb_i64(min,max) ((int8_t)randb_u64((uint64_t)(min),(uint64_t)(max)))

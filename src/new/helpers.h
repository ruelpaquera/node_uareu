/* 
 * Copyright (C) 2011, Digital Persona, Inc.
 *
 * This file is a part of sample code for the UareU SDK 2.x.
 */

#pragma once
#include <nan.h>
#include <dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>

using namespace v8;
using v8::FunctionTemplate;

typedef struct __ENROLLFP_DATA__ {
    uv_async_t async;
    Nan::Persistent<Function> callback; 
    int result; // noting = 0, process = 1, complete = 2 
    int finger = 0;
    unsigned char *pImage;
    unsigned char *pFmd; 
	unsigned int nFmdSize = 0;
	unsigned int pImageSize = 0;
} ENROLLFP_DATA;

typedef struct __ENROLLFP_STOP__ {
    uv_async_t async;
    Nan::Persistent<Function> callback;
} ENROLLFP_STOP;


typedef void (*fpEnroll_start_cb_)(void *edata);
typedef void (*fpVerify_start_cb_)(void *edata);
typedef struct __VERIFYFD_DATA__ {
    uv_async_t async;
    Nan::Persistent<Function> callback; 
    int result; // noting = 0, process = 1, complete = 2
    unsigned char *pFmd1; 
    unsigned char *pFmd2;
    Local<Array> pFmd3 = Nan::New<Array>();
	unsigned int nFmdSize1 = 0;
	unsigned int nFmdSize2 = 0;
    unsigned char *pImage;
	unsigned int pImageSize = 0;
} VERIFYFD_DATA;

typedef struct __VERIFYFD_STOP__ {
    
    uv_async_t async;
    Nan::Persistent<Function> callback;

} VERIFYFD_STOP;

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

void print_error(const char* szFunctionName, int nError);
void dispose();
int verifyFP(unsigned char* ppFt1, unsigned char* ppFt2,unsigned int nFmdSize1,unsigned int nFmdSize2);

int CaptureFinger(DPFPDD_DEV hReader, int dpi, DPFJ_FMD_FORMAT nFtType, unsigned char** ppFt, unsigned int* pFtSize,unsigned char **ppImage,unsigned int* _nOrigImageSize);
int CaptureFinger_(DPFPDD_DEV hReader, int dpi, DPFJ_FMD_FORMAT nFtType, unsigned char** ppFt, unsigned int* pFtSize,unsigned char **ppImage,unsigned int* _nOrigImageSize);

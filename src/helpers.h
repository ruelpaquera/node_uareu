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
} ENROLLFP_DATA;

typedef struct __ENROLLFP_STOP__ {
    uv_async_t async;
    Nan::Persistent<Function> callback;
} ENROLLFP_STOP;

// DPFPDD_DEV hReader = NULL; //handle of the selected reader
// int dpi = 0;
// char szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

typedef void (*fpEnroll_start_cb_)(void *,int ,unsigned char *,unsigned char *,unsigned int );
 

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

//error handling
void print_error(const char* szFunctionName, int nError);

//returns 0 if captured, otherwise an error code
int CaptureFinger(const char* szFingerName, DPFPDD_DEV hReader, int dpi, DPFJ_FMD_FORMAT nFtType, unsigned char** ppFt, unsigned int* pFtSize);
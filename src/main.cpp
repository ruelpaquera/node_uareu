#include "main.h"

#include "helpers.h"
#include "identify.h"
#include "selection.h"
 
#include <dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <locale.h>

#define MAX_MENU_STRING_LEN 120

#define MENU_TYPE_BACK 0x1
#define MENU_TYPE_EXIT 0x2

int initalized = -1;

using namespace v8;
using v8::FunctionTemplate;

NAN_METHOD(init)
{
    sigset_t sigmask;
	sigfillset(&sigmask);
	pthread_sigmask(SIG_BLOCK, &sigmask, NULL);
	setlocale(LC_ALL, "");
    initalized  = dpfpdd_init();
    if(DPFPDD_SUCCESS != initalized) print_error("dpfpdd_init()", initalized);
    info.GetReturnValue().Set(initalized == 0);
    return;
}


typedef struct __VERIFY_START__ {
    uv_async_t async;
    Nan::Persistent<Function> callback;
    int result;
    struct fp_print_data *fpdata;
} VERIFY_START;

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

void enroll_after(uv_handle_t* handle)
{
    VERIFY_START *enrollData = container_of((uv_async_t *)handle, VERIFY_START, async);

    if(!enrollData)
        return;

    // if(enrollData->fingerprint_data)
    //     free(enrollData->fingerprint_data);

    // enrollData->fingerprint_data = NULL;
    // enrollData->fingerprint_img = NULL;
    // enrollData->fingerprint_size = 0;
    delete enrollData;
}
#ifndef OLD_UV_RUN_SIGNATURE
void report_verify_start(uv_async_t *handle)
#else
void report_verify_start(uv_async_t *handle, int status)
#endif
{
    VERIFY_START *enrollData = container_of(handle, VERIFY_START, async);
    Nan::HandleScope scope;
    Nan::Callback callback(Nan::New<Function>(enrollData->callback));
    Nan::AsyncResource asyncResource("enrollProgress");
    Local<Value> argv[3];
    argv[0] = Nan::New(13);
    argv[1] = Nan::Null();

    printf("aw");

    argv[1] = Nan::New("fingerprintimg.c_str()").ToLocalChecked();
    
    callback.Call(3, argv, &asyncResource);
    uv_close((uv_handle_t*)&enrollData->async,enroll_after);
}
static void verify_start(void *edata)
{
    VERIFY_START *data = (VERIFY_START*)edata;
}
NAN_METHOD(startScan)
{ 
    bool ret = false;
    VERIFY_START *data;
    data = new VERIFY_START;

    int val = Nan::To<v8::Number>(info[0]).ToLocalChecked()->Value();
    Nan::Persistent<Function> callback;

    data->callback.Reset(v8::Local<v8::Function>::Cast(info[2]));
    uv_async_init(uv_default_loop(), &data->async, report_verify_start);


    // info.GetReturnValue().Set(val); 
    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}

NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startScan);
    // NAN_EXPORT(target, openDevice); 
}

NODE_MODULE(fingerprint, module_init)
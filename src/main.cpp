#include "main.h"

#include "helpers.h"
#include "identify.h"
#include "selection.h"
#include "enrollment.h"
 
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

#include <unistd.h>

#define MAX_MENU_STRING_LEN 120

#define MENU_TYPE_BACK 0x1
#define MENU_TYPE_EXIT 0x2

int initalized = -1;
int loops = 0;
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
NAN_METHOD(getReader)
{
    // hReader = GetReader(szReader, sizeof(szReader),&dpi);   
    // info.GetReturnValue().Set(initalized == 0);
}
NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startEnroll);
    NAN_EXPORT(target, getReader);
}

NODE_MODULE(fingerprint, module_init)


// typedef struct __VERIFY_START__ {
//     uv_async_t async;
//     Nan::Persistent<Function> callback;
//     int result;
//     DPFPDD_CAPTURE_PARAM *fpdata;
// } VERIFY_START;

/*/ #define container_of(ptr, type, member) ({			\
// 	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
// 	(type *)( (char *)__mptr - offsetof(type,member) );})*/

// void enroll_after(uv_handle_t* handle)
// {
//     VERIFY_START *data = container_of((uv_async_t *)handle, VERIFY_START, async);

//     if(!data)
//         return;

//     if(data->fpdata)
//         free(data->fpdata);

//     data->fpdata = NULL;
//     data->result = 0;
//     // enrollData->fingerprint_size = 0;
//     delete data;
// }
// #ifndef OLD_UV_RUN_SIGNATURE
// void report_verify_start(uv_async_t *handle)
// #else
// void report_verify_start(uv_async_t *handle, int status)
// #endif
// {
//     printf("report_verify_start");
//     VERIFY_START *data = container_of(handle, VERIFY_START, async);
//      Nan::HandleScope scope;
//     if(!data)
//         return;
 
//     Nan::Callback callback(Nan::New<Function>(data->callback));
//     Nan::AsyncResource asyncResource("verifyProgress");
//     Local<Value> argv[3];
//     argv[0] = Nan::New(data->result);
//     argv[1] = Nan::Null();

//     argv[1] = Nan::New("fingerprintimg.c_str()").ToLocalChecked();
    
//     callback.Call(2, argv, &asyncResource);
//     //if(data->result == 1769107579){
//         if (loops == 5){
//             printf("report_verify_start");
//             uv_close((uv_handle_t*)&data->async,enroll_after);
//         } else {
//             printf("report_verify_start else");
//         }
//     //}
// }
// static void verify_start_cb(void *edata)
// {
//     VERIFY_START *data = (VERIFY_START*)edata;


//     printf("\ncallback called %d \n",data->result);
//     // data->result = data->result; 
//     uv_async_send(&data->async);  

// }
// NAN_METHOD(startScan)
// { 
//     int result = 0;
//     bool ret = false;
//     VERIFY_START *data;

//     data = new VERIFY_START;

//     int val = Nan::To<v8::Number>(info[0]).ToLocalChecked()->Value(); 
//     if(!data)
//         goto error;
    
//     uv_async_init(uv_default_loop(), &data->async, report_verify_start);
//     data->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));

//         data->result++;
//         verify_start_cb((void*)data);
 
//     ret = true;
// error:
//     info.GetReturnValue().Set(Nan::New(ret));
//     return;
// }

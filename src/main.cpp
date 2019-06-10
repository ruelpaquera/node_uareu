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

static DPFPDD_DEV _hReader = NULL; //handle of the selected reader
static int _dpi = 0;
char _szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

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
NAN_METHOD(startScan)
{ 
    int val = Nan::To<v8::Number>(info[0]).ToLocalChecked()->Value();
    uv_async_t async;
    Nan::Persistent<Function> callback;

    callback.Reset(v8::Local<v8::Function>::Cast(info[2]));
    uv_async_init(uv_default_loop(), &async, report_verify_start);
    // info.GetReturnValue().Set(val); 
}

NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startScan); 
}

NODE_MODULE(fingerprint, module_init)
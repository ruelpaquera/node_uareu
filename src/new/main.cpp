#include "main.h"
#include "helpers.h"
#include "verify.h"

#include <dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>

#define MAX_MENU_STRING_LEN 120

#define MENU_TYPE_BACK 0x1
#define MENU_TYPE_EXIT 0x2

int initalized = -1;
int loops = 0;
using namespace v8;
using v8::FunctionTemplate;


NAN_METHOD(init)
{ 
    initalized  = dpfpdd_init();
    if(DPFPDD_SUCCESS != initalized) print_error("dpfpdd_init() 1", initalized);
    info.GetReturnValue().Set(initalized == 0);
    return;
}


NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startVerify);
    NAN_EXPORT(target, startVerifyMulti);
    NAN_EXPORT(target, stopVerify);
}
NODE_MODULE(fingerprint, module_init)
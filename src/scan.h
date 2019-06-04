#include "fingerprint.h"
#include "selection.h"  
#include "helpers.h"

#include <dpfpdd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

using namespace v8;
using v8::FunctionTemplate;

typedef struct __FINGER_DATA__ {
    Nan::Persistent<Function> callback;
    unsigned char *fingerprint_data;
    size_t fingerprint_size;
    int result;
    unsigned char *fingerprint_img;
} FINGER_DATA;

NAN_METHOD(checkharware) {
    int returns;
    FINGER_DATA *pdata;

    pdata->fingerprint_data = NULL;
    pdata->fingerprint_size = NULL;
    pdata->result = 0;

    returns = true;
error:
    info.GetReturnValue().Set(Nan::New(returns));
    return;
}
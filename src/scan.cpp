#include "fingerprint.h"
#include "selection.h"  
#include "helpers.h"
#include "scan.h"


using namespace v8;
using v8::FunctionTemplate;

typedef struct __FINGER_DATA__ {
    Nan::Persistent<Function> callback;
    unsigned char *fingerprint_data;
    int fingerprint_size;
    int result;
    unsigned char *fingerprint_img;
} FINGER_DATA;

NAN_METHOD(startScan) {
    int returns;
    int bDone = 0;
    FINGER_DATA *pdata;
    pdata = new FINGER_DATA;
    
    if(!pdata)
        goto error;
        
    pdata->fingerprint_data = NULL;
    pdata->fingerprint_size = 0;
    pdata->result = 0;

    pdata->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));
    
    while(!bDone){

        
    }


    returns = true;
error:
    info.GetReturnValue().Set(Nan::New(returns));
    return;
}
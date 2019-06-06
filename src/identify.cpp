#include "fingerprint.h"
#include "selection.h"  
#include "helpers.h"
#include "identify.h"


using namespace v8;
using v8::FunctionTemplate;

typedef struct __IDENTIFY_DATA__ {
    uv_async_t async;
    Nan::Persistent<Function> callback; 
    int result;
    unsigned char *pImage;
    unsigned char *pFmd; 
	unsigned int nFmdSize = 0;
} IDENTIFY_DATA;


// #ifndef OLD_UV_RUN_SIGNATURE
// void scan_progress(uv_async_t *handle)
// #else
// void scan_progress(uv_async_t *handle, int status)
// #endif {
//     // IDENTIFY_DATA *scanData = container_of(handle, IDENTIFY_DATA, async);
//     // Nan::HandleScope scope;
//     //  if(!scanData)
//     //     return;

//     // callback.Call(4, argv, &asyncResource);
//     // uv_close((uv_handle_t*)&scanData->async, NULL);
// }


NAN_METHOD(startScan) {
    int returns;
    int bDone = 1;
    IDENTIFY_DATA *pdata;
    pdata = new IDENTIFY_DATA;

    if(info.Length() < 2)
        return;
    
    if(!pdata)
        goto error;
        
    // pdata->pImage = NULL;
    // pdata->pFmd = 0;
    // pdata->nFmdSize = 0;

    // uv_async_init(uv_default_loop(), &pdata->async, scan_progress);
    // pdata->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));
    
    while(!bDone){

        
    } 

    returns = true;
error:
    info.GetReturnValue().Set(Nan::New(returns));
    return;
}
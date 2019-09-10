#include "verify.h"

#include "helpers.h"
#include "capture.h"
#include "base64.h"
 
#include <dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>


using namespace v8;
using namespace std;
using v8::FunctionTemplate; 

void verifyfp_after(uv_handle_t* handle)
{
    VERIFYFD_DATA *FPdata = container_of((uv_async_t *)handle, VERIFYFD_DATA, async);

    if(!FPdata)
        return;

    if(FPdata->pFmd1)
        free(FPdata->pFmd1);
    if(FPdata->pFmd2)
        free(FPdata->pFmd2);
 
    FPdata->result = 0; 
    delete FPdata;
} 
static void fpVerify_start_cb(void *edata)
{
    VERIFYFD_DATA *fpdata = (VERIFYFD_DATA*)edata; 

    if(!fpdata && fpdata->pFmd1 == NULL)
        return;

        
    if(!fpdata && fpdata->pFmd2 == NULL)
        return;
        

    Nan::Callback callback(Nan::New<Function>(fpdata->callback));
    Nan::AsyncResource asyncResource("verifyProgress");
    Local<Value> argv[3];

    argv[0] = Nan::Null();
    argv[1] = Nan::Null(); 
    
    
    // std::string pImage = base64_encode(reinterpret_cast<unsigned char* >(fpdata->pImage),fpdata->pImageSize);

    int result = verifyFP(
        fpdata->pFmd1,
        fpdata->pFmd2,
        (unsigned int )1769473,
        fpdata->nFmdSize2
    );

    argv[0] = Nan::New(result);
    // argv[1] = Nan::New(pImage.c_str()).ToLocalChecked();

    
    // // std::string pImage =  Nan::NewBuffer((unsigned char*)fpdata->pImage, fpdata->pFmd);
    // // std::string pImage = (char *)fpdata->pImage;
    // // std::string pFmd = (char *)fpdata->pFmd;
    // // std::string pImage = (char *)fpdata->pImage;
    // std::string pImage = base64_encode(reinterpret_cast<unsigned char* >(fpdata->pImage),fpdata->pImageSize);
    // // std::string pFmd = (char *)fpdata->pFmd;
    // std::string pFmd = base64_encode(reinterpret_cast<unsigned char* >(fpdata->pFmd),fpdata->nFmdSize);
    // // argv[3] = Nan::New(fingerprintimg.c_str()).ToLocalChecked();
    // // std::string pImage = "fpdata->pImage";
    // // std::string pFmd = "fpdata->pFmd";
    // printf("\nfpdata->pFmd1 %p\n",fpdata->pFmd1);
    // printf("\nfpdata->pFmd2 %p\n",fpdata->pFmd2);

    // argv[1] = Nan::New(pImage.c_str()).ToLocalChecked();
    // argv[2] = Nan::New(pFmd.c_str()).ToLocalChecked();

    callback.Call(2, argv, &asyncResource); 

    if(fpdata->pFmd1 != NULL)
        free(fpdata->pFmd1);
    if(fpdata->pFmd2 != NULL)
        free(fpdata->pFmd2); 
}
NAN_METHOD(startVerify)
{ 
    std::string bar = "";
    std::string bar_base64_decode = "";
    v8::Local<v8::Object> jsonObj = info[0]->ToObject();
    v8::Local<v8::String> barProp = Nan::New("fmt").ToLocalChecked();
 
    if (Nan::HasOwnProperty(jsonObj, barProp).FromJust()) {
        v8::Local<v8::Value> barValue = Nan::Get(jsonObj, barProp).ToLocalChecked();
        bar = std::string(*Nan::Utf8String(barValue));
    }
    int fingers = 1;
    bool ret = false;
    VERIFYFD_DATA *FPdata; 
    FPdata = new VERIFYFD_DATA; 

    if(!FPdata) goto error; 
        bar_base64_decode = base64_decode(bar);
        printf("\n-----------------------------------------------------\n"); 
        FPdata->pFmd1 = (unsigned char*)malloc(bar_base64_decode.size());
        memcpy(FPdata->pFmd1 , bar_base64_decode.c_str(), bar_base64_decode.size());
        FPdata->pFmd2 = (unsigned char*)NULL;
        FPdata->nFmdSize1 = bar_base64_decode.size();
        FPdata->nFmdSize2 = 0;

        FPdata->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));

        CaptureVerify(&fingers,fpVerify_start_cb,(void*)FPdata);
        
    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}       

/********************************************************************/


// void verify_stop_after(uv_handle_t* handle)
// {
//     VERIFYFD_STOP *data = container_of((uv_async_t *)handle, VERIFYFD_STOP, async);

//     if(!data)
//         return;

//     delete data;
// }

// #ifndef OLD_UV_RUN_SIGNATURE
// void report_verify_stop(uv_async_t *handle)
// #else
// void report_verify_stop(uv_async_t *handle, int status)
// #endif
// {
//     VERIFYFD_STOP *data = container_of(handle, VERIFY_STOP, async);
//     Nan::HandleScope scope;

//     if(!data)
//         return;

//     Nan::Callback callback(Nan::New<Function>(data->callback));
//     Nan::AsyncResource asyncResource("verifyStopped");
//     callback.Call(0, NULL, &asyncResource);
//     uv_close((uv_handle_t*)&data->async, verify_stop_after);
// }

// static void verify_stop_cb(struct fp_dev *dev, void *user_data)
// {
//     VERIFYFD_STOP *data = (VERIFYFD_STOP*)user_data;

//     if(!data)
//         return;

//     uv_async_send(&data->async);
// }


NAN_METHOD(stopVerify) {
    bool ret = false;
    struct fp_dev *dev;
    VERIFYFD_STOP *FPdata;

    if(info.Length() < 2)
        goto error;

    // dev = toFPDev(Nan::To<v8::Number>(info[0]).ToLocalChecked()->Value());
    // if(initalized != 0 || dev == NULL)
    //     goto error;

    // data = new VERIFY_STOP;
    // data->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));
    // uv_async_init(uv_default_loop(), &data->async, report_verify_stop);
    // ret = fp_async_verify_stop(dev, verify_stop_cb, data) == 0;

error:
    info.GetReturnValue().Set(Nan::New(ret));
}

/********************************************************************/
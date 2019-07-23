#include "enrollment.h"

#include "helpers.h"
#include "capture.h"
#include "base64.h"
 
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
#include <sys/time.h>

using namespace v8;
using v8::FunctionTemplate; 

int max_finger = 3; 


void enrollfp_after(uv_handle_t* handle)
{
    ENROLLFP_DATA *FPdata = container_of((uv_async_t *)handle, ENROLLFP_DATA, async);

    if(!FPdata)
        return;

    if(FPdata->pImage)
        free(FPdata->pImage);

    FPdata->pImage = NULL;
    FPdata->pFmd = NULL; 
    FPdata->nFmdSize = 0; 
    delete FPdata;
}
//,int result,unsigned char *pImage,unsigned char *pFmd,unsigned int nFmdSize
static void fpEnroll_start_cb(void *edata)
{
    ENROLLFP_DATA *fpdata = (ENROLLFP_DATA*)edata; 

    if(!fpdata && fpdata->pFmd == NULL)
        return;
 
    // printf("\ncallback return %s",fpdata->pImage); 
    // printf("\ncallback return %s",fpdata->pFmd); 
    // printf("\ncallback return %d",fpdata->nFmdSize);  

    Nan::Callback callback(Nan::New<Function>(fpdata->callback));
    Nan::AsyncResource asyncResource("verifyProgress");
    Local<Value> argv[3];
    argv[0] = Nan::New(fpdata->result);
    argv[1] = Nan::Null();
    argv[2] = Nan::Null();


    
    // std::string pImage =  Nan::NewBuffer((unsigned char*)fpdata->pImage, fpdata->pFmd);
    // std::string pImage = (char *)fpdata->pImage;
    // std::string pFmd = (char *)fpdata->pFmd;
    // std::string pImage = (char *)fpdata->pImage;
    std::string pImage = base64_encode(reinterpret_cast<unsigned char* >(fpdata->pImage),fpdata->pImageSize);
    // std::string pFmd = (char *)fpdata->pFmd;
    std::string pFmd = base64_encode(reinterpret_cast<unsigned char* >(fpdata->pFmd),fpdata->nFmdSize);
    // argv[3] = Nan::New(fingerprintimg.c_str()).ToLocalChecked();
    // std::string pImage = "fpdata->pImage";
    // std::string pFmd = "fpdata->pFmd";


    argv[1] = Nan::New(pImage.c_str()).ToLocalChecked();
    argv[2] = Nan::New(pFmd.c_str()).ToLocalChecked();

    callback.Call(3, argv, &asyncResource); 

    // if(NULL != pImage) free(pImage);
    // if(NULL != pFmd) free(pFmd);
    printf("\n-----------------------------------------------------\n"); 
}

NAN_METHOD(startEnroll)
{
    int finger = 1;
    bool ret = false;
    ENROLLFP_DATA *FPdata;

    FPdata = new ENROLLFP_DATA;
    
    if(!FPdata) goto error;

        // printf("\n-----------------------------------------------------\n"); 
        FPdata->pImage = NULL;
        FPdata->pFmd = NULL;
        FPdata->nFmdSize = 0;
        FPdata->result = -1;
        finger = Nan::To<v8::Number>(info[0]).ToLocalChecked()->Value();
        FPdata->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));
 
        fingerCapture(&finger,fpEnroll_start_cb,(void*)FPdata);
        
    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}
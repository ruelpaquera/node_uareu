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
//,int result,unsigned char *pImage,unsigned char *pFmd,unsigned int nFmdSize
static void fpVerify_start_cb(void *edata)
{
    VERIFYFD_DATA *fpdata = (VERIFYFD_DATA*)edata; 

    if(!fpdata && fpdata->pFmd1 == NULL)
        return;

        
    if(!fpdata && fpdata->pFmd2 == NULL)
        return;
        
 
    // printf("\ncallback return %s",fpdata->pImage); 
    // printf("\ncallback return %s",fpdata->pFmd); 
    // printf("\ncallback return %d",fpdata->nFmdSize);  

    Nan::Callback callback(Nan::New<Function>(fpdata->callback));
    Nan::AsyncResource asyncResource("verifyProgress");
    Local<Value> argv[3];

    //return to middleware result of verification
    argv[0] = Nan::New(fpdata->result);
    argv[1] = Nan::Null();
    argv[2] = Nan::Null();
    
    // start verification / checking fingers
    
    int result = verifyFP(
        fpdata->pFmd1,
        fpdata->pFmd2,
        fpdata->nFmdSize1,
        fpdata->nFmdSize1
    );


    if(!result){
        return;
    }
    
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


    // argv[1] = Nan::New(pImage.c_str()).ToLocalChecked();
    // argv[2] = Nan::New(pFmd.c_str()).ToLocalChecked();

    callback.Call(3, argv, &asyncResource); 
    printf("\n-----------------------------------------------------\n"); 
}

NAN_METHOD(startVerify)
{ 
    std::string bar = "";
    v8::Local<v8::Object> jsonObj = info[0]->ToObject();
    v8::Local<v8::String> barProp = Nan::New("fmt").ToLocalChecked();
 
    if (Nan::HasOwnProperty(jsonObj, barProp).FromJust()) {
        v8::Local<v8::Value> barValue = Nan::Get(jsonObj, barProp).ToLocalChecked();
        bar = std::string(*Nan::Utf8String(barValue));
    }
    // std::cout << base64_decode(bar);
    char* fmt;
    string fmt_ = "";
    int fingers = 2;
    bool ret = false;
    VERIFYFD_DATA *FPdata;
    std::string s;
    FPdata = new VERIFYFD_DATA; 

    if(!FPdata) goto error; 

        printf("\n-----------------------------------------------------\n"); 
        FPdata->pFmd1 = (unsigned char*)base64_decode(bar).c_str();
        FPdata->pFmd2 = NULL;
        FPdata->nFmdSize1 = 0;
        FPdata->nFmdSize2 = 0; 
        // printf("\n %p \n",p);
        

        FPdata->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));

        fingerCapture(&fingers,fpVerify_start_cb,(void*)FPdata);
        
    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}       
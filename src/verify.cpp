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
    
    printf("\nverify fail\n");
    // start verification / checking fingers
    
    int result = verify(
        fpdata->pFmd1,
        fpdata->pFmd2
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

    std::cout << base64_decode(bar);
    char* fmt;
    string fmt_ = "tae";
    int fingers = 1;
    bool ret = false;
    VERIFYFD_DATA *FPdata;
    std::string s;
    FPdata = new VERIFYFD_DATA;
    printf("\ncall\n");
    if(!FPdata) goto error;
        // auto fmt_ = base64_decode(bar).c_str();
        printf("\n-----------------------------------------------------\n"); 
        FPdata->pFmd1 = (unsigned char*)base64_decode(bar).c_str();
        FPdata->pFmd2 = NULL;
        FPdata->result = -1;
        //fmt = Nan::Local<v8::String> Nan::New<String::ExternalStringResource * info[0]>;
        // Nan::MaybeLoca=l<v8::String> Nan::New<T>(char * fmt_);
        // std::string fmd(v8::Value(info[0]->ToString()));
        // Local<String> ss_prop = Nan::New<String>(info[0]).ToLocalChecked();
        // cout << ss_prop;
        // fmt = *(v8::String::Utf8Value(info[0]->ToString()));
        // Nan::To<v8::Number>(info[0]).ToLocalChecked()->Value();
        // fmt = Nan::To<v8::Object>(info[0]).ToLocalChecked()->ToObject();
        // v8::String::Utf8Value arg(info[0]->ToString());
        // fmt_ = Nan::To<v8::String>(info[0]);
        // Nan::Utf8String teststr = Nan::New<String::ExternalStringResource * info[0]>;
        // fmt = (char *)v8::Local<v8::Object>::Cast(info[0]);
        // fmt = static_cast<char *>(info[0]);
        // v8::String::Utf8Value param1(info[0]);
        // v8::Local<v8::Value> p(info[0]);
        // cout << p;
        // Local<string> tests = info[0]->ToString();
        // printf("\n %p \n",p);
        

        FPdata->callback.Reset(v8::Local<v8::Function>::Cast(info[1]));

        fingerCapture(&fingers,fpVerify_start_cb,(void*)FPdata);
        
    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}       
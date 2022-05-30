#include "register.h"

#include "helpers.h"
#include "capture.h"
#include "base64.h"

#include <dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>

using namespace v8;
using namespace std;
using v8::FunctionTemplate;

static void fpScann_start_cb(void *edata) {
    SCANING_HANDLER *fpdata = (SCANING_HANDLER*)edata;
    if(!fpdata && fpdata->pFmd == NULL)
        return;

    Nan::Callback callback(Nan::New<Function>(fpdata->callback));
    Nan::AsyncResource asyncResource("scanningProgress");
    Local<Value> argv[2]; 

    int result = 0; 

    std::string fmd_base64_encode = base64_encode(fpdata->pFmd,502);
    std::string image_base64_encode = base64_encode(fpdata->pImage,fpdata->pImageSize); 
        // std::string sName(reinterpret_cast<char*>(fpdata->pFmd));
    // std::string bar_base64_decode = base64_decode(sName); 
    // std::string bar_base64_encode = base64_encode(fpdata->pFmd,502); 

    // unsigned char *pFmd = (unsigned char*)malloc(bar_base64_encode.size());

    // memcpy(pFmd, bar_base64_encode.c_str(), bar_base64_encode.size());  

 
    argv[0] = Nan::New<String>(fmd_base64_encode).ToLocalChecked();
    argv[1] = Nan::New<String>(image_base64_encode).ToLocalChecked();

    callback.Call(2, argv, &asyncResource);  
    if(fpdata->pFmd != NULL)
        free(fpdata->pFmd);
}

NAN_METHOD(scanning)
{

    bool ret = false;
    SCANING_HANDLER *ScanData; 
    ScanData = new SCANING_HANDLER;  
    if(!ScanData) goto error;  

    ScanData->callback.Reset(v8::Local<v8::Function>::Cast(info[0]));
    Capture(fpScann_start_cb,(void*)ScanData);
    
    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}       

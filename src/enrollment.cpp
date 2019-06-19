#include "enrollment.h"

#include "helpers.h"
#include "capture.h"
 
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

static void fpEnroll_start_cb(void *edata,int result,unsigned char *pImage,unsigned char *pFmd,unsigned int nFmdSize)
{
    ENROLLFP_DATA *fpdata = (ENROLLFP_DATA*)edata; 

    if(!fpdata && fpdata->pFmd == NULL)
        return;
 
    fpdata->pImage = pImage;
    fpdata->pFmd = pFmd;
    fpdata->nFmdSize = nFmdSize; 
    fpdata->result = result;

    printf("\ncallback return %s",fpdata->pImage); 
    printf("\ncallback return %s",fpdata->pFmd); 
    printf("\ncallback return %d",fpdata->nFmdSize);  

    Nan::Callback callback(Nan::New<Function>(fpdata->callback));
    Nan::AsyncResource asyncResource("verifyProgress");
    Local<Value> argv[3];
    argv[0] = Nan::New(fpdata->result);
    argv[1] = Nan::Null();
    argv[2] = Nan::Null();

    argv[1] = Nan::New(pImage);
    argv[2] = Nan::New(pFmd);

    callback.Call(3, argv, &asyncResource); 

    if(NULL != pImage) free(pImage);
    if(NULL != pFmd) free(pFmd);
}

NAN_METHOD(startEnroll)
{
    // int result = 0; 
    int finger = 0;
    bool ret = false;
    ENROLLFP_DATA *FPdata;

    FPdata = new ENROLLFP_DATA;
    
    if(!FPdata) goto error;

        FPdata->pImage = NULL;
        FPdata->pFmd = NULL;
        FPdata->nFmdSize = 0;
        FPdata->result = -1;
         
        FPdata->callback.Reset(v8::Local<v8::Function>::Cast(info[0]));
 
        fingerCapture(&finger,fpEnroll_start_cb,(void*)FPdata);
        

    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}











        // printf(" putang ina %d ",result_);
        // if(result_ == 1){
        //     fpEnroll_start_cb((void*)FPdata);
        // }
        // void * context_unused = NULL;

        // DPFPDD_CAPTURE_PARAM cparam;
        // cparam.size = sizeof(cparam);
        // cparam.image_fmt = DPFPDD_IMG_FMT_ISOIEC19794;
        // cparam.image_proc = DPFPDD_IMG_PROC_NONE;
        // cparam.image_res = dpi;
	    // DPFPDD_CAPTURE_RESULT cresult;
        // cresult.size = sizeof(cresult);
        // cresult.info.size = sizeof(cresult.info);
	    // unsigned int nOrigImageSize = 0;
        /*
        typedef struct __ENROLLFP_DATA__ {
            uv_async_t async;
            Nan::Persistent<Function> callback; 
            int result;
            unsigned char *pImage;
            unsigned char *pFmd; 
            unsigned int nFmdSize = 0;
        } ENROLLFP_DATA;
        */
        // result = dpfpdd_capture(hReader,&cparam, -1,  &cresult,&nOrigImageSize, FPdata->pImage);
        // if(DPFPDD_E_MORE_DATA != result){
        //     print_error("dpfpdd_capture()", result);
        //     return;
        // }
   
        // 
		// Identification(hReader,dpi);
		// CaptureFinger("any finger", hReader, dpi, DPFJ_FMD_ANSI_378_2004, &pFmd, &nFmdSize))

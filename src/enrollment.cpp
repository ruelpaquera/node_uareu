#include "enrollment.h"

#include "helpers.h"
#include "selection.h"
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

DPFPDD_DEV hReader = NULL; //handle of the selected reader
int dpi = 0;
char szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

// using namespace v8;
// using v8::FunctionTemplate;

// typedef struct __ENROLLFP_DATA__ {
//     uv_async_t async;
//     Nan::Persistent<Function> callback; 
//     int result;
//     unsigned char *pImage;
//     unsigned char *pFmd; 
// 	unsigned int nFmdSize = 0;
// } ENROLLFP_DATA;

// typedef struct __ENROLLFP_STOP__ {
//     uv_async_t async;
//     Nan::Persistent<Function> callback;
// } ENROLLFP_STOP;

// #define container_of(ptr, type, member) ({			\
// 	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
// 	(type *)( (char *)__mptr - offsetof(type,member) );})



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
#ifndef OLD_UV_RUN_SIGNATURE
void report_enrollfp_start(uv_async_t *handle)
#else
void report_enrollfp_start(uv_async_t *handle, int status)
#endif
{
    printf("report_enrollfp_start");
    ENROLLFP_DATA *FPdata = container_of(handle, ENROLLFP_DATA, async);
    Nan::HandleScope scope;
    
    if(!FPdata)
        return;
 
    Nan::Callback callback(Nan::New<Function>(FPdata->callback));
    Nan::AsyncResource asyncResource("verifyProgress");
    Local<Value> argv[3];
    argv[0] = Nan::New(FPdata->result);
    argv[1] = Nan::Null();

    argv[1] = Nan::New("fingerprintimg.c_str()").ToLocalChecked();
    
    callback.Call(2, argv, &asyncResource); 

    // if (loops == 5){
        printf("report_enrollfp_start");
        uv_close((uv_handle_t*)&FPdata->async,enrollfp_after);
    // } else {
    //     printf("report_enrollfp_start else");
    // } 
}
static void fpEnroll_start_cb(void *edata,int result,unsigned char *pImage,unsigned char *pFmd,unsigned int nFmdSize)
{
    ENROLLFP_DATA *fpdata = (ENROLLFP_DATA*)edata;

    if(!fpdata)
        return;

    printf("\ncallback called %d \n",fpdata->result);    

    fpdata->pImage = pImage;
    fpdata->pFmd = pFmd;
    fpdata->nFmdSize = nFmdSize;

    uv_async_send(&fpdata->async);  

}



NAN_METHOD(startEnroll)
{
    int result = 0;
    bool ret = false;
    ENROLLFP_DATA *FPdata;

    FPdata = new ENROLLFP_DATA;
    if(!FPdata)
        goto error;
    hReader = GetReader(szReader, sizeof(szReader),&dpi);
    if(NULL != hReader){
        // char szItem[MAX_DEVICE_NAME_LENGTH + 20]; 
        uv_async_init(uv_default_loop(), &FPdata->async, report_enrollfp_start);
        FPdata->callback.Reset(v8::Local<v8::Function>::Cast(info[0]));
	    char* vFingerName[5];    
        vFingerName[0] = const_cast<char*>("your thumb");
        vFingerName[1] = const_cast<char*>("your index finger");
        vFingerName[2] = const_cast<char*>("your middle finger");
        vFingerName[3] = const_cast<char*>("your ring finger");
        vFingerName[4] = const_cast<char*>("any finger");    

        fingerCapture(hReader,dpi,&fpEnroll_start_cb);

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
   
        // fpEnroll_start_cb((void*)FPdata);
		// Identification(hReader,dpi);
		// CaptureFinger("any finger", hReader, dpi, DPFJ_FMD_ANSI_378_2004, &pFmd, &nFmdSize))


    } else {
        goto error;
    }

    ret = true;
error:
    info.GetReturnValue().Set(Nan::New(ret));
    return;
}



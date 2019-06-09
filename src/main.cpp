#include "main.h"
#include "helpers.h"
#include "identify.h"
#include "selection.h"
 
#include <dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>

int initalized = -1;


DPFPDD_DEV _hReader = NULL; //handle of the selected reader
int _dpi = 0;
char _szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

void initModules(){
    sigset_t sigmask;
	sigfillset(&sigmask);
	pthread_sigmask(SIG_BLOCK, &sigmask, NULL);
	setlocale(LC_ALL, "");
    initalized  = dpfpdd_init();
    printf("\ninitalized %d" ,initalized);
    return;
}

NAN_METHOD(init)
{
    initModules();
    while(!initalized){
        printf("\nseek fp %d" ,initalized);
        if(initalized == 0){
            reader();
            break;
        }
    }
    info.GetReturnValue().Set(initalized == 0);
    return;
}

NAN_METHOD(startScan)
{
    Identification(_hReader,_dpi);
}

NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startScan);
    NAN_EXPORT(target, selectfpd);
}

NODE_MODULE(fingerprint, module_init)
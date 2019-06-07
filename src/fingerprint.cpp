#include "fingerprint.h"
#include "helpers.h"
#include "identify.h"
#include "selection.h"


#include <dpfpdd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <locale.h>
 
int initalized = -1;


void initModules(){
    sigset_t sigmask;
	sigfillset(&sigmask);
	pthread_sigmask(SIG_BLOCK, &sigmask, NULL);
	
	setlocale(LC_ALL, "");
    initalized  = dpfpdd_init();
    return;
}

NAN_METHOD(init)
{
    initModules();

    info.GetReturnValue().Set(initalized == 0);
    return;
}

NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startScan);
    NAN_EXPORT(target, selectfpd);
}

NODE_MODULE(fingerprint, module_init)
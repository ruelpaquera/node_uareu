#include "main.h"
#include "helpers.h"
#include "identify.h"
#include "selection.h"
 
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

#define MAX_MENU_STRING_LEN 120

#define MENU_TYPE_BACK 0x1
#define MENU_TYPE_EXIT 0x2

int initalized = -1;

static DPFPDD_DEV _hReader = NULL; //handle of the selected reader
static int _dpi = 0;
char _szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

NAN_METHOD(init)
{
    sigset_t sigmask;
	sigfillset(&sigmask);
	pthread_sigmask(SIG_BLOCK, &sigmask, NULL);
	setlocale(LC_ALL, "");
    initalized  = dpfpdd_init();
    if(DPFPDD_SUCCESS != initalized) print_error("dpfpdd_init()", initalized);
    info.GetReturnValue().Set(initalized == 0);
    return;
}
NAN_METHOD(getDevice)
{
    reader();
}
NAN_METHOD(startScan)
{
    // Identification(_hReader,_dpi);
}

NAN_MODULE_INIT(module_init){
    NAN_EXPORT(target, init);
    NAN_EXPORT(target, startScan);
    NAN_EXPORT(target, getDevice);
}

NODE_MODULE(fingerprint, module_init)
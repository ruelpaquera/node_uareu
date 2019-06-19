#include <nan.h>

#include "helpers.h"
#include <dpfpdd.h>

int fingerCapture(int *finger,void(*func)(void *edata,int result,unsigned char *pImage,unsigned char *pFmd,unsigned int nFmdSize),void *FPdata);//,void*FPdata
//int fingerCapture(DPFPDD_DEV hReaders, int dpi,void(void *edata,int result,unsigned char *pImage,unsigned char *pFmd,unsigned int nFmdSize));
// Nan::NAN_METHOD_RETURN_TYPE fingerCapture(Nan::NAN_METHOD_ARGS_TYPE info);
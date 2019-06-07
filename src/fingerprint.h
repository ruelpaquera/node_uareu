#include <nan.h> 


#include <dpfpdd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <locale.h>

extern int initalized;

Nan::NAN_METHOD_RETURN_TYPE init(Nan::NAN_METHOD_ARGS_TYPE info);
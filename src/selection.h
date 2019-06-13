#include <nan.h>

#include <dpfpdd.h>
// void reader();
DPFPDD_DEV GetReader(char* szReader, size_t nReaderLen, int *pDPI);

Nan::NAN_METHOD_RETURN_TYPE selectfpd(Nan::NAN_METHOD_ARGS_TYPE info);
Nan::NAN_METHOD_RETURN_TYPE openDevice(Nan::NAN_METHOD_ARGS_TYPE info);

#include "fingerprint.h"
#include "selection.h"  
#include "helpers.h"

#include <dpfpdd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


DPFPDD_DEV GetReader(char* szReader, size_t nReaderLen, int *pDPI){
	DPFPDD_DEV hReader = NULL;
	strncpy(szReader, "", nReaderLen);
	int bStop = 0;

    while(!bStop){
        unsigned int nReaderCnt = 1;
		DPFPDD_DEV_INFO* pReaderInfo = (DPFPDD_DEV_INFO*)malloc(sizeof(DPFPDD_DEV_INFO) * nReaderCnt);
        while(NULL != pReaderInfo){
			unsigned int i = 0;
			for(i = 0; i < nReaderCnt; i++){
				pReaderInfo[i].size = sizeof(DPFPDD_DEV_INFO); 
			}  
        }
    }
	return hReader;
}


NAN_METHOD(selectfpd) {



    info.GetReturnValue().Set(Nan::New("This is a thing.").ToLocalChecked());
}
// #include "fingerprint.h"
#include "helpers.h"
#include "selection.h"  

// #include <dpfpdd.h>

// #include <stdlib.h>
// #include <stdio.h>
// #include <errno.h>
// #include <string.h>

DPFPDD_DEV hReader = NULL; //handle of the selected reader
int dpi = 0;
char szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

DPFPDD_DEV GetReader(char* szReader, size_t nReaderLen, int *pDPI){
	// DPFPDD_DEV hReader = NULL;
	strncpy(szReader, "", nReaderLen);
	int bStop = 0;
	int nChoice = 0;
    while(!bStop){
        unsigned int nReaderCnt = 1;
		DPFPDD_DEV_INFO* pReaderInfo = (DPFPDD_DEV_INFO*)malloc(sizeof(DPFPDD_DEV_INFO) * nReaderCnt);
        while(NULL != pReaderInfo){
			unsigned int i = 0;
			for(i = 0; i < nReaderCnt; i++){
				pReaderInfo[i].size = sizeof(DPFPDD_DEV_INFO); 
			} 
			
			unsigned int nNewReaderCnt = nReaderCnt;
			int result = dpfpdd_query_devices(&nNewReaderCnt, pReaderInfo); 
			
			if(DPFPDD_SUCCESS != result && DPFPDD_E_MORE_DATA != result){
				print_error("dpfpdd_query_devices()", result);
				free(pReaderInfo);
				pReaderInfo = NULL;
				nReaderCnt = 0;
				break;
			}

			if(DPFPDD_E_MORE_DATA == result){
				DPFPDD_DEV_INFO* pri = (DPFPDD_DEV_INFO*)realloc(pReaderInfo, sizeof(DPFPDD_DEV_INFO) * nNewReaderCnt);
				if(NULL == pri){
					print_error("realloc()", ENOMEM);
					break;
				}
				pReaderInfo = pri;
				nReaderCnt = nNewReaderCnt;
				continue;
			}

			printf("\nloop nested while");
			nReaderCnt = nNewReaderCnt;
			break;
        }
		
		if(0 != nReaderCnt){
			printf("\n\nAvailable readers:\n");
			unsigned int i = 0;
			for(i = 0; i < nReaderCnt; i++){
				if(std::to_string(pReaderInfo[i].id.product_id) == "000a" && std::to_string(pReaderInfo[i].id.vendor_id) == "05ba" ){
					nChoice = i;
					printf(" loop   %s\n", pReaderInfo[i].name);
					// break;
				}
			}

			printf("  %s\n", pReaderInfo[i].name);
			printf("\n");
			printf("Selected reader:  %s\n", pReaderInfo[nChoice].name);
			printf("Vendor name:      %s\n", pReaderInfo[nChoice].descr.vendor_name);
			printf("Product name:     %s\n", pReaderInfo[nChoice].descr.product_name);
			printf("Serial number:    %s\n", pReaderInfo[nChoice].descr.serial_num);
			printf("USB VID:          %04x\n", pReaderInfo[nChoice].id.vendor_id);
			printf("USB PID:          %04x\n", pReaderInfo[nChoice].id.product_id);
			printf("USB BCD revision: %04x\n", pReaderInfo[nChoice].ver.bcd_rev);
			printf("HW version:       %d.%d.%d\n", pReaderInfo[nChoice].ver.hw_ver.major, pReaderInfo[nChoice].ver.hw_ver.minor, pReaderInfo[nChoice].ver.hw_ver.maintenance);
			printf("FW version:       %d.%d.%d\n", pReaderInfo[nChoice].ver.fw_ver.major, pReaderInfo[nChoice].ver.fw_ver.minor, pReaderInfo[nChoice].ver.fw_ver.maintenance);
			
			int result = dpfpdd_open(pReaderInfo[nChoice].name, &hReader);
			if(DPFPDD_SUCCESS == result){
				strncpy(szReader, pReaderInfo[nChoice].name, nReaderLen);
				printf("\n %s is open \n", pReaderInfo[i].name);
			}
			else
				printf("\n %s can't open \n", pReaderInfo[i].name);
			break;
		}
		else printf("\n\nNo readers available\n");
    } 

	//return nChoice;
}

void reader(){
	hReader = NULL;
	hReader = GetReader(szReader, sizeof(szReader),&dpi);
	if(NULL != hReader){
		char szItem[MAX_DEVICE_NAME_LENGTH + 20]; 
		snprintf(szItem, sizeof(szItem), "Select new reader (selected: %s)", szReader);
		printf("\nSelect new reader (selected: %s)", szReader); 
	}
	//return true;
}

NAN_METHOD(selectfpd) {
	reader();
    info.GetReturnValue().Set(Nan::New(true));
}
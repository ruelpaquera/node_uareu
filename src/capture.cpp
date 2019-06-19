#include "helpers.h"
#include "capture.h"
#include "selection.h"
// #include "enrollment.h"
// #include "identify.h"
// #include "enrollment.h"

#include <unistd.h>
DPFPDD_DEV hReaders = NULL; //handle of the selected reader
int dpi = 0;
char szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

//,void(*func)(void *,int *,unsigned char *,unsigned char *,unsigned int *),void *FPdata
int fingerCapture(int *finger,fpEnroll_start_cb_ func,void *FPdata){ 
	if(hReaders == NULL) 
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);

	// ENROLLFP_DATA *fpdata = (ENROLLFP_DATA*)FPdata; 
	// const int nFingerCnt = 5;
	unsigned char* vFmd;
	unsigned int vFmdSize;
	// char* vFingerName;    
    int i = *finger; 
		vFmd = NULL;
		vFmdSize = 0; 

    int result = dpfpdd_led_config(hReaders, DPFPDD_LED_ACCEPT | DPFPDD_LED_REJECT, DPFPDD_LED_CLIENT, NULL);
	if(DPFPDD_SUCCESS != result && DPFPDD_E_NOT_IMPLEMENTED != result){
		print_error("dpfpdd_led_config()", result);
	}
	int bStop = 0;
    while(!bStop){
        //capture fingers
		// fpdata->pFmd = vFmd;
		// fpdata->nFmdSize = vFmdSize;
		bStop = CaptureFinger(const_cast<char*>("your thumb"), hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize);
 		printf("\nfingerCapture %d",bStop);
		printf("\nfingerCapture finger %d",i);
 
		// printf("\nfingerCapture pdata->result %d",fpdata->result);

		//if(bStop == 0){
			// func(fpdata,123,NULL,vFmd,vFmdSize); 
			vFmd = NULL;
			vFmdSize = 0;
			i++;
			*finger = i; 
			// fingerCapture(finger,func,FPdata);
			break;
		//}
    }
	return bStop;
}
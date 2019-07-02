#include "helpers.h"
#include "capture.h"
#include "selection.h"
// #include "enrollment.h"
// #include "identify.h"
// #include "enrollment.h"

#include <unistd.h>
DPFPDD_DEV hReaders = NULL; //handle of the selected reader
int dpi = 100;
char szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

//,void(*func)(void *,int *,unsigned char *,unsigned char *,unsigned int *),void *FPdata
int fingerCapture(int *finger,fpEnroll_start_cb_ func,void *FPdata){ 
	const int nFingerCnt = *finger;
	unsigned char* vFmd;
	unsigned int vFmdSize; 
	unsigned char* ppImage;
	int i = 0;
	if(hReaders == NULL) 
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
		
	ENROLLFP_DATA *fpdata = (ENROLLFP_DATA*)FPdata; 

  	// int result = dpfpdd_led_config(hReaders, DPFPDD_LED_ACCEPT | DPFPDD_LED_REJECT, DPFPDD_LED_CLIENT, NULL);
	// if(DPFPDD_SUCCESS != result && DPFPDD_E_NOT_IMPLEMENTED != result){
	// 	print_error("dpfpdd_led_config()", result);
	// }
	int bStop = 0;
	while(!bStop){   
		if(i == nFingerCnt){
			bStop = 1;
			break;
		}
		bStop = CaptureFinger(hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize,&ppImage);
		
		if(!bStop){
			
			fpdata->pFmd = vFmd;
			fpdata->nFmdSize = vFmdSize;
			fpdata->pImage = ppImage;
			fpdata->finger = i;




		

			func(fpdata);


			if(NULL != vFmd) free(vFmd);
			// if(NULL != ppImage) free(ppImage);
			ppImage = NULL;
			vFmd = NULL;
			vFmdSize = 0; 

		}
		i++;
	}
	dpfpdd_close(hReaders);
	return bStop;
}
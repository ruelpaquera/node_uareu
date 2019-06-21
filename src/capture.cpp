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
int fingerCapture(int *finger,int countFinger,fpEnroll_start_cb_ func,void *FPdata){ 
	const int nFingerCnt = countFinger;
	unsigned char* vFmd;
	unsigned int vFmdSize;
	// char* vFingerName;
	unsigned char* ppImage;
	int i = 0;
	// const int nFingerCnt = countFinger;
	// unsigned char* vFmd[nFingerCnt];
	// unsigned int vFmdSize[nFingerCnt];
	// char* vFingerName[nFingerCnt];
	// unsigned char* ppImage[nFingerCnt];
	// int i = 0;
	// for(i = 0; i < nFingerCnt; i++){
	// 	vFmd[i] = NULL;
	// 	vFmdSize[i] = 0;
	// }

	// vFingerName[0] = const_cast<char*>("thumb");
	// vFingerName[1] = const_cast<char*>("index");
	// vFingerName[2] = const_cast<char*>("middle");
	// vFingerName[3] = const_cast<char*>("ring");
	// vFingerName[4] = const_cast<char*>("any"); 

	if(hReaders == NULL) 
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);

	ENROLLFP_DATA *fpdata = (ENROLLFP_DATA*)FPdata; 

  int result = dpfpdd_led_config(hReaders, DPFPDD_LED_ACCEPT | DPFPDD_LED_REJECT, DPFPDD_LED_CLIENT, NULL);
	if(DPFPDD_SUCCESS != result && DPFPDD_E_NOT_IMPLEMENTED != result){
		print_error("dpfpdd_led_config()", result);
	}
	int bStop = 0;
    while(!bStop){
      // for(i = 0; i < nFingerCnt; i++){
			// 	if(0 == CaptureFinger(vFingerName, hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize,&ppImage)) continue;
			// 	bStop = 1;
			// 	break;
			// } 
			bStop = CaptureFinger(hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize,&ppImage);
			if(i == nFingerCnt || bStop){
				bStop = 1;
				break;
			}
			if(!bStop){
				printf("\n ppImage %s",ppImage);
				printf("\n Fmd %s",vFmd);  

				fpdata->pFmd = vFmd;
				fpdata->nFmdSize = vFmdSize;
				fpdata->pImage = ppImage;
				fpdata->finger = i;
				func(fpdata);
			}
 
			// for(int i = 0; i < nFingerCnt; i++){
				if(NULL != vFmd) free(vFmd);
				// if(NULL != ppImage[i]) free(ppImage[i]);
				ppImage = NULL;
				vFmd = NULL;
				vFmdSize = 0; 
				i++;
			// }
    }
	dpfpdd_close(hReaders);
	return bStop;
}
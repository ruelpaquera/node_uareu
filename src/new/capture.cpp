#include "helpers.h"
#include "capture.h"
#include "selection.h"
#include "base64.h" 

#include <unistd.h>
#include <fstream>

DPFPDD_DEV hReaders = NULL; //handle of the selected reader
int dpi = 0;
char szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader

int fingerCapture(int *finger,fpEnroll_start_cb_ func,void *FPdata){ 
	const int nFingerCnt = *finger;
	unsigned char* vFmd;
	unsigned int vFmdSize; 
	unsigned char* ppImage;
	int i = 0;
	if(hReaders == NULL) 
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
		
	ENROLLFP_DATA *fpdata = (ENROLLFP_DATA*)FPdata; 
	int bStop = 0;

	unsigned int nImageSize ;
	
	while(!bStop){   

		if(i == nFingerCnt){
			bStop = 1;
			break;
		}
		bStop = CaptureFinger(hReaders, dpi, DPFJ_FMD_ISO_19794_2_2005, &vFmd, &vFmdSize,&ppImage,&nImageSize);
		
		if(!bStop){
			fpdata->pFmd = vFmd;
			fpdata->nFmdSize = vFmdSize;
			fpdata->pImage = ppImage;
			fpdata->pImageSize = nImageSize;
			fpdata->finger = i;

    		printf("\n size %d \n",nImageSize);

			func(fpdata);
			if(NULL != vFmd) free(vFmd);
			ppImage = NULL; 
			vFmd = NULL;
			vFmdSize = 0;  

			i++;
		}

	}

	dpfpdd_close(hReaders);
	return bStop;
}

int Capture(fpScanning_cb_ func,void *FPdata){ 
	unsigned char* vFmd;
	unsigned int vFmdSize; 
	unsigned char* ppImage;
	int i = 0; 
	if(hReaders == NULL) {
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
	} else {
		dpfpdd_init();
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
	}
		
	SCANING_HANDLER *fpdata = (SCANING_HANDLER*)FPdata; 
	int bStop = 0; 
	unsigned int nImageSize ; 
	while(!bStop){  
		if(i == 1){
			bStop = 1;
			break;
		}
		bStop = CaptureFinger_2(hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize,&ppImage,&nImageSize,5000);
 
		if(!bStop){ 
			fpdata->pFmd = vFmd;
			fpdata->nFmdSize = vFmdSize;
			fpdata->pImage = ppImage;
			fpdata->pImageSize = nImageSize;

			func(fpdata); 
			ppImage = NULL; 
			vFmd = NULL;
			vFmdSize = 0;
			i++;			 
		}
	}  
	dpfpdd_close(hReaders);
	hReaders = NULL;
	return bStop;
}

int CaptureVerify(int *finger,fpVerify_start_cb_ func,void *FPdata, int timeout){ 
	const int nFingerCnt = *finger;
	unsigned char* vFmd;
	unsigned int vFmdSize; 
	unsigned char* ppImage;
	int i = 0; 
	if(hReaders == NULL) {
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
	} else {
		dpfpdd_init();
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
	}
		
	
	VERIFYFD_DATA *fpdata = (VERIFYFD_DATA*)FPdata; 
	int bStop = 0; 
	unsigned int nImageSize ; 
	while(!bStop){  
		if(i == nFingerCnt){
			bStop = 1;
			break;
		} 
		bStop = CaptureFinger_2(hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize,&ppImage,&nImageSize,timeout);
 
		if(!bStop){ 
			fpdata->pFmd2 = vFmd;
			fpdata->nFmdSize2 = vFmdSize;
			fpdata->pImage = ppImage;
			fpdata->pImageSize = nImageSize;	
	
			// printf("\n-----------------------------"); 
			// printf("\npFeatures %x",vFmd); 
			// printf("\nFeaturesSize %d",nImageSize); 
			// printf("\npImage %u \n",ppImage); 
			// printf("\npImage %c \n",ppImage); 
			// printf("\n-----------------------------"); 

			func(fpdata); 
			ppImage = NULL; 
			vFmd = NULL;
			vFmdSize = 0;
			i++;
			 
		}
	}  
	dpfpdd_close(hReaders);
	hReaders = NULL;
	return bStop;
}

int CaptureStop(fpVerify_start_cb_ func,void *FPdata){

	VERIFYFD_STOP *fpdata = (VERIFYFD_STOP*)FPdata; 
	if(hReaders == NULL) 
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
	
	func(fpdata);
	if(NULL != hReaders) {
		dpfpdd_cancel(hReaders);
		dpfpdd_close(hReaders);
		dpfpdd_exit();
		// dpfpdd_init();
	}
	
	return 0;
}

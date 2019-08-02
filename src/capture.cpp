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
		bStop = CaptureFinger(hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize,&ppImage,&nImageSize);
		
		if(!bStop){
			fpdata->pFmd = vFmd;
			fpdata->nFmdSize = vFmdSize;
			fpdata->pImage = ppImage;
			fpdata->pImageSize = nImageSize;
			fpdata->finger = i;

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
int CaptureVerify(int *finger,fpVerify_start_cb_ func,void *FPdata){ 
	const int nFingerCnt = *finger;
	unsigned char* vFmd;
	unsigned int vFmdSize; 
	unsigned char* ppImage;
	int i = 0;
	if(hReaders == NULL) 
		hReaders = GetReader(szReader, sizeof(szReader),&dpi);
		
	VERIFYFD_DATA *fpdata = (VERIFYFD_DATA*)FPdata; 
	int bStop = 0;

	unsigned int nImageSize ;

	while(!bStop){
		printf("\n------CaptureVerify-----\n");
		if(i == nFingerCnt){
			bStop = 1;
			break;
		}
		bStop = CaptureFinger_(hReaders, dpi, DPFJ_FMD_ISO_19794_2_2005, &vFmd, &vFmdSize,&ppImage,&nImageSize);
		if(0 == bStop)  bStop = CaptureFinger_(hReaders, dpi, DPFJ_FMD_ISO_19794_2_2005, &vFmd, &vFmdSize,&ppImage,&nImageSize);
		if(0 == bStop) {
			printf("\n CaptureVerify %p \n",fpdata->pFmd1);
			fpdata->pFmd2 = vFmd;
			fpdata->nFmdSize1 = vFmdSize;
			fpdata->pImage = ppImage;
			fpdata->pImageSize = nImageSize;

			int result = verifyFP(
				vFmd,
				vFmd,
				fpdata->nFmdSize1,
				fpdata->nFmdSize1
			);
			
			func(fpdata);
			if(NULL != vFmd) free(vFmd);
			ppImage = NULL; 
			vFmd = NULL;
			vFmdSize = 0;
			i++;
		}else
			bStop = 1;
		if(!bStop){
			// printf("\n CaptureVerify %p \n",fpdata->pFmd1);
			// fpdata->pFmd2 = vFmd;
			// fpdata->nFmdSize1 = vFmdSize;
			// fpdata->pImage = ppImage;
			// fpdata->pImageSize = nImageSize;

			// int result = verifyFP(
			// 	vFmd,
			// 	vFmd,
			// 	fpdata->nFmdSize1,
			// 	fpdata->nFmdSize1
			// );
			
			// func(fpdata);
			// if(NULL != vFmd) free(vFmd);
			// ppImage = NULL; 
			// vFmd = NULL;
			// vFmdSize = 0;
			// i++;
		}
	}
	printf("\n------CaptureVerify close-----\n");
	dpfpdd_close(hReaders);
	return bStop;
}
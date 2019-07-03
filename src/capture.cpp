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
	int comstart = dpfj_start_compression();
	if(comstart != DPFJ_SUCCESS){
 		print_error("dpfpdd_capture()", comstart);
	}else{
		printf("\nstarted compression\n");
	}
	int setwsq = dpfj_set_wsq_bitrate(4.19,100);
	if(setwsq != DPFJ_SUCCESS){
 		print_error("dpfpdd_capture()", setwsq);
	}else{
		printf("\nsuccess dpfj_set_wsq_bitrate\n");
	}
	

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

			// int compressfid = dpfj_compress_fid(DPFJ_FID_ISO_19794_4_2005,ppImage,vFmdSize,DPFJ_COMPRESSION_WSQ_NIST);
			// if(compressfid != DPFJ_SUCCESS){
			// 	print_error("dpfj_compress_fid()", setwsq);
			// }else{
			// 	printf("\nsuccess dpfj_compress_fid\n");
			// }

		

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
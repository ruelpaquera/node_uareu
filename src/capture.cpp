#include "helpers.h"
#include "capture.h"
#include "selection.h"

#include <unistd.h>
DPFPDD_DEV hReaders = NULL; //handle of the selected reader
int dpi = 100;
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


	unsigned char pImage  ;
	unsigned int nImageSize ;
	
	while(!bStop){   

		if(i == nFingerCnt){
			bStop = 1;
			break;
		}
		bStop = CaptureFinger(hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd, &vFmdSize,&ppImage,&nImageSize);
		
		if(!bStop){
			// for(int xx = 0;xx < nImageSize;xx++){
			// 	printf(" %x ",ppImage[xx]);
			// }
			// int comstart = dpfj_start_compression();
			// if(comstart != DPFJ_SUCCESS){
			// 	print_error("dpfpdd_capture()", comstart);
			// }else{
			// 	printf("\nstarted compression\n");
			// }
			// int setwsq = dpfj_set_wsq_bitrate(4.19,100);
			// if(setwsq != DPFJ_SUCCESS){
			// 	print_error("dpfpdd_capture()", setwsq);
			// }else{
			// 	printf("\nsuccess dpfj_set_wsq_bitrate\n");
			// }
			// int cloop = 0;

			// int compressfid = dpfj_compress_fid(DPFJ_FID_ISO_19794_4_2005,vFmd,vFmdSize,DPFJ_COMPRESSION_WSQ_NIST);
			
			
			// while(!bStop){				
			// 	if(compressfid == DPFJ_SUCCESS){
			// 		printf("\ncompressfid %d\n",compressfid);
			// 		break;
			// 	}else if(compressfid == DPFJ_E_COMPRESSION_NOT_STARTED){
			// 		printf("\nfail DPFJ_E_COMPRESSION_NOT_STARTED\n");
			// 	}else if(compressfid == DPFJ_E_COMPRESSION_INVALID_WSQ_PARAMETER){
			// 		printf("\nfail DPFJ_E_COMPRESSION_INVALID_WSQ_PARAMETER\n");
			// 	}else if(compressfid == DPFJ_E_COMPRESSION_WSQ_LIB_NOT_FOUND){
			// 		printf("\nfail DPFJ_E_COMPRESSION_WSQ_LIB_NOT_FOUND\n");
			// 	}else if(compressfid == DPFJ_E_COMPRESSION_WSQ_FAILURE){
			// 		printf("\nfail DPFJ_E_COMPRESSION_WSQ_FAILURE\n");
			// 	}else {
			// 		print_error("dpfj_compress_fid()", compressfid);
			// 	} 
			// }

			// printf("\nsuccess dpfj_compress_fid\n");
			// int getdata = dpfj_get_processed_data(&pImage,&nImageSize);
			// if(getdata != DPFJ_SUCCESS ){
			// 	print_error("dpfj_get_processed_data()", getdata);
			// 	break;
			// } else {
			// 	printf("\ndpfj_get_processed_data pImage %* \n",pImage);
			// 	printf("\ndpfj_get_processed_data nImageSize %* \n",nImageSize);

				fpdata->pFmd = vFmd;
				fpdata->nFmdSize = vFmdSize;
				fpdata->pImage = ppImage;
				fpdata->finger = i;

				func(fpdata);
				if(NULL != vFmd) free(vFmd);
				ppImage = NULL; 
				vFmd = NULL;
				vFmdSize = 0; 
				
				// dpfj_finish_compression();

				i++;
			// 	cloop = getdata;
			// }

		}
	}

	dpfpdd_close(hReaders);
	return bStop;
}
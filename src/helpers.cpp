/* 
 * Copyright (C) 2011, Digital Persona, Inc.
 *
 * This file is a part of sample code for the UareU SDK 2.x.
 */


#include <dpfpdd.h>
#include <dpfj.h>
#include <dpfj_compression.h>
#include <dpfj_quality.h>

#include "helpers.h" 
#include "base64.h" 

#include <fstream>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h> 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// error handling

void print_error(const char* szFunctionName, int nError){
	char sz[256];
	if(_DP_FACILITY == (nError >> 16)){
		char* szError = NULL;
		switch(nError){
		case DPFPDD_E_NOT_IMPLEMENTED: szError = const_cast<char*>("API call is not implemented."); break;
		case DPFPDD_E_FAILURE: szError = const_cast<char*>("Unspecified failure."); break;
		case DPFPDD_E_NO_DATA: szError = const_cast<char*>("No data is available."); break;
		case DPFPDD_E_MORE_DATA: szError = const_cast<char*>("The memory allocated by the application is not big enough for the data which is expected."); break;
		case DPFPDD_E_INVALID_PARAMETER: szError = const_cast<char*>("One or more parameters passed to the API call are invalid."); break;
		case DPFPDD_E_INVALID_DEVICE: szError = const_cast<char*>("Reader handle is not valid."); break;
		case DPFPDD_E_DEVICE_BUSY: szError = const_cast<char*>("The API call cannot be completed because another call is in progress."); break;
		case DPFPDD_E_DEVICE_FAILURE: szError = const_cast<char*>("The reader is not working properly."); break;
		case DPFJ_E_INVALID_FID: szError = const_cast<char*>("FID is invalid."); break;
		case DPFJ_E_TOO_SMALL_AREA: szError = const_cast<char*>("Image is too small."); break;
		case DPFJ_E_INVALID_FMD: szError = const_cast<char*>("FMD is invalid."); break;
		case DPFJ_E_ENROLLMENT_IN_PROGRESS: szError = const_cast<char*>("Enrollment operation is in progress."); break;
		case DPFJ_E_ENROLLMENT_NOT_STARTED: szError = const_cast<char*>("Enrollment operation has not begun."); break;
		case DPFJ_E_ENROLLMENT_NOT_READY: szError = const_cast<char*>("Not enough in the pool of FMDs to create enrollment FMD."); break;
		case DPFJ_E_ENROLLMENT_INVALID_SET: szError = const_cast<char*>("Unable to create enrollment FMD with the collected set of FMDs."); break;
		case DPFJ_E_COMPRESSION_IN_PROGRESS: szError = const_cast<char*>("Compression or decompression operation is in progress"); break;
		case DPFJ_E_COMPRESSION_NOT_STARTED: szError = const_cast<char*>("Compression or decompression operation was not started."); break;
		case DPFJ_E_COMPRESSION_INVALID_WSQ_PARAMETER: szError = const_cast<char*>("One or more parameters passed for WSQ compression are invalid."); break;
		case DPFJ_E_COMPRESSION_WSQ_FAILURE: szError = const_cast<char*>("Unspecified error during WSQ compression or decompression."); break;
		case DPFJ_E_COMPRESSION_WSQ_LIB_NOT_FOUND: szError = const_cast<char*>("Library for WSQ compression is not found or not built-in."); break;
		case DPFJ_E_QUALITY_NO_IMAGE: szError = const_cast<char*>("Image is invalid or absent."); break;
		case DPFJ_E_QUALITY_TOO_FEW_MINUTIA: szError = const_cast<char*>("Too few minutia detected in the fingerprint image."); break;
		case DPFJ_E_QUALITY_FAILURE: szError = const_cast<char*>("Unspecified error during execution."); break;
		case DPFJ_E_QUALITY_LIB_NOT_FOUND: szError = const_cast<char*>("Library for image quality is not found or not built-in."); break;
		}
		sprintf(sz, "%s returned DP error: 0x%x \n%s", szFunctionName, (0xffff & nError), szError);
	}
	else{
		sprintf(sz, "%s returned system error: 0x%x", szFunctionName, (0xffff & nError));
	}
	printf("%s \n\n", sz);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// capture

DPFPDD_DEV g_hReader = NULL;

void signal_handler(int nSignal) {
	if(SIGINT == nSignal){
		//cancel capture
		if(NULL != g_hReader) dpfpdd_cancel(g_hReader);
	}
}

int CaptureFinger(DPFPDD_DEV hReader, int dpi, DPFJ_FMD_FORMAT nFtType, unsigned char** ppFt, unsigned int* pFtSize,unsigned char **ppImage){
	int result = 0; 
	*ppFt = NULL;
	*pFtSize = 0;
	const char* szFingerName = "any"; 
	//prepare capture parameters and result
	DPFPDD_CAPTURE_PARAM cparam = {};
	cparam.size = sizeof(cparam);
	cparam.image_fmt = DPFPDD_IMG_FMT_ISOIEC19794;
	cparam.image_proc = DPFPDD_IMG_PROC_NONE;
	cparam.image_res = dpi;
	
	DPFPDD_CAPTURE_RESULT cresult = {};
	cresult.size = sizeof(cresult);
	cresult.info.size = sizeof(cresult.info);
	
	//get size of the image
	unsigned int nOrigImageSize = 0;
	result = dpfpdd_capture(hReader, &cparam, 0, &cresult, &nOrigImageSize, NULL);
	if(DPFPDD_E_MORE_DATA != result){
		print_error("dpfpdd_capture()", result);
		return result;
	}
	//unsigned char* pImage = (unsigned char*)malloc(nOrigImageSize);
	unsigned char* pImage = (unsigned char*)malloc(nOrigImageSize);
	if(NULL == pImage){
		print_error("malloc()", ENOMEM); 
		return ENOMEM;
	}
		
	//set signal handler 
	g_hReader = hReader;
	
	struct sigaction new_action, old_action;
	new_action.sa_handler = &signal_handler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;
	sigaction(SIGINT, &new_action, &old_action);

	//unblock SIGINT (Ctrl-C)
	sigset_t new_sigmask, old_sigmask;
	sigemptyset(&new_sigmask);
	sigaddset(&new_sigmask, SIGINT);
	pthread_sigmask(SIG_UNBLOCK, &new_sigmask, &old_sigmask);
	
	while(1){ 
		//wait until ready
		int is_ready = 0;
		unsigned int nImageSize = nOrigImageSize;
		while(1){
			DPFPDD_DEV_STATUS ds;
			ds.size = sizeof(DPFPDD_DEV_STATUS);
			result = dpfpdd_get_device_status(hReader, &ds);
			if(DPFPDD_SUCCESS != result){
				print_error("dpfpdd_get_device_status()", result);
				break;
			}
			
			if(DPFPDD_STATUS_FAILURE == ds.status){
				print_error("Reader failure", DPFPDD_STATUS_FAILURE);
				result = DPFPDD_STATUS_FAILURE;
				break;
			}
			if(DPFPDD_STATUS_READY == ds.status || DPFPDD_STATUS_NEED_CALIBRATION == ds.status){
				is_ready = 1;
				break;
			}
		}
		if(!is_ready) break;

		//capture fingerprint 
		printf("\nloop Put %s on the reader, or press Ctrl-C to cancel...\r\n", szFingerName);
		// printf("\n1pImage %p\n",pImage);
		// printf("\n1nImageSize %d\n",nImageSize);
		result = dpfpdd_capture(hReader, &cparam, -1, &cresult, &nImageSize, pImage); 

		printf("\result %d\n",result);
		if(DPFPDD_SUCCESS != result){
			print_error("dpfpdd_capture()", result);
		}
		else{ 
			if(cresult.success){ 
					
				// printf("\nDPFPDD_QUALITY %d \n",cresult.quality);
				// printf("\ncresult.score %d \n",cresult.score);
				// printf("\ncresult.size %d \n",cresult.size);
				// printf("\ncresult.info.width %d \n",cresult.info.width);
				// printf("\ncresult.info.height %d \n",cresult.info.height);
				// printf("\ncresult.info.res %d \n",cresult.info.res);
				// printf("\ncresult.info.bpp %d \n",cresult.info.bpp);
				// printf("\ncresult.info.size %d \n",cresult.info.size);

				unsigned char pImage2;
				unsigned int nImageSize2;
				// pImage2 = NULL;
				// int dpfidcon = dpfj_dp_fid_convert(pImage,nImageSize,DPFJ_FID_ANSI_381_2004,dpi,0,&pImage2,&nImageSize2);
				// if(DPFJ_SUCCESS == dpfidcon){
				// 	printf("dpfj_dp_fid_convert success");
				// }else {
				// 	printf("dpfj_dp_fid_convert Failed");
				// }
				
				int rawcon = dpfj_raw_convert();

				// printf("\ndpfj_get_processed_data pImage2 %s\n",pImage2);

				// *ppImage = pImage;
				//get max size for the feature template
 
				unsigned int nFeaturesSize = MAX_FMD_SIZE;
				unsigned char* pFeatures = (unsigned char*)malloc(nFeaturesSize);
				if(NULL == pFeatures){
					print_error("malloc()", ENOMEM); 
					result = ENOMEM;
				}
				else{ 
					

					// printf("\ndpfj_get_processed_data nImageSize2 %s\n",nImageSize2);
					// printf("\ncresult.info.width %d \n",cresult.info.width);
					// printf("\ncresult.info.height %d \n",cresult.info.height);
					// printf("\ncresult.info.res %d \n",cresult.info.res);
					// printf("\ncresult.info.bpp %d \n",cresult.info.bpp);
					// printf("\ncresult.info.size %d \n",cresult.info.size);

					// int compress = dpfj_compress_raw(pImage, cresult.info.size, cresult.info.width, cresult.info.height, dpi, cresult.info.bpp,DPFJ_COMPRESSION_WSQ_NIST);
					
					// int setwsq = dpfj_set_wsq_size(nImageSize,100);
					// if(setwsq == DPFJ_SUCCESS){
					// 	printf("\ndpfj_set_wsq_size success\n");
					// }else if(setwsq == DPFJ_E_FAILURE){
					// 	printf("\ndpfj_set_wsq_size fail\n");
					// }
					// int scompress = dpfj_start_compression();
					// if(scompress == DPFJ_SUCCESS){
					// 	printf("compress start success");
					// }else if(scompress == DPFJ_E_COMPRESSION_IN_PROGRESS){
					// 	printf("compress start in progress");
					// }else {
					// 	printf("compress FAILURE");
					// }
					// int setbitrate = dpfj_set_wsq_bitrate(90,0);
					// if(setbitrate == DPFJ_SUCCESS){
					// 	printf("\ndpfj_set_wsq_bitrate success\n");
					// }else if(setbitrate == DPFJ_E_FAILURE){
					// 	printf("\ndpfj_set_wsq_bitrate fail\n");
					// }
					// int compress = dpfj_compress_fid(DPFJ_FID_ISO_19794_4_2005,pImage,nImageSize,DPFJ_COMPRESSION_WSQ_AWARE);
					// printf("\ndpfj_compress_fid compress %d \n",compress);
					// pImage2 = NULL;
					// nImageSize = NULL;
					// dpfj_get_processed_data(&pImage2,&nImageSize2);

					// printf("\ndpfj_get_processed_data pImage2 %s\n",pImage2);
					// // printf("\ndpfj_get_processed_data nImageSize2 %s\n",nImageSize2);

					// int ecompress = dpfj_finish_compression();
					// if(ecompress == DPFJ_SUCCESS){
					// 	printf("\ncompress finish success\n");
					// }else{
					// 	printf("\ncompress in FAILURE\n");
					// }

					*ppImage = pImage;
					printf("\npImage2 %s\n",pImage);// DPFJ_FID_ANSI_381_2004 DPFJ_FID_ISO_19794_4_2005
					printf("\n2nImageSize %d\n",nImageSize);
					// result = dpfj_create_fmd_from_fid(DPFJ_FID_ISO_19794_4_2005, pImage, nImageSize, nFtType, pFeatures, &nFeaturesSize);
 					// if(DPFJ_SUCCESS == result){ 
						*ppFt = pFeatures;
						*pFtSize = nFeaturesSize;	

					// // 	*ppImage = pImage;  
					// // 	printf("\npFeatures %s",pFeatures);
					// // 	printf("\nFeaturesSize %d\n",nFeaturesSize);
					// // 	printf("\npImage %s\n",pImage);
					// 	// printf("features extracted (%ldms).\n\n", mseconds); 
					// }
					// else{
					// 	print_error("dpfj_create_fmd_from_fid()", result); 
					// 	free(pFeatures);
					// }
				}
			}
			else if(DPFPDD_QUALITY_CANCELED == cresult.quality){
				//capture canceled
				result = EINTR;
			}
			else{
				//bad capture
				printf("    bad capture, quality feedback: 0x%x\n", cresult.quality);
				unsigned int i = 0;
				for(i = 1; i < 0x80000000; i <<= 1){
					switch(cresult.quality & i){
					case 0: break;
					case DPFPDD_QUALITY_TIMED_OUT:            printf("    timeout expired \n"); break;
					case DPFPDD_QUALITY_CANCELED:             printf("    capture was canceled \n"); break;
					case DPFPDD_QUALITY_NO_FINGER:            printf("    not a finger detected \n"); break;
					case DPFPDD_QUALITY_FAKE_FINGER:          printf("    fake finger detected \n"); break;
					case DPFPDD_QUALITY_FINGER_TOO_LEFT:      printf("    finger is too far left on the reader \n"); break;
					case DPFPDD_QUALITY_FINGER_TOO_RIGHT:     printf("    finger is too far right on the reader \n"); break;
					case DPFPDD_QUALITY_FINGER_TOO_HIGH:      printf("    finger is too high on the reader \n"); break;
					case DPFPDD_QUALITY_FINGER_TOO_LOW:       printf("    finger is too low in the reader \n"); break;
					case DPFPDD_QUALITY_FINGER_OFF_CENTER:    printf("    finger is not centered on the reader \n"); break;
					case DPFPDD_QUALITY_SCAN_SKEWED:          printf("    scan is skewed too much \n"); break;
					case DPFPDD_QUALITY_SCAN_TOO_SHORT:       printf("    scan is too short \n"); break;
					case DPFPDD_QUALITY_SCAN_TOO_LONG:        printf("    scan is too long \n"); break;
					case DPFPDD_QUALITY_SCAN_TOO_SLOW:        printf("    speed of the swipe is too slow \n"); break;
					case DPFPDD_QUALITY_SCAN_TOO_FAST:        printf("    speed of the swipe is too fast \n"); break;
					case DPFPDD_QUALITY_SCAN_WRONG_DIRECTION: printf("    direction of the swipe is wrong \n"); break;
					case DPFPDD_QUALITY_READER_DIRTY:         printf("    reader needs cleaning \n"); break;
					default: printf("    unknown quality bitmask: 0x%x \n", i); break;
					}
				}
				continue;
			}
		}
		break;
	}
		
	//restore signal mask
	pthread_sigmask(SIG_SETMASK, &old_sigmask, NULL);
	
	//restore signal handler
	sigaction (SIGINT, &old_action, NULL);
	g_hReader = NULL;

	if(NULL != pImage){
		free(pImage);
		printf("\npImage free\n");
	}
	return result;
}
/* 
 * Copyright (C) 2011, Digital Persona, Inc.
 *
 * This file is a part of sample code for the UareU SDK 2.x.
 */

#include "helpers.h" 

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <dpfpdd.h>
 
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// error handling

void print_error(const char* szFunctionName, int nError){
	char sz[256];
	if(_DP_FACILITY == (nError >> 16)){
		char* szError = NULL;
		switch(nError){
		case DPFPDD_E_NOT_IMPLEMENTED: szError = "API call is not implemented."; break;
		case DPFPDD_E_FAILURE: szError = "Unspecified failure."; break;
		case DPFPDD_E_NO_DATA: szError = "No data is available."; break;
		case DPFPDD_E_MORE_DATA: szError = "The memory allocated by the application is not big enough for the data which is expected."; break;
		case DPFPDD_E_INVALID_PARAMETER: szError = "One or more parameters passed to the API call are invalid."; break;
		case DPFPDD_E_INVALID_DEVICE: szError = "Reader handle is not valid."; break;
		case DPFPDD_E_DEVICE_BUSY: szError = "The API call cannot be completed because another call is in progress."; break;
		case DPFPDD_E_DEVICE_FAILURE: szError = "The reader is not working properly."; break;
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
int        g_bCancel = 0;

void signal_handler(int nSignal) {
	if(SIGINT == nSignal){
		g_bCancel = 1;
		//cancel capture
		if(NULL != g_hReader) dpfpdd_cancel(g_hReader);
	}
}

int CaptureFinger(DPFPDD_DEV hReader, int bStream){
	int result = 0;

	//get capabilities to check if requested mode is supported
	int dpi;
	unsigned int nCapsSize = sizeof(DPFPDD_DEV_CAPS);
	int bCanCapture = 0, bCanStream = 0;
	while(1){
		DPFPDD_DEV_CAPS* pCaps = (DPFPDD_DEV_CAPS*)malloc(nCapsSize);
		if(NULL == pCaps){
			print_error("malloc()", ENOMEM);
			break;
		}
		pCaps->size = nCapsSize;
		result = dpfpdd_get_device_capabilities(hReader, pCaps);

		if(DPFPDD_SUCCESS != result && DPFPDD_E_MORE_DATA != result){
			print_error("dpfpdd_get_device_capabilities()", result);
			free(pCaps);
			break;
		}
		if(DPFPDD_E_MORE_DATA == result){
			nCapsSize = pCaps->size;
			free(pCaps);
			continue;
		}

		bCanCapture = pCaps->can_capture_image;
		bCanStream = pCaps->can_stream_image;
		dpi = pCaps->resolutions[0];

		free(pCaps);
		break;
	}

	if(bStream && !bCanStream){
		printf("this reader cannot work in streaming mode \n\n");
		return -1;
	}
	if(!bStream && !bCanCapture){
		printf("this reader cannot work in capture mode \n\n");
		return -1;
	}

	//prepare capture parameters and result
	DPFPDD_CAPTURE_PARAM cparam = {0};
	cparam.size = sizeof(cparam);
	cparam.image_fmt = DPFPDD_IMG_FMT_ISOIEC19794;
	cparam.image_proc = DPFPDD_IMG_PROC_NONE;
	cparam.image_res = dpi;
	DPFPDD_CAPTURE_RESULT cresult = {0};
	cresult.size = sizeof(cresult);
	cresult.info.size = sizeof(cresult.info);
	//get size of the image
	unsigned int nOrigImageSize = 0;
	result = dpfpdd_capture(hReader, &cparam, 0, &cresult, &nOrigImageSize, NULL);
	if(DPFPDD_E_MORE_DATA != result){
		print_error("dpfpdd_capture()", result);
		return result;
	}

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
	
	g_bCancel = 0;
	while(!g_bCancel){
		//wait until ready
		int is_ready = 0;
		unsigned int nImageSize = nOrigImageSize;
		while(!g_bCancel){
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

		if(0 == bStream){
			//capture fingerprint
			//printf("Put your finger on the reader, or press Ctrl-C to cancel...\r\n");
			result = dpfpdd_capture(hReader, &cparam, -1, &cresult, &nImageSize, pImage);
			if(DPFPDD_SUCCESS != result){
				//print_error("dpfpdd_capture()", result);
			}
			else{
				if(cresult.success){
					//captured 
					// printf("    fingerprint captured\n");
					// printf("    width: %d, height: %d \n\n", cresult.info.width, cresult.info.height);
				}
				else if(DPFPDD_QUALITY_CANCELED == cresult.quality){
					//capture canceled
					// printf("    fingerprint capture canceled\n");
					result = EINTR;
				}
				else{
					//bad capture
					printf("    bad capture, quality feedback: 0x%x.\n", cresult.quality);
					continue;
				}
			}
			break;
		}
		else{
			//stream fingerprints
			int nCnt = 0;
			int bFailure = 0;

			result = dpfpdd_start_stream(hReader);
			if(DPFPDD_SUCCESS != result){
				print_error("dpfpdd_start_stream()", result);
			}

			while(DPFPDD_SUCCESS == result && !g_bCancel){
				result = dpfpdd_get_stream_image(hReader, &cparam, &cresult, &nImageSize, pImage);
				if(DPFPDD_SUCCESS == result){
					nCnt++;
					printf("    %d image captured, score: %d \n", nCnt, cresult.score);
				}
				else{
					print_error("dpfpdd_get_stream_image()", result);
					bFailure = 1;
					break;
				}
			}

			result = dpfpdd_stop_stream(hReader);
			if(DPFPDD_SUCCESS != result){
				print_error("dpfpdd_stop_stream()", result);
			}

			if(bFailure) break;
		}
	}
	
	//restore signal mask
	pthread_sigmask(SIG_SETMASK, &old_sigmask, NULL);
	
	//restore signal handler
	sigaction (SIGINT, &old_action, NULL);
	g_hReader = NULL;
	
	if(NULL != pImage) free(pImage);
	return result;
} 
static int capture_cnt;
 
static void DPAPICALL CaptureCallback(void* pContext, unsigned int reserved, unsigned int nDataSize, void* pData){
	//sanity checks
//	if(NULL == pContext) return; This check is invalid as pContext is set to null
	if(NULL == pData) return;

	//allocate memory for capture data and the image in one chunk
	DPFPDD_CAPTURE_CALLBACK_DATA_0* pCaptureData = (DPFPDD_CAPTURE_CALLBACK_DATA_0*)pData;
	DPFPDD_CAPTURE_RESULT* cresult = (DPFPDD_CAPTURE_RESULT *) &pCaptureData->capture_result;

	if(DPFPDD_SUCCESS != pCaptureData->error){
		print_error("CaptureCallback()",  pCaptureData->error);
		//this will cause the async thread to self-terminate, let's exit from the main thread
		g_bCancel = 1;
		return;
	}
	if(cresult->success){
		//captured
		printf("    fingerprint captured\n");
		printf("    width: %d, height: %d \n\n", cresult->info.width, cresult->info.height);
	}
	else if(DPFPDD_QUALITY_CANCELED == cresult->quality){
		//capture canceled
		print_error("CaptureCallback() cancelled", DPFPDD_QUALITY_CANCELED);
		//this will cause the async thread to self-terminate, let's exit from the main thread
		g_bCancel = 1;
		return;
	}
	else{
		//bad capture
		printf("    bad capture, quality feedback: 0x%x.\n", cresult->quality);
		//continue;
	}

	printf("%d\r\nPut your finger on the reader, or press Ctrl-C to cancel...\r\n",++capture_cnt);

}

int AsyncCaptureFinger(DPFPDD_DEV hReader, int bStream){
	int result = 0;

	//get capabilities to check if requested mode is supported
	int dpi;
	unsigned int nCapsSize = sizeof(DPFPDD_DEV_CAPS);
	int bCanCapture = 0, bCanStream = 0;
	while(1){
		DPFPDD_DEV_CAPS* pCaps = (DPFPDD_DEV_CAPS*)malloc(nCapsSize);
		if(NULL == pCaps){
			print_error("malloc()", ENOMEM);
			break;
		}
		pCaps->size = nCapsSize;
		result = dpfpdd_get_device_capabilities(hReader, pCaps);

		if(DPFPDD_SUCCESS != result && DPFPDD_E_MORE_DATA != result){
			print_error("dpfpdd_get_device_capabilities()", result);
			free(pCaps);
			break;
		}
		if(DPFPDD_E_MORE_DATA == result){
			nCapsSize = pCaps->size;
			free(pCaps);
			continue;
		}

		bCanCapture = pCaps->can_capture_image;
		bCanStream = pCaps->can_stream_image;
		dpi = pCaps->resolutions[0];

		free(pCaps);
		break;
	}

	if(bStream && !bCanStream){
		printf("this reader cannot work in streaming mode \r\n\n");
		return -1;
	}
	if(!bStream && !bCanCapture){
		printf("this reader cannot work in capture mode \r\n\n");
		return -1;
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

	//prepare parameters and result
	DPFPDD_CAPTURE_PARAM cp = {0};
	cp.size = sizeof(cp);
	cp.image_fmt = DPFPDD_IMG_FMT_PIXEL_BUFFER;
	cp.image_proc = DPFPDD_IMG_PROC_NONE;
	cp.image_res = dpi;


	//start asyncronous capture
	void * context_unused = NULL;
	capture_cnt = 0;
	result = dpfpdd_capture_async(hReader, &cp, context_unused, CaptureCallback);
	if(DPFPDD_SUCCESS != result){
		printf("error when calling dpfpdd_capture_async() 0x%x", result);
		return -1;
	}

	printf("%d\r\nPut your finger on the reader, or press Ctrl-C to cancel...\r\n",++capture_cnt);
	g_bCancel = 0;

	while(!g_bCancel){
		// let the async thread do the job and keep an eye for Ctrl-C or device failures
		sleep(1);
	}

	//restore signal mask
	pthread_sigmask(SIG_SETMASK, &old_sigmask, NULL);
	
	//restore signal handler
	sigaction (SIGINT, &old_action, NULL);

	g_hReader = NULL;

	return result;
}  

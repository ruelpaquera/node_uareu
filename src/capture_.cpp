#include "helpers.h"
#include "capture.h"
#include "enrollment.h"
// #include "identify.h"
// #include "enrollment.h"

#include <unistd.h>


int fingerCapture(DPFPDD_DEV hReaders, int dpi,int stat,void *func,ENROLLFP_DATA FPdata){ 
	const int nFingerCnt = 5;
	unsigned char* vFmd[nFingerCnt];
	unsigned int vFmdSize[nFingerCnt];
	char* vFingerName[nFingerCnt];    
    int i = 0;
    for(i = 0; i < nFingerCnt; i++){
		vFmd[i] = NULL;
		vFmdSize[i] = 0;
	}
    vFingerName[0] = const_cast<char*>("your thumb");
	vFingerName[1] = const_cast<char*>("your index finger");
	vFingerName[2] = const_cast<char*>("your middle finger");
	vFingerName[3] = const_cast<char*>("your ring finger");
	vFingerName[4] = const_cast<char*>("any finger"); 

    int result = dpfpdd_led_config(hReaders, DPFPDD_LED_ACCEPT | DPFPDD_LED_REJECT, DPFPDD_LED_CLIENT, NULL);
	if(DPFPDD_SUCCESS != result && DPFPDD_E_NOT_IMPLEMENTED != result){
		//print_error("dpfpdd_led_config()", result);
	}
	int bStop = 0;
    while(!bStop){
        //capture fingers
		for(i = 0; i < nFingerCnt; i++){
            printf("capture fingers loop \n\n");
			if(0 == CaptureFinger(vFingerName[i], hReaders, dpi, DPFJ_FMD_ANSI_378_2004, &vFmd[i], &vFmdSize[i])) continue;			
			bStop = 1; 
			break;
		}

            printf("done capture fingers loop \n\n");
        if(!bStop){
        	long mseconds = 0;
			//struct timeval tv1, tv2;
			// gettimeofday(&tv1, NULL);

			//target false positive identification rate: 0.00001
			//for a discussion of  how to evaluate dissimilarity scores, as well as the statistical validity of the dissimilarity score and error rates, consult the Developer Guide
			unsigned int falsepositive_rate = DPFJ_PROBABILITY_ONE / 100000; 
			unsigned int nCandidateCnt = nFingerCnt;
			DPFJ_CANDIDATE vCandidates[nFingerCnt];
			int result = dpfj_identify(DPFJ_FMD_ANSI_378_2004, vFmd[nFingerCnt - 1], vFmdSize[nFingerCnt - 1], 0,
				DPFJ_FMD_ANSI_378_2004, nFingerCnt - 1, vFmd, vFmdSize, falsepositive_rate, &nCandidateCnt, vCandidates);

			// gettimeofday(&tv2, NULL);
			// mseconds = (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000; //time of operation in milliseconds

            printf("loop dpfj_identify result %d", result);
			if(DPFJ_SUCCESS == result){
				if(0 != nCandidateCnt){
					//optional: to get false match rate run compare for the top candidate
					unsigned int falsematch_rate = 0;
					result = dpfj_compare(DPFJ_FMD_ANSI_378_2004, vFmd[nFingerCnt - 1], vFmdSize[nFingerCnt - 1], 0, 
						DPFJ_FMD_ANSI_378_2004, vFmd[vCandidates[0].fmd_idx], vFmdSize[vCandidates[0].view_idx], 0, &falsematch_rate);

					//turn green LED on for 1 sec
					dpfpdd_led_ctrl(hReaders, DPFPDD_LED_ACCEPT, DPFPDD_LED_CMD_ON);
					usleep(1);
					dpfpdd_led_ctrl(hReaders, DPFPDD_LED_ACCEPT, DPFPDD_LED_CMD_OFF);

					//print out the results
					printf("Fingerprint identified (%ldms), %s\n", mseconds, vFingerName[vCandidates[0].fmd_idx]);
					printf("dissimilarity score: 0x%x.\n", falsematch_rate);
					printf("false match rate: %e.\n\n\n", (double)(falsematch_rate / DPFJ_PROBABILITY_ONE));
				}
				else{
					//turn red LED on for 1 sec
					dpfpdd_led_ctrl(hReaders, DPFPDD_LED_REJECT, DPFPDD_LED_CMD_ON);
					usleep(1000);
					dpfpdd_led_ctrl(hReaders, DPFPDD_LED_REJECT, DPFPDD_LED_CMD_OFF);

					//print out the results
					printf("Fingerprint was not identified.\n\n\n");
				}
			}
			//else print_error("dpfj_identify()", result);
        }

		//release memory
		for(i = 0; i < nFingerCnt; i++){
			if(NULL != vFmd[i]) free(vFmd[i]);
			vFmd[i] = NULL;
			vFmdSize[i] = 0;
		}
    }
	return bStop;
}
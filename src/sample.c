/* 
 * Copyright (C) 2011, Digital Persona, Inc.
 *
 * This file is a part of sample code for the UareU SDK 2.x.
 */

#include "helpers.h"
#include "menu.h"
#include "selection.h"
#include "verification.h"
#include "identification.h"
#include "enrollment.h"

#include <dpfpdd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <locale.h>

//////////////////////////////////////////////////////////////////////////
static int SelectEngine(void)
{
    int bStop = 0;
    int result = 0;
    while(!bStop)
    {
        //put menu on screen
        menu_t* pMenu = NULL;
        int result = Menu_Create("Engine selection", MENU_TYPE_BACK, &pMenu);
        if(0 == result) result = Menu_AddItem(pMenu, 0, "Select DPFR engine 6");
        if(0 == result) result = Menu_AddItem(pMenu, 1, "Select DPFR engine 7");
        if(0 == result){
            int nChoice = 0;
            Menu_DoModal(pMenu, &nChoice);

            if(-1 == nChoice){
                //back
                bStop = 1;
            }
            if(nChoice < 2){
                //engine selected, print out the info
                printf("\n");

                //printf("Selected:  %d\n", nChoice+1);
                switch (nChoice)
                {
                case 0:
                    result = dpfj_select_engine(NULL,DPFJ_ENGINE_DPFJ);// engine 6
                    break;
                case 1:
                    result = dpfj_select_engine(NULL,DPFJ_ENGINE_DPFJ7);// engine 7
                    break;
                }
                if(DPFPDD_SUCCESS != result)
                    printf("Could not select DPFR engine %d\n", nChoice+6);// engine 6
                else
                    printf("Now running DPFR engine %d\n", nChoice+6);// engine 7

                printf("\n");
                bStop = 1;
            }
            Menu_Destroy(pMenu);
        }
        else print_error("Menu_Create() or Menu_AddItem()", result);

    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main

int main(int argc, char** argv){
	//block all signals, SIGINT will be unblocked and handled in CaptureFinger()
	sigset_t sigmask;
	sigfillset(&sigmask);
	pthread_sigmask(SIG_BLOCK, &sigmask, NULL);
	
	setlocale(LC_ALL, "");
	
	//initialize capture library
	int result = dpfpdd_init();
	if(DPFPDD_SUCCESS != result) print_error("dpfpdd_init()", result);
	else{
		DPFPDD_DEV hReader = NULL; //handle of the selected reader
		int dpi = 0;
		char szReader[MAX_DEVICE_NAME_LENGTH]; //name of the selected reader
		
		menu_t* pMenu = NULL;
		int res = Menu_Create("UareU SDK 3.x sample application (verification, identification, enrollment)", MENU_TYPE_EXIT, &pMenu);
		if(0 == res) res = Menu_AddItem(pMenu, 101, "Select new reader (not selected)");
		if(0 == res) res = Menu_AddItem(pMenu, 102, "Run verification");
		if(0 == res) res = Menu_AddItem(pMenu, 103, "Run identification");
		if(0 == res) res = Menu_AddItem(pMenu, 104, "Run enrollment");
		if(0 == res) res = Menu_AddItem(pMenu, 105, "Select Engine ");
		if(0 == res){
			//main menu loop
			int bStop = 0;
			while(!bStop){
				int nChoice = 0;
				Menu_DoModal(pMenu, &nChoice);
				
				switch(nChoice){
					case 101: //select reader
						//close reader if opened
						if(NULL != hReader){
							result = dpfpdd_close(hReader);
							if(DPFPDD_SUCCESS != result) print_error("dpfpdd_close()", result);
							hReader = NULL;
						}
						//open new reader
						hReader = SelectAndOpenReader(szReader, sizeof(szReader),&dpi);
						if(NULL != hReader){
							char szItem[MAX_DEVICE_NAME_LENGTH + 20];
							snprintf(szItem, sizeof(szItem), "Select new reader (selected: %s)", szReader);
							Menu_AddItem(pMenu, 101, szItem);
						}
						else{
							Menu_AddItem(pMenu, 101, "Select new reader (not selected)");
						}
						break;
					case 102: //run verification
						if(NULL == hReader){
							printf("\nReader is not selected!");
						}
						else{
							Verification(hReader,dpi);
						}
						break;
					case 103: //run identification
						if(NULL == hReader){
							printf("\nReader is not selected!");
						}
						else{
							Identification(hReader,dpi);
						}
						break;
					case 104: //run enrollment
						if(NULL == hReader){
							printf("\nReader is not selected!");
						}
						else{
							Enrollment(hReader,dpi);
						}
						break;
					case 105: //select engine
						SelectEngine();
						//close reader if opened
						if(NULL != hReader){
							result = dpfpdd_close(hReader);
							hReader = NULL;
							Menu_AddItem(pMenu, 101, "Select new reader (not selected)");
						}
						break;   
					case -2: //exit
						bStop = 1;
						break;
				}
			}
			
			Menu_Destroy(pMenu);
		}
		else print_error("Menu_Create() or Menu_AddItem()", res);
		
		//close reader
		if(NULL != hReader){
			result = dpfpdd_close(hReader);
			if(DPFPDD_SUCCESS != result) print_error("dpfpdd_close()", result);
			hReader = NULL;
		}
		
		//release capture library
		dpfpdd_exit(); 
	}
	
	return 0;
}


//==============================================================================
//
// Title:		SerialECG
// Purpose:		A short description of the application.
//
// Created on:	28.10.2013 at 16:12:15 by robert.
// Copyright:	R97E56279. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <formatio.h>
#include <rs232.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "SerialECG.h"
#include "toolbox.h"
#include "ComManager.h"
#include "RealTimeECGProcess.h"


//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

int mainPanel = 0;
static int aboutPanel = 0;
int comPanel = 0;


//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/*******************************************************************************
	functia main
*******************************************************************************/
/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (mainPanel = LoadPanel (0, "SerialECG.uir", MAIN_PANEL));
	errChk (aboutPanel = LoadPanel (0, "SerialECG.uir", ABOUTPANEL));
	errChk (comPanel = LoadPanel (0, "SerialECG.uir", COM_PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (mainPanel));
	errChk (RunUserInterface ());

Error:

	if( flagComInStatus )
	{
			//daca portul serial este dechis resetez flagComInStatus
		flagComInStatus = FALSE; 
			//dezinstalez functie de callback
		InstallComCallback( comInPort, 0, 0, 0, 0, 0 ); 
			//inchid portul serial
		CloseCom( comInPort );
	}
	
	/* clean up */
	if (mainPanel > 0)
		DiscardPanel (mainPanel);
	if (aboutPanel > 0)
		DiscardPanel (aboutPanel);
	if (comPanel > 0)
		DiscardPanel (comPanel);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/*******************************************************************************
	functia de callback a panoului principal
*******************************************************************************/
/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK MainPanelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

/*******************************************************************************
	functia de callback a panoului despre
*******************************************************************************/
int CVICALLBACK OnAboutPanelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			RemovePopup(0);
			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a panoului de setari
*******************************************************************************/
int CVICALLBACK OnComPanelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:
			
			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:

			
			RemovePopup(0);
			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a barei de meniu
*******************************************************************************/
void CVICALLBACK OnMainMenubarCB (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int handleOnImage = 0;
	int handleOffImage = 0;
	
	switch(menuItem)
	{
		case  MENUBAR_FILE_OPEN:

			break;
		case  MENUBAR_FILE_SAVE:

			break;
		case  MENUBAR_FILE_EXIT:
			QuitUserInterface (0); //extra: inchid aplicatia (portul serial se inchide in main, dupa RunUserInterface)
			break;
		case  MENUBAR_SETTINGS_COM_IN_OPEN:
			handleOnImage = OpenFile("LedOn.bmp", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY );
			handleOffImage = OpenFile("LedOff.bmp", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY );
			
			SetMenuBarAttribute( menuBar, menuItem, ATTR_ITEM_NAME, (OpenCloseComInPort())? "Inchide com port" : "Deschide com port");
			SetMenuBarAttribute( menuBar, menuItem, ATTR_ITEM_BITMAP, flagComInStatus ? handleOnImage: handleOffImage );
			
			CloseFile( handleOnImage );
			CloseFile( handleOffImage );
			break;
		case  MENUBAR_SETTINGS_COM:
			DisplayComSettingsPanel();
			break;
		case  MENUBAR_ABOUT_ABOUT:
			InstallPopup( aboutPanel);
			break;
	}
}

int CVICALLBACK OnButtonSend (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char string[ 256 ] = { 0 };
	
	switch (event)
	{
		case EVENT_COMMIT:
			if( flagComInStatus )
			{
					//obtin textul ce trebuie trimis pe seriala
				GetCtrlVal( panel, MAIN_PANEL_IDC_TXT_TOSEND, string );
					//resetez continutul controlului
				SetCtrlVal( panel, MAIN_PANEL_IDC_TXT_TOSEND, "" );
					//scriu pe seriala textul respectiv
			//	ComWrt( /*de completat*/);
				ComWrt( comInPort, string, strlen( string ) ); //extra: scriu pe portul serial deschis toate caracterele textului
			}
			else
			{
				MessagePopup( "Atentie !", "Portul serial nu este deschis!"	);
			}
			break;
	}
	return 0;
}

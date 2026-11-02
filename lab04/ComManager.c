//==============================================================================
//
// Title:		ComManager.c
// Purpose:		A short description of the implementation.
//
// Created on:	28.10.2013 at 18:00:40 by Terran.
// Copyright:	UTI. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <userint.h>
#include <ansi_c.h>
#include <rs232.h>
#include "ComManager.h"
#include "SerialECG.h"
#include "ProcessData.h"
#include "RealTimeECGProcess.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
int comInPort = DEFAULT_COM_IN_PORT;
static int comInBaudRate = DEFAULT_COM_IN_BAUDRATE;
static int comInDataBits = DEFAULT_COM_IN_DATABITS;
static int comInStopBits  =DEFAULT_COM_IN_STOPBITS;
static int comInParityBits = DEFAULT_COM_IN_PARITYBITS;
static int comInQInSize = DEFAULT_COM_IN_QINSIZE;
static int comInQOutSize = DEFAULT_COM_IN_QOUTSIZE;

//==============================================================================
// Static functions
void CVICALLBACK ComInCallback (int portNumber, int eventMask, void *callbackData);

//==============================================================================
// Global variables
BOOL flagComInStatus = FALSE;

//==============================================================================
// Global functions

/*******************************************************************************
	functia ce actualizeaza configurarile si afiseaza panelul de setari
*******************************************************************************/
int DisplayComSettingsPanel(void)
{
	int err = 0;
	
		//actualizez valorile controalelor de pe panelul de configurare port serial
	SetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_PORT, comInPort );
	SetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_BAUDRATE, comInBaudRate );
	SetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_DATABITS, comInDataBits );
	SetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_STOPBITS, comInStopBits );
	SetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_PARITYBITS, comInParityBits );
	SetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_QINSIZE, comInQInSize );
	SetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_QOUTSIZE, comInQOutSize );
	
		//afise panelul de configurare
	InstallPopup( comPanel );

	return err;
}

/*******************************************************************************
	functia deschidere a portului serial de intrare
*******************************************************************************/
BOOL OpenCloseComInPort(void)
{
	int err = 0;
	char comName[ 64 ] = { 0 };
	
	//obtin valorile setarilor efectuate pentru portul serial
	GetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_PORT, &comInPort );
	GetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_BAUDRATE, &comInBaudRate );
	GetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_DATABITS, &comInDataBits );
	GetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_STOPBITS, &comInStopBits );
	GetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_PARITYBITS, &comInParityBits );
	GetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_QINSIZE, &comInQInSize );
	GetCtrlVal( comPanel, COM_PANEL_IDC_COM_IN_QOUTSIZE, &comInQOutSize );
	
	
	//verific starea portul serial
	if( !flagComInStatus )
	{
		//daca este inchis si trebuie deschis
		
			//formez numele portului serial
		sprintf( comName, "COM%d", comInPort );
			
			//deschid portu serial
		// err = OpenComConfig( /*de completat*/ );									  
		err = OpenComConfig(comInPort, comName, comInBaudRate, comInParityBits, comInDataBits,
						   comInStopBits, comInQInSize, comInQOutSize); //extra: ultimul parametru corectat: dimensiunea cozii de iesire (era pus de doua ori QInSize)
		
		//verifi daca s-a deschis cu succes
		if( err >= 0 )
		{
			//daca s-a deschis cu succes instalez o functie de callback
				
			err = InstallComCallback(comInPort,LWRS_RXCHAR,0,0,ComInCallback,NULL); //*
			
				//verific daca s-a instalat cu success
			if( err >= 0 )
			{
					//daca s-a instalat cu succes setez  flagComInStatus
				flagComInStatus = TRUE;			
			}
			else
			{
					//daca nu s-a instalat cu success resetez flagComInStatus
				flagComInStatus = FALSE;
				
					//inchi portul serial
				CloseCom( comInPort );
			}
		}
		else
		{
				//port serial nu s-a deshis, resetez flagComInStatus
			flagComInStatus = FALSE;		
		}
	}
	else
	{
		//portu serial este deschis si trebuie inchis
		
			//resetez flagComInStatus
		flagComInStatus = FALSE; 
			//dezinstalez functia de callback (argumentul pointer la functie trebuie sa fie 0
		InstallComCallback( comInPort, 0, 0, 0, 0, 0 ); 
			//inchid portul serial
		CloseCom( comInPort );
	}
	
		//returnez starea portului serial
	return flagComInStatus;
}


/*******************************************************************************
	functia de callback pentru portul serial de intrare
*******************************************************************************/
void CVICALLBACK ComInCallback (int portNumber, int eventMask, void *callbackData)
{
	unsigned int comBufferLength = 0;
	char * comBuffer = 0;
	unsigned int comReadBytes = 0;
	
	
		//atat timp cat exista octeti in bufferul de intrare
	while (comBufferLength = GetInQLen(portNumber))
	{
			//aloc memorie pentru sirul bufferul local in care citesc
		comBuffer = (char*)calloc( comBufferLength + 1, sizeof(char));
			//citesc din portul serial un numar de  comBufferLength octeti
		comReadBytes = ComRd(portNumber,comBuffer,comBufferLength);	//*
		
			//lucrand cu caractere trebuie adaugat terminatorul de sir
		comBuffer[ comBufferLength ] = 0;
			//pun pe interfata continutul
		SetCtrlVal( mainPanel, MAIN_PANEL_IDC_TXT_COM_BUFFER, comBuffer );
		
			//procesez informatia
		ProcessComInBuffer( comBuffer, comBufferLength );
		
			//eliberez spatiul de memorie ocupat de bufferul local
		free( comBuffer );
		comBuffer = 0;
	}
}

//==============================================================================
//
// Title:		ProcessData.c
// Purpose:		A short description of the implementation.
//
// Created on:	28.10.2013 at 18:53:14 by Terran.
// Copyright:	UTI. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <userint.h>
#include <ansi_c.h>
#include <utility.h>
#include "ProcessData.h"
#include "SerialECG.h"
#include "RealTimeECGProcess.h"

//==============================================================================
// Constants
#define PACK_BEGING		'<'
#define PACK_END		'>'

#define AUTOMAT_PACK_START	1
#define AUTOMAT_PACK_DATA	2

//==============================================================================
// Types

//==============================================================================
// Static global variables
static char pack[ 128 ] = { 0 };
static unsigned char packIndex = 0;
static automateState = AUTOMAT_PACK_START;
const int sizeBuffer = 300;
int fs = 128;
int delay, num_qrs, num_r_peaks;
int ecg_initialized = 0;
int ecg_peak_detected = 0;
int ecg_sample_counter = 0;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
double ecg_sample;
double heart_rate = 0.0;
//==============================================================================
// Global functions
// 
//	formatul pachetului este <123 123 123>
//
int ProcessComInBuffer( char *comBuffer, int comBufferLength )
{
	int err = 0;
	int i = 0, k = 0;
	//static int iCh0 = 0, iCh1 = 0, iCh2 = 0;
	
	static int iCh[3];
	
	//char pack[20]; //extra: scos - ascundea bufferul static 'pack' si se pierdea pachetul cand venea rupt in doua citiri
	char DateCanal[4] = {0, 0, 0, 0} ;
	DateCanal[3] = '\0';
	int indexDate = 0;
	int chCurent = 0;
	double bandpass_output = 0.0, derivative_output = 0.0, squared_output = 0.0, integrated_output = 0.0; //extra: iesirile etapelor Pan-Tompkins
	
	for( i = 0; i < comBufferLength; i++ )
	{
		switch( automateState )
		{
			case AUTOMAT_PACK_START:
				
				//...
				
				if (comBuffer[i] == PACK_BEGING) //extra: am gasit inceputul unui pachet '<'
				{
					packIndex = 0;
					automateState = AUTOMAT_PACK_DATA;
				}

				break;
			case AUTOMAT_PACK_DATA:
				
				//...
				if (comBuffer[i] == PACK_END) //extra: am gasit sfarsitul pachetului '>' (poate veni intr-o citire ulterioara, starea se pastreaza intre apeluri)
				{
					pack[packIndex] = '\0';
					automateState = AUTOMAT_PACK_START;
					
					if (sscanf(pack, "%d %d %d", &iCh[0], &iCh[1], &iCh[2]) != 3) //extra: extrag cele 3 valori; verific ca s-au citit toate
					{
						break;
					}
					
					PlotStripChartPoint( mainPanel, MAIN_PANEL_IDC_DISP_ECG1, (double)iCh[0] ); //*
					PlotStripChartPoint( mainPanel, MAIN_PANEL_IDC_DISP_ECG2, (double)iCh[1] ); //*
					PlotStripChartPoint( mainPanel, MAIN_PANEL_IDC_DISP_ECG3, (double)iCh[2] ); //*
					
					// To do 1: Conversia Datelor
    
					// Conversia valorilor de tip integer extrase din serial in valori de tip double
					ecg_sample = (double)iCh[2] / 1024.0; //extra: canalul D3 impartit la rezolutia convertorului (1024 de valori)
	
				    /* Initializarea parametrilor pentru functia de analiza a semnalului ECG */
				    if (!ecg_initialized) {
				        real_time_ecg_init();
						ecg_initialized = 1;
				    }
    
    				ecg_sample_counter++; 
					
					// To do 2: Includerea algoritmului
					
					/* Procesul de analiza a semnalului ECG pentru a se extrage complexul QRS */
					
				    // Filtru trece banda cu frecventele de taiere intre 5 si 15 Hz
				    bandpass_output = real_time_bandpass_filter(ecg_sample); //extra: pastrez doar banda 5-15 Hz (scot linia de baza si zgomotul)
					// Derivata semnalului filtrat
				    derivative_output = real_time_derivative(bandpass_output, ecg_sample_counter); //extra: panta semnalului, maxima in complexul QRS
					// Patratul derivatei
				    squared_output = derivative_output * derivative_output; //extra: valori pozitive si varfurile QRS amplificate
					// Valoarea mediei mobile a semnalului ridicat la patrat, utilizat pentru detectarea varfurilor undelor R
				    integrated_output = real_time_moving_average(squared_output, ecg_sample_counter); //extra: integrare pe fereastra de 20 de esantioane (energia QRS)
					// Detectarea valorii maxime din compelxul QRS si calcului frecventei cardiace
					heart_rate = real_time_peak_detection(integrated_output, bandpass_output, ecg_sample, ecg_sample_counter); //extra: praguri adaptive; intoarce BPM cand gaseste o unda R, altfel 0
					
					// Salvarea datelor pe Strip Chart si Display
					
					if(heart_rate != 0.0){
						
						//To do 3: Afisarea ritmului cardiac
						
						// Afisarea grafica a variatiei frecventei cardiace
						PlotStripChartPoint( mainPanel, MAIN_PANEL_IDC_DISP_HRV_3, heart_rate ); //extra: adaug valoarea BPM pe stripchart-ul de variabilitate
						// Afisarea valorii frecventei cardiace
						SetCtrlVal( mainPanel, MAIN_PANEL_NUMERIC_3, heart_rate ); //extra: afisez numeric frecventa cardiaca
					}
				}
				else if (comBuffer[i] == PACK_BEGING) //extra: a aparut '<' inainte de '>' (pachet stricat)
				{
					packIndex = 0;
				}
				else if (packIndex < sizeof(pack) - 1) //extra: caracter din interiorul pachetului si mai e loc in buffer
				{
					pack[packIndex++] = comBuffer[i];
				}
				else //extra: pachet prea lung (lipseste '>')
				{
					automateState = AUTOMAT_PACK_START;
				}
				
				break;
		}
	}
	
	return err;
}

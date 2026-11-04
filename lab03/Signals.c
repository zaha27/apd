//==============================================================================
//
// Title:		Signals.c
// Purpose:		A short description of the implementation.
//
// Created on:	21.10.2013 at 12:19:18 by Terran.
// Copyright:	UTI. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include "toolbox.h"
#include <ansi_c.h>
#include <formatio.h>
#include <utility.h>
#include <analysis.h>
#include <userint.h>
#include "Signals.h"
#include "AcquisitionSimulation.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
static double rowSignal[ GRAPH_NPOINTS ];
static char fileName[256] = {0};
static int traceColor = VAL_RED;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
int signalType = SIGNALTYPE_SINEWAVE; //tipul semnalului selectat in meniu
int plotHandle = NO_PLOT;  //variabila ce contine handle-ul plotului de pe graph
double amplitude = 5.0;	   //amplitudinea semnalului generat
double frequency = 1.0/113.0; //frecventa de achizitie a semnalului generat
double dutyCycle = 50.0; //factorul de umplere al semnalului dreptunghiular
BOOL saveFlag = FALSE; //flag ce indica daca butonul de salvare a fost apasat

//==============================================================================
// Global functions

/*******************************************************************************
	functia de callback a butonului de achizite
*******************************************************************************/
int CVICALLBACK ManualAcquisitionCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	static double phase = 0.0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//To do: 02
			//*********
			switch( signalType ) //extra: generez semnalul in functie de tipul selectat din meniul Semnal
			{
				case SIGNALTYPE_SINEWAVE:
					SineWave( GRAPH_NPOINTS, amplitude, frequency, &phase, rowSignal ); //extra: genereaza GRAPH_NPOINTS esantioane; phase se actualizeaza ca semnalul sa continue la urmatoarea achizitie
					break;
				case SIGNALTYPE_TRIANGLEWAVE:
					TriangleWave( GRAPH_NPOINTS, amplitude, frequency, &phase, rowSignal ); //extra: genereaza semnalul triunghiular in rowSignal
					break;
				case SIGNALTYPE_SQUAREWAVE:
					SquareWave( GRAPH_NPOINTS, amplitude, frequency, &phase, dutyCycle, rowSignal ); //extra: genereaza semnalul dreptunghiular cu factorul de umplere dutyCycle
					break;
				case SIGNALTYPE_NOISE:
					WhiteNoise( GRAPH_NPOINTS, amplitude, -1, rowSignal ); //extra: seed negativ = alt zgomot la fiecare apel
					break;
			}
			
			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a butonului de afisare
*******************************************************************************/
int CVICALLBACK ManualDisplayCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double maxVal = 0.0;
	double minVal = 0.0;
	ssize_t maxIndex = 0; //extra: ssize_t (nu int) pentru ca MaxMin1D cere ssize_t* (corect si pe 32, si pe 64 de biti)
	ssize_t minIndex = 0;
	double mean = 0.0;
	static int count = 0;
	char string[ MAX_PATHNAME_LEN ] = { 0 };
	
	switch (event)
	{
		case EVENT_COMMIT:
			//To do: 03
			//*********
			if( plotHandle != NO_PLOT )
			{
				DeleteGraphPlot( panel, MAIN_PANEL_IDC_GRAPH_DISPLAY, plotHandle, VAL_DELAYED_DRAW ); //extra: sterg plotul anterior folosind handle-ul lui
			}
			plotHandle = PlotY( panel, MAIN_PANEL_IDC_GRAPH_DISPLAY, rowSignal, GRAPH_NPOINTS, VAL_DOUBLE,
							    VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, traceColor ); //extra: desenez semnalul cu culoarea de trasare curenta si retin handle-ul plotului
			MaxMin1D( rowSignal, GRAPH_NPOINTS, &maxVal, &maxIndex, &minVal, &minIndex ); //extra: calculez maximul si minimul semnalului (si pozitiile lor)
			Mean( rowSignal, GRAPH_NPOINTS, &mean ); //extra: calculez media; apoi afisez minimul, maximul si media
			SetCtrlVal( panel, MAIN_PANEL_IDC_NUM_MIN, minVal );
			SetCtrlVal( panel, MAIN_PANEL_IDC_NUM_MAX, maxVal );
			SetCtrlVal( panel, MAIN_PANEL_IDC_NUM_MEAN, mean );
			
			//To do: 08
			//*********
			if( saveFlag ) //extra: daca butonul Salvare este apasat
			{
				ArrayToFile( fileName, rowSignal, VAL_DOUBLE, GRAPH_NPOINTS, 1, VAL_GROUPS_TOGETHER,
							 VAL_GROUPS_AS_COLUMNS, VAL_SEP_BY_TAB, 10, VAL_ASCII, VAL_TRUNCATE ); //extra: scriu in fisier semnalul afisat (o valoare pe linie, text)
			}
			
			//To do: 06
			//*********
			GraphDisplayCB( panel, MAIN_PANEL_IDC_GRAPH_DISPLAY, EVENT_COMMIT, 0, 0, 0 ); //extra: actualizez Esantion si Valoare pentru pozitia curenta a cursorului
			
			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a controlului esantion
*******************************************************************************/
int CVICALLBACK SampleCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double val = 0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//To do: 07
			//*********
			GetCtrlVal( panel, control, &val ); //extra: citesc indexul introdus in Esantion (mai jos il limitez la 0..GRAPH_NPOINTS-1)
			if( val < 0 )
			{
				val = 0;
			}
			if( val > GRAPH_NPOINTS - 1 )
			{
				val = GRAPH_NPOINTS - 1;
			}
			SetCtrlVal( panel, control, val );
			SetGraphCursor( panel, MAIN_PANEL_IDC_GRAPH_DISPLAY, 1, val, rowSignal[ (int)val ] ); //extra: mut cursorul 1 pe esantionul cerut
			SetCtrlVal( panel, MAIN_PANEL_IDC_NUM_SAMPLE_VALUE, rowSignal[ (int)val ] ); //extra: afisez valoarea esantionului

			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a controalelor color numeric (cursor si trasare)
*******************************************************************************/
int CVICALLBACK OnChooseColorCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int val = 0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//To do: 05
			//*********
			GetCtrlVal( panel, control, &val );
			if( control == MAIN_PANEL_IDC_COLOR_CURSOR ) //extra: s-a schimbat culoarea cursorului
			{
				SetCursorAttribute( panel, MAIN_PANEL_IDC_GRAPH_DISPLAY, 1, ATTR_CURSOR_COLOR, val ); //extra: aplic culoarea pe cursorul 1 al grafului
			}
			else if( control == MAIN_PANEL_IDC_COLOR_TRACE ) //extra: s-a schimbat culoarea de trasare
			{
				traceColor = val; //extra: retin culoarea, ca urmatoarele PlotY sa o foloseasca
				if( plotHandle != NO_PLOT )
				{
					SetPlotAttribute( panel, MAIN_PANEL_IDC_GRAPH_DISPLAY, plotHandle, ATTR_TRACE_COLOR, traceColor ); //extra: schimb culoarea plotului existent
				}
			}

			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a butoanelor salvare si incarcare
*******************************************************************************/
int CVICALLBACK FileIoCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int val = 0;
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	char path[MAX_PATHNAME_LEN] = { 0 };
	int acqMode = 0; //extra: pozitia switch-ului Mod de achizitie (0 = la cerere, 1 = automat)
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal( panel, control, &val );
			switch( control )
			{
				case MAIN_PANEL_IDC_BTT_FILESAVE:
					//To do: 08
					//*********
					if( val ) //extra: butonul Salvare a fost apasat (1)
					{
						GetSystemDate( &month, &day, &year ); //extra: citesc data si ora curenta pentru numele fisierului
						GetSystemTime( &hour, &minute, &second );
						sprintf( path, "RowSignal_%04d.%02d.%02d_%02d-%02d-%02d.dat",
								 year, month, day, hour, minute, second ); //extra: construiesc numele RowSignal_aaaa.ll.zz_hh-mm-ss.dat
						if( FileSelectPopup( "", path, "*.dat", "Salvare semnal", VAL_SAVE_BUTTON,
											 0, 1, 1, 1, fileName ) == VAL_NO_FILE_SELECTED ) //extra: utilizatorul alege unde salveaza; la Cancel ridic butonul si nu salvez nimic
						{
							SetCtrlVal( panel, control, 0 );
							saveFlag = FALSE;
							break;
						}
						saveFlag = TRUE; //extra: memorez ca butonul este apasat
						SetCtrlAttribute( panel, MAIN_PANEL_IDC_BTT_FILELOAD, ATTR_DIMMED, 1 ); //extra: dezactivez Incarca cat timp salvez
						if( plotHandle != NO_PLOT )
						{
							ArrayToFile( fileName, rowSignal, VAL_DOUBLE, GRAPH_NPOINTS, 1, VAL_GROUPS_TOGETHER,
										 VAL_GROUPS_AS_COLUMNS, VAL_SEP_BY_TAB, 10, VAL_ASCII, VAL_TRUNCATE ); //extra: scriu semnalul afisat in fisier
						}
					}
					else //extra: butonul Salvare a fost ridicat (0)
					{
						saveFlag = FALSE;
						GetCtrlVal( panel, MAIN_PANEL_IDC_SW_ACQMODE, &acqMode );
						SetCtrlAttribute( panel, MAIN_PANEL_IDC_BTT_FILELOAD, ATTR_DIMMED, acqMode ); //extra: reactivez Incarca doar in modul la cerere
					}

					break;
				
				case MAIN_PANEL_IDC_BTT_FILELOAD:
					//To do: 09
					//*********
					if( FileSelectPopup( "", "*.dat", "*.dat", "Incarcare semnal", VAL_LOAD_BUTTON,
										 0, 0, 1, 0, path ) == VAL_NO_FILE_SELECTED ) //extra: utilizatorul alege fisierul de incarcat
					{
						break;
					}
					if( FileToArray( path, rowSignal, VAL_DOUBLE, GRAPH_NPOINTS, 1, VAL_GROUPS_TOGETHER,
									 VAL_GROUPS_AS_COLUMNS, VAL_ASCII ) < 0 ) //extra: citesc semnalul din fisier in rowSignal
					{
						MessagePopup( "Eroare", "Fisierul selectat nu a putut fi citit!" );
						break;
					}
					ManualDisplayCB( panel, MAIN_PANEL_IDC_BTT_MANUALDISPLAY, EVENT_COMMIT, 0, 0, 0 ); //extra: afisez semnalul incarcat (plot cu culoarea curenta, min/max/medie, cursor)

					break;
			}
			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a grafului (atunci cand exista un cursor definit)
*******************************************************************************/
int CVICALLBACK GraphDisplayCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double x = 0;
	double y = 0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//To do: 06
			//*********
			GetGraphCursor( panel, MAIN_PANEL_IDC_GRAPH_DISPLAY, 1, &x, &y ); //extra: citesc pozitia cursorului 1 (mai jos o limitez la 0..GRAPH_NPOINTS-1)
			if( x < 0 )
			{
				x = 0;
			}
			if( x > GRAPH_NPOINTS - 1 )
			{
				x = GRAPH_NPOINTS - 1;
			}
			SetCtrlVal( panel, MAIN_PANEL_IDC_NUM_SAMPLE_INDEX, (double)(int)(x + 0.5) ); //extra: afisez indexul esantionului (rotunjit)
			SetCtrlVal( panel, MAIN_PANEL_IDC_NUM_SAMPLE_VALUE, rowSignal[ (int)(x + 0.5) ] ); //extra: afisez valoarea esantionului din semnal

			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a numeric slide-ului interval de achizitie
*******************************************************************************/
int CVICALLBACK AcquisitionTimerPeriodCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double val = 0.0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//To do: 04
			//*********
			GetCtrlVal( panel, control, &val ); //extra: citesc perioada aleasa pe slider (secunde)
			SetCtrlAttribute( panel, MAIN_PANEL_SIMULATION_TIMER, ATTR_INTERVAL, val ); //extra: modific perioada timerului de simulare
			
			break;
	}
	return 0;
}

/*******************************************************************************
	functia de callback a timerului de simulare
*******************************************************************************/
int CVICALLBACK OnSimulationTimerCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			//To do: 04
			//*********
			ManualAcquisitionCB( panel, MAIN_PANEL_IDC_BTT_MANUAL_ACQ, EVENT_COMMIT, 0, 0, 0 ); //extra: la fiecare tick fac o achizitie (ca la cerinta 2)
			ManualDisplayCB( panel, MAIN_PANEL_IDC_BTT_MANUALDISPLAY, EVENT_COMMIT, 0, 0, 0 ); //extra: si afisez semnalul (ca la cerinta 3)
			
			break;
	}
	return 0;
}


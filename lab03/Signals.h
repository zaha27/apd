//==============================================================================
//
// Title:		Defines.h
// Purpose:		A short description of the interface.
//
// Created on:	21.10.2013 at 12:09:08 by Terran.
// Copyright:	UTI. All Rights Reserved.
//
//==============================================================================

#ifndef __Defines_H__
#define __Defines_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants
//To do: 02
//*********
#define NO_SIGNAL						0
#define SIGNALTYPE_SINEWAVE				1
#define SIGNALTYPE_TRIANGLEWAVE			2
#define SIGNALTYPE_SQUAREWAVE			3
#define SIGNALTYPE_NOISE				4
		
#define NO_PLOT							-1	
		
#define GRAPH_NPOINTS 					500 /*numarul de puncte de pe graph, semnal*/

//To do: 01
//*********
#define USER_EVENTS						1000
#define MYEVENT_CLOSE					(USER_EVENTS + 1) /// eveniment definit de programator pentru inchiderea aplicatiei
		
//==============================================================================
// Types

//==============================================================================
// External variables
extern int signalType;
extern int plotHandle;
extern BOOL saveFlag;

//==============================================================================
// Global functions

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Defines_H__ */

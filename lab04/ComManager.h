//==============================================================================
//
// Title:		ComManager.h
// Purpose:		A short description of the interface.
//
// Created on:	28.10.2013 at 18:00:40 by Terran.
// Copyright:	UTI. All Rights Reserved.
//
//==============================================================================

#ifndef __ComManager_H__
#define __ComManager_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants
#define FALSE						0
#define TRUE 						1
		
#define DEFAULT_COM_IN_PORT			2
#define DEFAULT_COM_IN_BAUDRATE		9600
#define DEFAULT_COM_IN_DATABITS		8
#define DEFAULT_COM_IN_STOPBITS		1
#define DEFAULT_COM_IN_PARITYBITS	0
#define DEFAULT_COM_IN_QINSIZE		1024
#define DEFAULT_COM_IN_QOUTSIZE		1024

//==============================================================================
// Types

//==============================================================================
// External variables
extern int comPanel;
extern BOOL flagComInStatus;
extern int comInPort;

//==============================================================================
// Global functions

BOOL OpenCloseComInPort(void);
int DisplayComSettingsPanel(void);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __ComManager_H__ */

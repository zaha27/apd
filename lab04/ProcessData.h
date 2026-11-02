//==============================================================================
//
// Title:		ProcessData.h
// Purpose:		A short description of the interface.
//
// Created on:	28.10.2013 at 18:53:14 by Terran.
// Copyright:	UTI. All Rights Reserved.
//
//==============================================================================

#ifndef __ProcessData_H__
#define __ProcessData_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables
extern int mainPanel;  

//==============================================================================
// Global functions

int ProcessComInBuffer( char *comBuffer, int comBufferLenght );

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __ProcessData_H__ */

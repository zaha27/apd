/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2013. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ABOUTPANEL                       1       /* callback function: OnAboutPanelCB */
#define  ABOUTPANEL_IDC_PICTURE           2       /* control type: picture, callback function: (none) */
#define  ABOUTPANEL_IDC_TEXT_ABOUT        3       /* control type: textMsg, callback function: (none) */

#define  MAIN_PANEL                       2       /* callback function: MainPanelCB */
#define  MAIN_PANEL_IDC_GRAPH_DISPLAY     2       /* control type: graph, callback function: GraphDisplayCB */
#define  MAIN_PANEL_IDC_SW_ACQMODE        3       /* control type: binary, callback function: AcquisitionModeCB */
#define  MAIN_PANEL_IDC_SW_POWER          4       /* control type: binary, callback function: PowerSwitchCB */
#define  MAIN_PANEL_IDC_POWER_LED         5       /* control type: LED, callback function: (none) */
#define  MAIN_PANEL_IDC_BTT_MANUALDISPLAY 6       /* control type: command, callback function: ManualDisplayCB */
#define  MAIN_PANEL_IDC_BTT_MANUAL_ACQ    7       /* control type: command, callback function: ManualAcquisitionCB */
#define  MAIN_PANEL_IDC_NUM_SAMPLE_VALUE  8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_NUM_SAMPLE_INDEX  9       /* control type: numeric, callback function: SampleCB */
#define  MAIN_PANEL_IDC_NUM_MAX           10      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_NUM_MEAN          11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_NUM_MIN           12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_TXT_SIGNAL        13      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_IDC_TXT_FILEIO        14      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_IDC_TXT_SAMPLE        15      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_IDC_COLOR_TRACE       16      /* control type: color, callback function: OnChooseColorCB */
#define  MAIN_PANEL_IDC_COLOR_CURSOR      17      /* control type: color, callback function: OnChooseColorCB */
#define  MAIN_PANEL_IDC_SL_ACQ_PERIOD     18      /* control type: scale, callback function: AcquisitionTimerPeriodCB */
#define  MAIN_PANEL_DECORATION_7          19      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_6          20      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_5          21      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_4          22      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_3          23      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_2          24      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION            25      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_IDC_BTT_FILESAVE      26      /* control type: textButton, callback function: FileIoCB */
#define  MAIN_PANEL_SIMULATION_TIMER      27      /* control type: timer, callback function: OnSimulationTimerCB */
#define  MAIN_PANEL_IDC_BTT_FILELOAD      28      /* control type: command, callback function: FileIoCB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_FILE                     2
#define  MENUBAR_FILE_NEW                 3       /* callback function: OnMenuBar */
#define  MENUBAR_FILE_LOAD                4       /* callback function: OnMenuBar */
#define  MENUBAR_FILE_SAVE                5       /* callback function: OnMenuBar */
#define  MENUBAR_FILE_SEPARATOR           6
#define  MENUBAR_FILE_EXIT                7       /* callback function: OnMenuBar */
#define  MENUBAR_SIGNAL                   8
#define  MENUBAR_SIGNAL_SINE              9       /* callback function: OnMenuBar */
#define  MENUBAR_SIGNAL_TRIANGLE          10      /* callback function: OnMenuBar */
#define  MENUBAR_SIGNAL_SQUARE            11      /* callback function: OnMenuBar */
#define  MENUBAR_SIGNAL_NOISE             12      /* callback function: OnMenuBar */
#define  MENUBAR_ABOUT                    13
#define  MENUBAR_ABOUT_ABOUT              14      /* callback function: OnMenuBar */


     /* Callback Prototypes: */

int  CVICALLBACK AcquisitionModeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AcquisitionTimerPeriodCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FileIoCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GraphDisplayCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualAcquisitionCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ManualDisplayCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAboutPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChooseColorCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK OnMenuBar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK OnSimulationTimerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PowerSwitchCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SampleCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

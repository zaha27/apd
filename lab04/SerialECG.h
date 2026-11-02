/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
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
#define  ABOUTPANEL_IDC_ECG_PICTURE       2       /* control type: picture, callback function: (none) */
#define  ABOUTPANEL_IDC_ECG_TEXT          3       /* control type: textMsg, callback function: (none) */

#define  COM_PANEL                        2       /* callback function: OnComPanelCB */
#define  COM_PANEL_IDC_COM_OUT_PARITYBIT  2       /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_OUT_STOPBITS   3       /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_OUT_DATABITS   4       /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_OUT_BAUDRATE   5       /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_OUT_PORT       6       /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_OUT_QOUTSIZE   7       /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_OUT_QINSIZE    8       /* control type: ring, callback function: (none) */
#define  COM_PANEL_DECORATION_2           9       /* control type: deco, callback function: (none) */
#define  COM_PANEL_IDC_COM_IN_PARITYBITS  10      /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_IN_STOPBITS    11      /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_IN_DATABITS    12      /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_IN_BAUDRATE    13      /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_IN_PORT        14      /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_IN_QOUTSIZE    15      /* control type: ring, callback function: (none) */
#define  COM_PANEL_IDC_COM_IN_QINSIZE     16      /* control type: ring, callback function: (none) */
#define  COM_PANEL_DECORATION             17      /* control type: deco, callback function: (none) */
#define  COM_PANEL_IDC_TXT_COM_IN         18      /* control type: textMsg, callback function: (none) */
#define  COM_PANEL_IDC_TXT_COM_IN_2       19      /* control type: textMsg, callback function: (none) */

#define  MAIN_PANEL                       3       /* callback function: MainPanelCB */
#define  MAIN_PANEL_IDC_DISP_HRV_3        2       /* control type: strip, callback function: (none) */
#define  MAIN_PANEL_IDC_DISP_ECG1         3       /* control type: strip, callback function: (none) */
#define  MAIN_PANEL_IDC_DISP_ECG2         4       /* control type: strip, callback function: (none) */
#define  MAIN_PANEL_IDC_DISP_ECG3         5       /* control type: strip, callback function: (none) */
#define  MAIN_PANEL_IDC_BTT_SEND          6       /* control type: command, callback function: OnButtonSend */
#define  MAIN_PANEL_IDC_TXT_COM_BUFFER    7       /* control type: textBox, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_3             8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_TXT_TOSEND        9       /* control type: string, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_FILE                     2
#define  MENUBAR_FILE_OPEN                3       /* callback function: OnMainMenubarCB */
#define  MENUBAR_FILE_SAVE                4       /* callback function: OnMainMenubarCB */
#define  MENUBAR_FILE_SEPARATOR           5
#define  MENUBAR_FILE_EXIT                6       /* callback function: OnMainMenubarCB */
#define  MENUBAR_SETTINGS                 7
#define  MENUBAR_SETTINGS_COM_IN_OPEN     8       /* callback function: OnMainMenubarCB */
#define  MENUBAR_SETTINGS_SEPARATOR_2     9
#define  MENUBAR_SETTINGS_COM             10      /* callback function: OnMainMenubarCB */
#define  MENUBAR_ABOUT                    11
#define  MENUBAR_ABOUT_ABOUT              12      /* callback function: OnMainMenubarCB */


     /* Callback Prototypes: */

int  CVICALLBACK MainPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAboutPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnButtonSend(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnComPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK OnMainMenubarCB(int menubar, int menuItem, void *callbackData, int panel);


#ifdef __cplusplus
    }
#endif
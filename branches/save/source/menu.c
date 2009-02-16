/*++
Copyright ? 2008 

Module Name:
    menu.c

Abstract:
    The implement of the game menu.

Author:
    jason (jsfaint@gmail.com) 2008-11-11

--*/

#include "menu.h"

void vbirthday(void);

/***********************************************************
void MenuInit(void);

Routine Description:
    initial game menu.
Arguments:
    void
Return Value:
    None
***********************************************************/
void vMenuInit(u8 *gameState)
{
    PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
    PA_SetBgPalCol(0, 2, PA_RGB(31, 0, 0));
    PA_SetBgPalCol(0, 3, PA_RGB(0, 0, 31));
    PA_SetBgPalCol(0, 4, PA_RGB(0, 31, 0));
    PA_Init8bitBg(0, 3);

    *gameState = Menu_Show;
}

void vMenuShow(u8 *gameState)
{
    if (*gameState == Menu_Show)
        PA_CenterSmartText(0, 0, 100, 255, 120, "Touch to Start!", 2, 4, 1);

    //PA_WaitFor(Pad.Newpress.Anykey || Stylus.Newpress);
    if(Stylus.Newpress || Pad.Newpress.Anykey)
        *gameState = Game_Init;
}

void vSplashScreen(void)
{
    vbirthday();
}

void vbirthday(void)
{
    int i;
    if ((PA_UserInfo.BdayMonth == PA_RTC.Month) && (PA_UserInfo.BdayDay == PA_RTC.Day))
    {
        PA_OutputText(0, 2, 6, "Hi %s,", PA_UserInfo.Name);
        PA_OutputText(0, 4, 8, "Happy Birthday!", PA_UserInfo.Name);
        PA_OutputText(0, 4, 10, "Please enjoy the game :)");
        PA_OutputText(0, 16, 15, "TKKN DS Team");
        PA_OutputText(0, 18, 16, "20%02d.%02d.%02d", PA_RTC.Year, PA_RTC.Month, PA_RTC.Day);

        PA_WaitFor(Stylus.Newpress || Pad.Newpress.Anykey);
    }
    else
       return;
}
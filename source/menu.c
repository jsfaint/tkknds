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
    //PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
    //PA_SetBgPalCol(0, 2, PA_RGB(31, 0, 0));
    //PA_SetBgPalCol(0, 3, PA_RGB(0, 0, 31));
    //PA_SetBgPalCol(0, 4, PA_RGB(0, 31, 0));
    //PA_Init8bitBg(0, 3);
    //PA_CenterSmartText(0, 0, 100, 255, 120, "Touch to Start!", 2, 4, 1);
    
    PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
    PA_Init8bitBg(1, 3);

    
    PA_CenterSmartText(1, 0, 100, 255, 120, "TKKN DS", 1, 4, 1);
    *gameState = Menu_Show;
//    while(1)
//   {
//        if (Stylus.Newpress || Pad.Newpress.Anykey)
//        {
//            *gameState = Menu_Show;
//            break;
//        }
//        
//        PA_WaitForVBL();
//    }
//    
//    PA_Clear8bitBg(0); //clear screen 0
}

enum GameMenu
{
    START_GAME = 0,
    TOP_10,
    Credit
};
void vMenuShow(u8 *gameState)
{
    int iSelect = 0;
    
    PA_InitText(1, 0);
    PA_InitText(0, 0);
    
    PA_OutputSimpleText(g_screen,11,10,"Game Start");
    PA_OutputSimpleText(g_screen,11,12,"Game Score");
    
    while(1)
    {
        if (Pad.Newpress.Up)
            iSelect--;
        else if (Pad.Newpress.Down)
            iSelect++;

        if (iSelect < 0)
            iSelect = 1;
        else if (iSelect > 1)
            iSelect = 0;
        
        switch(iSelect)
        {
            case START_GAME:
                PA_OutputSimpleText(g_screen,7,10,"->");
                PA_OutputSimpleText(g_screen,7,12,"  ");

                if (Pad.Newpress.A)
                {
                    *gameState = Game_Init;
                    PA_Clear8bitBg(0);
                    PA_Clear8bitBg(1);
                    return;
                }
                break;

            case TOP_10:
                PA_OutputSimpleText(g_screen,7,10,"  ");
                PA_OutputSimpleText(g_screen,7,12,"->");

                if (Pad.Newpress.A)
                {
                    *gameState = Game_Statistic;
                    PA_Clear8bitBg(0);
                    PA_Clear8bitBg(1);
                    PA_ClearTextBg(0);
                    return;
                }
                break;

            case Credit:
                break;

            default:
                break;
        }
        
        PA_WaitForVBL();
    }
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

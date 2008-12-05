/*++
Copyright ? 2008 Pesonal Usage

Module Name:
    menu.c

Abstract:
    The implement of the game menu.

Author:
    jason (jsfaint@gmail.com) 2008-11-11

--*/

#include "menu.h"

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
    //PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
    //PA_SetBgPalCol(1, 2, PA_RGB(31, 0, 0));	
    //PA_SetBgPalCol(1, 3, PA_RGB(0, 0, 31));
    //PA_SetBgPalCol(1, 4, PA_RGB(0, 31, 0));
    
    PA_Init8bitBg(0, 3);
    //PA_Init8bitBg(1, 3);
    
    *gameState = Menu_Show;
}

void vMenuShow(u8 *gameState)
{
    if (*gameState == Menu_Show)
        PA_CenterSmartText(0, 0, 100, 255, 120, "Touch to Start!", 2, 4, 1);

    if(Stylus.Newpress || Pad.Newpress.Anykey)
        *gameState = Game_Init;

}

vSplashScreen(void)
{
    PA_SplashBlue();
}
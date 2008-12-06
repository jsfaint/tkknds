// Includes
#include <PA9.h>
#if !defined(__HEADER_H__)
#include "header.h"
#endif
#if !defined(__MENU_H__)
#include "menu.h"
#endif
#if !defined(__GAME_H__)
#include "game.h"
#endif
#if !defined(__SOUND_H__)
#include "sound.h"
#endif


u8  g_screen = 0;
s32 g_count = 0;

// Function: main()
int main(int argc, char ** argv)
{
    u8 gameState = Menu_Init;
    
    PA_Init();    // Initializes PA_Lib
    PA_InitVBL(); // Initializes a standard VBL
    
    PA_InitText(1,0); // On the top screen

    // Game Splash Screens
    vSplashScreen();
    
    vSoundInitial();
    // Infinite loop to keep the program running
    while (1)
    {
        switch(gameState)
        {
        case Menu_Init:
            vMenuInit(&gameState);
            break;
        case Menu_Show:
            vMenuShow(&gameState);
            break;
        case Game_Init:
            iGameInit(&gameState, 0);
            break;
        case Game_Play:
            g_count++;
            vGamePlay(&gameState);
            break;
        
        case Game_Pause:
            vGamePause(&gameState);
            break;
        
        case Game_Statis:
            if(Pad.Newpress.Anykey || Stylus.Newpress)
                gameState = Menu_Init;
            break;
        default:
            break;
        }
        
        PA_OutputText(1, 0, 1, "%d.%02ds", g_count/PA_RTC.FPS, g_count%PA_RTC.FPS);

        PA_WaitForVBL();
    }
    
    return 0;
} // End of main()

// Includes
#include <PA9.h>
#include "efs_lib.h"
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
bool  g_bLibfat = 0;
int InitFileSys(void);

// Function: main()
int main(int argc, char ** argv)
{
    u8 gameState = Menu_Init;

    PA_Init();    // Initializes PA_Lib
    PA_InitVBL(); // Initializes a standard VBL
    PA_InitText(1, 0);
    PA_InitText(0, 0);

#ifdef SAVE_DATA
    //check fat
    PA_OutputSimpleText(g_screen,5,10,"Check saving data!");
    PA_OutputSimpleText(g_screen,5,11,"Please stand by...");
    g_bLibfat = InitFileSys();
    if (g_bLibfat)
    {
        iCheckData();
    }
    PA_OutputSimpleText(g_screen,5,10,"                  ");
    PA_OutputSimpleText(g_screen,5,11,"                  ");
    PA_WaitForVBL();
#endif

    vSoundInitial();

    // Game Splash Screens
    vSplashScreen();
    
    // Infinite loop to keep the program running
    while (1)
    {
        switch(gameState)
        {
            case Menu_Init:
                //vSoundPlayOp();
                vMenuInit(&gameState);
                break;

            case Menu_Show:
                vMenuShow(&gameState);
                break;

            case Game_Init:
                //AS_MP3Stop();
                iGameInit(&gameState);
                break;

            case Game_Play:
                vSoundPlayBgm();
                PA_WaitForVBL();
                PA_WaitForVBL();
                PA_WaitForVBL();
                vGamePlay(&gameState);
                break;

            case Game_Pause:
                vGamePause(&gameState);
                break;

            case Game_Statistic:
                vGameStatistic(&gameState);
                break;

            default:
                break;
        }

        PA_WaitForVBL();
    }

    return 0;
} // End of main()

int InitFileSys(void)
{
    // Initialize EFS
    if (!EFS_Init(EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL)) {
        PA_OutputText(0, 1, 1, "EFS init error !!!");
        return false;
    }
    
    // Check if test.mp3 is there, regardless if libfat or EFS_Lib is used
    MP3FILE* file = FILE_OPEN("bgm.mp3");
    if (!file) {
        PA_OutputText(0, 1, 1, "bgm.mp3 not found !!!");
        PA_WaitFor(Stylus.Newpress || Pad.Newpress.Anykey);
        return false;
    }
    FILE_CLOSE(file);
    
    MP3FILE *file1 = FILE_OPEN("op.mp3");
    if (!file) {
        PA_OutputText(0, 1, 1, "op.mp3 not found !!!");
        PA_WaitFor(Stylus.Newpress || Pad.Newpress.Anykey);
        return false;
    }
    FILE_CLOSE(file1);
    
    return true;
}

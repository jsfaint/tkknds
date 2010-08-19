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
#include "save.h"

void draw_arrow(int iSelect);
void vbirthday(void);

extern Option g_option;

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
	PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
	PA_Init8bitBg(1, 3);
	PA_InitText(1, 0);
	PA_InitText(0, 0);

	PA_CenterSmartText(1, 0, 100, 255, 120, "TKKN DS", 1, 4, 1);

	PA_OutputSimpleText(g_screen, 11, 10, "Start");
	PA_OutputSimpleText(g_screen, 11, 12, "Score");
	PA_OutputSimpleText(g_screen, 11, 14, "Option");
#if 0
	PA_OutputSimpleText(g_screen, 11, 16, "Credit");
#endif

	PA_OutputText(1, 26, 22, "%s", VERSION);

	*gameState = Menu_Show;
}

enum GameMenu
{
	Menu_Start = 0,
	Menu_Score,
	Menu_Option,
	Menu_Credit
};
void vMenuShow(u8 *gameState)
{
	int iSelect = 0;

	while(1)
	{
		if (Pad.Newpress.Up)
			iSelect--;
		else if (Pad.Newpress.Down)
			iSelect++;

		if (iSelect < Menu_Start)
			iSelect = Menu_Option;
		else if (iSelect > Menu_Option)
			iSelect = Menu_Start;

		switch(iSelect)
		{
		case Menu_Start:
			draw_arrow(iSelect);

			if (Pad.Newpress.A)
			{
				*gameState = Game_Init;
				PA_Clear8bitBg(0);
				PA_Clear8bitBg(1);
				return;
			}
			break;

		case Menu_Score:
			draw_arrow(iSelect);

			if (Pad.Newpress.A)
			{
				*gameState = Game_Statistic;
				PA_Clear8bitBg(0);
				PA_Clear8bitBg(1);
				PA_ClearTextBg(0);
				return;
			}
			break;

		case Menu_Option:
			draw_arrow(iSelect);

			if (Pad.Newpress.A)
			{
				*gameState = Game_Option;
				PA_Clear8bitBg(0);
				PA_Clear8bitBg(1);
				PA_ClearTextBg(0);
				return;
			}
			break;

		case Menu_Credit:
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
	if ((PA_UserInfo.BdayMonth == PA_RTC.Month) && (PA_UserInfo.BdayDay == PA_RTC.Day))
	{
		PA_OutputText(0, 2, 6, "Hi %s,", PA_UserInfo.Name);
		PA_OutputText(0, 4, 8, "Happy Birthday!", PA_UserInfo.Name);
		PA_OutputText(0, 4, 10, "Please enjoy the game :)");
		PA_OutputText(0, 16, 15, "TKKN DS Team");
		PA_OutputText(0, 18, 16, "20%02d.%02d.%02d", PA_RTC.Year, PA_RTC.Month, PA_RTC.Day);

		PA_WaitFor(Stylus.Newpress || Pad.Newpress.Anykey);
	}
}

void draw_arrow(int iSelect)
{
	PA_OutputSimpleText(g_screen, 7, 10, "  ");
	PA_OutputSimpleText(g_screen, 7, 12, "  ");
	PA_OutputSimpleText(g_screen, 7, 14, "  ");
	PA_OutputSimpleText(g_screen, 7, 16, "  ");

	PA_OutputSimpleText(g_screen, 7, 10 + 2*iSelect, "->");
}

//game option menu
void vGameOption(u8 *pGameState)
{
	Save sData;
	u8 row = 0;

	PA_Clear8bitBg(0);
	PA_Clear8bitBg(1);
	PA_ClearTextBg(0);

	PA_OutputSimpleText(g_screen, 10, 10, "Music");
	PA_OutputSimpleText(g_screen, 10, 14, "Sound");

	while(1) {
		if (g_option.music_enable) {
			PA_OutputSimpleText(g_screen, 8, 11, "-> On      OFF");
		}
		else {
			PA_OutputSimpleText(g_screen, 8, 11, "   On   -> OFF");
		}
		
		if (g_option.sound_enable) {
			PA_OutputSimpleText(g_screen, 8, 15, "-> On      OFF");
		}
		else {
			PA_OutputSimpleText(g_screen, 8, 15, "   On   -> OFF");
		}

		if (Pad.Newpress.Up || Pad.Newpress.Down) {
			row = !row;
			if (row) {
				PA_OutputSimpleText(g_screen, 6, 10, " ");
				PA_OutputSimpleText(g_screen, 6, 14, "*");
			}
			else {
				PA_OutputSimpleText(g_screen, 6, 10, "*");
				PA_OutputSimpleText(g_screen, 6, 14, " ");
			}
		}

		if (Pad.Newpress.Left || Pad.Newpress.Right) {
			if (row) {
				g_option.sound_enable = !g_option.sound_enable;
			}
			else {
				g_option.music_enable = !g_option.music_enable;
			}
		}

		if (Pad.Newpress.B) {
			break;
		}

		PA_WaitForVBL();
	}

	iSetOption(g_option);
}

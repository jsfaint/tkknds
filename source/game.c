/*++
  Copyright ? 2008
  Module Name:
  game.c
Abstract:

Author:
jason (jsfaint@gmail.com) 2008-11-12
--*/

#include "game.h"
#include "gfx/all_gfx.c"
#ifdef SAVE_DATA
#include "save.h"
#endif

#define PLANEX  (g_plane.x>>8)
#define PLANEY  (g_plane.y>>8)

s32 g_count = 0;
u32 g_bulletNum = BULLET_INIT;
Plane g_plane;
Bullet g_bullet[BULLET_MAX];
u16 g_plane_gfx;
u16 g_bullet_normal_gfx;
#if 0
u16 g_bullet_hunter_gfx;
u16 g_bullet_explode_gfx;
#endif

/************** Function *****************/
static void vPlaneInit(void);
static void vMovePlane(void);
bool bCheckCollision();
void vBulletInit(u8 uIndex);
void vMoveBullet(u8 *pGameState);
void vBulletInitAll(void);
void vDestructSprites(void);
void vInputName(char* szBuf, int *iLength);
void vSaveFileData(u8 *pGameState);
int iShowResult(int position, Score score);
void vCreateBullet(u8 index);

s32 iGameInit(u8 *pGameState)
{
	g_count = 0;
	// load backgroud
	PA_EasyBgLoad(g_screen, 0, bg);

	vPlaneInit();

	memset(g_bullet, 0, sizeof(Bullet)*BULLET_MAX);
	vBulletInitAll();

	*pGameState = Game_Play;
	return 0;
}

void vGamePlay(u8 *pGameState)
{
	while(*pGameState == Game_Play)
	{
		g_count++;
		PA_OutputText(1, 1, 0, "TKKN DS %s", VERSION);
		PA_OutputText(1, 1, 2, "%d.%02ds", g_count/PA_RTC.FPS, g_count%PA_RTC.FPS);
		PA_OutputText(1, 1, 3, "FPS: %d", PA_RTC.FPS);
		PA_OutputText(1, 1, 5, "Bullet Num: %d", g_bulletNum);

		//In the beginning of the game, bullet should be increasing in 1/10 seconds.
		//After the bullet number met "BULLET_MIN", add a bullet every interval, about "BULLET_INCREASE_INTEVAL" seconds
		if (g_count%6==0 && g_bulletNum < BULLET_MIN) {
			vBulletInit(g_bulletNum);
			vCreateBullet(g_bulletNum);
			g_bulletNum++;
		}
		else if (g_count%(PA_RTC.FPS*BULLET_INCREASE_INTEVAL)==0 && g_bulletNum<=BULLET_MAX)
		{
			vBulletInit(g_bulletNum);
			vCreateBullet(g_bulletNum);
			g_bulletNum++;
		}

		vMovePlane();

		vMoveBullet(pGameState);

		if(bCheckCollision())
		{
			*pGameState = Game_Statistic;
			PA_SetSpriteAnim(g_screen, 0, 3);
			AS_MP3Stop();
			vSoundPlayExplode();
			vSaveFileData(*pGameState);
			return;
		}

		if(Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.X || Pad.Newpress.Y)
			*pGameState = Game_Pause;

		PA_WaitForVBL();
	}
}

void vGamePause(u8 *pGameState)
{
	if(Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.X || Pad.Newpress.Y)
		*pGameState = Game_Play;
	else
		*pGameState = Game_Pause;
}

/*********************************************************
  void vPlaneInit(void)
  Routine Description:
  initial plane.
Arguments:
void
Return Value:
None
 ***********************************************************/
static void vPlaneInit(void)
{
	// fix position with sprites size
	g_plane.x = (SCREEN_WIDTH/2)<<8;
	g_plane.y = (SCREEN_HEIGHT/2)<<8;

	PA_LoadSpritePal(0, 0, (void*)plane_Pal);
	g_plane_gfx = PA_CreateGfx(g_screen, (void*)plane_Sprite, OBJ_SIZE_16X16, 1);
	PA_CreateSpriteFromGfx  (g_screen, 0, g_plane_gfx,
			OBJ_SIZE_16X16, 1, 0,
			PLANEX, PLANEY);
}

/*********************************************************
  vMovePlane(void)
  Routine Description:
  handle plane moving, control the plane by Pad or Stylus
Arguments:
void
Return Value:
None
 ***********************************************************/
static void vMovePlane(void)
{
	s16 angle;

#ifdef STYLUS_CONTRL
	if (Stylus.Held)
	{
		angle = PA_GetAngle(PLANEX, PLANEY, Stylus.X, Stylus.Y);
		g_plane.x += PA_Cos(angle);
		g_plane.y -= PA_Sin(angle);

		if ((PA_Cos(angle)>>8)>= 0)// right, display 2
			PA_SetSpriteAnim(g_screen, 0, 2);
		else if ((PA_Cos(angle)>>8)< 0) // left, display 1
			PA_SetSpriteAnim(g_screen, 0, 1);
	}

	else if (Stylus.Released)
		PA_SetSpriteAnim(g_screen, 0, 0); // stop, display 0
#endif

	g_plane.x += (Pad.Held.Right - Pad.Held.Left)<<8;
	g_plane.y += (Pad.Held.Down - Pad.Held.Up)<<8;

	//show animate
	if (Pad.Newpress.Left)
		PA_SetSpriteAnim(g_screen, 0, 1);
	else if (Pad.Released.Left)
		PA_SetSpriteAnim(g_screen, 0, 0);

	if (Pad.Newpress.Right)
		PA_SetSpriteAnim(g_screen, 0, 2);
	else if (Pad.Released.Right)
		PA_SetSpriteAnim(g_screen, 0, 0);

	if (PLANEX <= 0)
		g_plane.x = 0;
	else if (PLANEX  >= (SCREEN_WIDTH-PLANE_W))
		g_plane.x = (SCREEN_WIDTH - PLANE_W)<<8;

	if (PLANEY <= 0)
		g_plane.y = 0;
	else if (PLANEY >= (SCREEN_HEIGHT-PLANE_H))
		g_plane.y = (SCREEN_HEIGHT - PLANE_H)<<8;

	PA_SetSpriteXY(g_screen, 0, PLANEX, PLANEY);
}

/********************************************
  bool bCheckCollision()
  Routine Description:
  check if the given bullent is collision with plane.
  Consider these sprites as circle.
Arguments:
void
Return Value:
bool, collision return true, no collision return false.
 ********************************************/
bool bCheckCollision()
{
	s16 ii;
	for (ii=0; ii<g_bulletNum; ii++)
	{
		if (PA_Distance(PLANEX+8, PLANEY+8, g_bullet[ii].x+4, g_bullet[ii].y+4) < 8*3)
			return TRUE;
	}

	return FALSE;
}

/********************************************
  void vBulletInit(Bullet *pBullet)
  Routine Description:
  initial a bullet

Arguments:
u8 uIndex; the index of bullet.
Return Value:
none
 ********************************************/
void vBulletInit(u8 uIndex)
{
#define UP      1
#define LEFT    2
#define DOWN    3
#define RIGHT   4
	u8 tmp;
	s16 angle;
	Bullet *pBullet = &g_bullet[uIndex];

	tmp = PA_RandMinMax(1, 4);

	switch(tmp)
	{
		case UP: // y = 0
			pBullet->y = 0;
			pBullet->x = PA_RandMinMax(0, SCREEN_WIDTH);
			break;
		case LEFT: // x = 0
			pBullet->x = 0;
			pBullet->y = PA_RandMinMax(0, SCREEN_HEIGHT);
			break;
		case DOWN: // y = max
			pBullet->x = PA_RandMinMax(0, SCREEN_WIDTH);
			pBullet->y = SCREEN_HEIGHT - 4;
			break;
		case RIGHT: // x = max
			pBullet->x = SCREEN_WIDTH - 4;
			pBullet->y = PA_RandMinMax(0, SCREEN_HEIGHT);
			break;
		default:
			break;
	}

	//bullet type
#if 0
	pBullet->type = PA_RandMinMax(0, 2);
#else
	pBullet->type = Bullet_Normal;
#endif

	angle = PA_GetAngle(pBullet->x, pBullet->y, PLANEX, PLANEY);
	pBullet->vx = PA_Cos(angle)>>8;
	pBullet->vy = -PA_Sin(angle)>>8;
}

void vBulletInitAll(void)
{
	u8 uIndex;

	PA_LoadSpritePal(0, 1, (void*)bullet_normal_Pal);
	g_bullet_normal_gfx = PA_CreateGfx(g_screen, (void*)bullet_normal_Sprite, OBJ_SIZE_8X8, 1);

#if 0
	PA_LoadSpritePal(0, 1, (void*)bullet_hunter_Pal);
	g_bullet_hunter_gfx = PA_CreateGfx(g_screen, (void*)bullet_hunter_Sprite, OBJ_SIZE_8X8, 1);

	PA_LoadSpritePal(0, 1, (void*)bullet_explorer_Pal);
	g_bullet_explode_gfx = PA_CreateGfx(g_screen, (void*)bullet_explorer_Sprite, OBJ_SIZE_8X8, 1);
#endif

	for (uIndex=0; uIndex<g_bulletNum; uIndex++)
	{
		vBulletInit(uIndex);
		vCreateBullet(uIndex);
	}
}

void vMoveBullet(u8 *pGameState)
{
	u16 uIndex;

	for (uIndex=0; uIndex<g_bulletNum; uIndex++)
	{
		if(g_bullet[uIndex].x >= SCREEN_WIDTH || g_bullet[uIndex].x <= 0
				|| g_bullet[uIndex].y >= SCREEN_HEIGHT || g_bullet[uIndex].y <= 0) {
			vBulletInit(uIndex);
		}

		switch(g_bullet[uIndex].type) {
			case Bullet_Normal:
				g_bullet[uIndex].x += g_bullet[uIndex].vx;
				g_bullet[uIndex].y += g_bullet[uIndex].vy;
				break;
			case Bullet_Hunter:
			case Bullet_Explode:
			default:
				break;
		}

		PA_SetSpriteXY(g_screen, uIndex+1, g_bullet[uIndex].x, g_bullet[uIndex].y);
	}
}

void vDestructSprites(void)
{
	u16 uIndex;

	// delete all sprites, plane is 0, and bullet is 1-BULLET_MAX
	for (uIndex = 0; uIndex <= g_bulletNum; uIndex++)
	{
		PA_DeleteSprite(g_screen, uIndex);
	}

	g_count = 0;
	g_bulletNum = BULLET_MIN;
}

void vGameStatistic(u8 *pGameState)
{
	vShowScore();

	while(1)
	{
		if (Pad.Newpress.B)
		{
			*pGameState = Menu_Init;
			PA_ClearTextBg(0);
			PA_ClearTextBg(1);
			return;
		}
		else if (Pad.Newpress.X)
		{
			*pGameState = Game_Init;
			PA_ClearTextBg(0);
			PA_ClearTextBg(1);
			return;
		}

		PA_WaitForVBL();
	}
}

#ifdef SAVE_DATA
/**************************
 *    szBufLen <= 32 bytes
 *
 **************************/
void vInputName(char* szBuf, int *iLength)
{
	int nletter = 0;
	char letter = 0;

	PA_InitText(1, 0);
	PA_InitText(0, 0);

	PA_InitKeyboard(2); // Initialize KeyBoard
	PA_KeyboardIn(20, 95);
	PA_OutputSimpleText(0, 1, 3, "Please input your name: ");

	while (1)
	{
		letter = PA_CheckKeyboard();

		if(nletter > 32)
			break;

		if (letter > 31)
		{
			szBuf[nletter] = letter;
			nletter++;
		}
		else if ((letter == PA_BACKSPACE)&&nletter) // Backspace pressed
		{
			nletter--;
			szBuf[nletter] = ' '; // Erase the last letter
		}
		else if ((letter == '\n' || Pad.Newpress.A) && nletter!=0) // Enter pressed
			break;

		PA_OutputSimpleText(0, 1, 4, szBuf); // Write the text
		PA_WaitForVBL();
	}

	PA_KeyboardOut();
	PA_ClearTextBg(0);

	*iLength = nletter;
}

void vShowScore(void)
{
	Save sData;
	int  ii;

	PA_ClearTextBg(1);

	PA_OutputSimpleText(1, 12, 3,"Top 10");
	PA_OutputSimpleText(1, 1, 4,"-----------------------------");

	if (0 == iLoadData(&sData))
	{
		for (ii=0; ii<MAX_SAVE_ITEM; ii++)
		{
			PA_OutputText(1, 5, ii+6, "%02d, %s, %d.%02ds, %d\n", ii+1,
					sData.top[ii].szUsername,
					sData.top[ii].count/60,
					sData.top[ii].count%60,
					sData.top[ii].bulletNum);
		}
	}
	else
	{
		PA_OutputSimpleText(1, 11, 8,"ERROR!!!");
	}

	PA_OutputSimpleText(1, 0, 22, "Press <B> to Main Menu..");
	PA_OutputSimpleText(1, 0, 23, "Press <X> to Restart..");
}
#endif

void vSaveFileData(u8 *pGameState)
{
	Score score;
	int position;
	int iLength;
	char szBuf[60];

	score.count = g_count;
	score.bulletNum = g_bulletNum;

	vDestructSprites();
	PA_WaitForVBL();

	position = getPosition(score.count, score.bulletNum);

	if (iShowResult(position, score) == 1) {
		*pGameState = Game_Init;
		return;
	}

	if (position>= 0 && position < MAX_SAVE_ITEM) {
		memset(szBuf, 0, 60);

		vInputName(szBuf, &iLength); //max length < 32 bytes
		memcpy(score.szUsername, szBuf, iLength);

		//show congratulation message
		position = iSaveData(&score);
	}
}

int iShowResult(int position, Score score)
{
	PA_InitText(0, 0);

	if (position>= 0 && position < MAX_SAVE_ITEM)
		PA_OutputSimpleText(0, 8, 9, "congratulations!");
	else
		PA_OutputSimpleText(0, 8, 9, "Sorry.U are out!");

	PA_OutputText(0, 10, 11, "Time: %d.%02ds", score.count/60, score.count%60);
	PA_OutputText(0, 10, 12, "Bullet: %d", score.bulletNum);
	PA_OutputSimpleText(0, 0, 22, "Press <X> to restart game.");
	PA_OutputSimpleText(0, 0, 23, "Press <Anykey> to see score list.");

	PA_WaitForVBL();

	while(1)
	{
		if (Pad.Newpress.X) {
			return 1;
		}
		if (Pad.Newpress.Anykey)
			return 0;
		PA_WaitForVBL();
	}
}

void vCreateBullet(u8 index)
{
	switch(g_bullet[index].type) {
	case Bullet_Normal:
		PA_CreateSpriteFromGfx(g_screen, index+1, g_bullet_normal_gfx,
				OBJ_SIZE_8X8, 1, 1,
				g_bullet[index].x, g_bullet[index].y);
		break;
#if 0
	case Bullet_Hunter:
		PA_CreateSpriteFromGfx(g_screen, index+1, g_bullet_hunter_gfx,
				OBJ_SIZE_8X8, 1, 1,
				g_bullet[index].x, g_bullet[index].y);
		break;

	case Bullet_Explode:
		PA_CreateSpriteFromGfx(g_screen, index+1, g_bullet_explode_gfx,
				OBJ_SIZE_8X8, 1, 1,
				g_bullet[index].x, g_bullet[index].y);
		break;
#endif
	default:
		break;
	}
}

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

Plane g_Plane;
Bullet g_bullet[BULLET_MAX];
u16 g_bullet_gfx;
u16 g_plane_gfx;

/************** Function *****************/
static void vPlaneInit(void);
static void vMovePlane(void);
bool bCheckCollision();
void vBulletInit(u16 uIndex);
//bool bCheckCollision();
inline bool bCheckCollisionbyIndex(u16 ii);
void vMoveBullet(u8 *pGameState);
void vBulletInitAll(void);
void vDestructSprites(void);

s32 iGameInit(u8 *pGameState)
{
    g_count = 0;
    // load backgroud
    PA_EasyBgLoad(g_screen, 0, bg);
    //PA_EasyBgLoad(1, 0, bg);

    vPlaneInit();

    memset(g_bullet, 0, sizeof(Bullet)*BULLET_MAX);
    vBulletInitAll();

    *pGameState = Game_Play;
    return 0;
}

void vGamePlay(u8 *pGameState)
{
    u8 ii;

    g_count++;
    PA_OutputText(1, 1, 1, "%d.%02ds", g_count/PA_RTC.FPS, g_count%PA_RTC.FPS);
    PA_OutputText(1, 1, 2, "FPS: %d", PA_RTC.FPS);    
    PA_OutputText(1, 1, 3, "Bullet Num: %d", g_bulletNum);

    //add a bullet every interval, about 5 seconds
    if (g_count%(PA_RTC.FPS*BULLET_INCREASE_INTEVAL)==0 && g_bulletNum<=BULLET_MAX)
    {
        vBulletInit(g_bulletNum);
        PA_CreateSpriteFromGfx(g_screen, g_bulletNum+1, g_bullet_gfx,
                OBJ_SIZE_8X8, 1, 1,
                g_bullet[g_bulletNum].x, g_bullet[g_bulletNum].y);
        g_bulletNum++;
    }

    vMovePlane();
    vMoveBullet(pGameState);

//    if(bCheckCollision())
//    {
//        PA_SetSpriteAnim(g_screen, 0, 3);
//        vSoundPlayExplode();
//        *pGameState = Game_Statis;
//        return;
//    }

    if(Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.X || Pad.Newpress.Y)
        *pGameState = Game_Pause;

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
    g_Plane.x = SCREEN_WIDTH/2;
    g_Plane.y = SCREEN_HEIGHT/2;

    PA_LoadSpritePal(0, 0, (void*)plane_Pal);
    g_plane_gfx = PA_CreateGfx(g_screen, (void*)plane_Sprite, OBJ_SIZE_16X16, 1);
    PA_CreateSpriteFromGfx  (g_screen, 0, g_plane_gfx,
            OBJ_SIZE_16X16, 1, 0,
            g_Plane.x, g_Plane.y);
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

    g_Plane.x += Pad.Held.Right - Pad.Held.Left;
    g_Plane.y += Pad.Held.Down - Pad.Held.Up;

    //show animate
    if (Pad.Newpress.Left)
        PA_SetSpriteAnim(g_screen, 0, 1);
    else if (Pad.Released.Left)
        PA_SetSpriteAnim(g_screen, 0, 0);

    if (Pad.Newpress.Right)
        PA_SetSpriteAnim(g_screen, 0, 2);
    else if (Pad.Released.Right)
        PA_SetSpriteAnim(g_screen, 0, 0);

#ifdef STYLUS_CONTRL
    // follow the Stylus
    if (Stylus.Held)
    {
        angle = PA_GetAngle(g_Plane.x+8, g_Plane.y+8, Stylus.X, Stylus.Y);
        g_Plane.x += (PA_Cos(angle)>>8);
        g_Plane.y -= (PA_Sin(angle)>>8);

        if ((PA_Cos(angle)>>8)> 0)// right, display 2
            PA_SetSpriteAnim(g_screen, 0, 2);
        else if ((PA_Cos(angle)>>8)< 0) // left, display 1
            PA_SetSpriteAnim(g_screen, 0, 1);
    }
    else if (Stylus.Released)
        PA_SetSpriteAnim(g_screen, 0, 0); // stop, display 0
#endif

    if (g_Plane.x <= 0)
        g_Plane.x = 0;
    else if ((g_Plane.x+ PLANE_W) > SCREEN_WIDTH)
        g_Plane.x = SCREEN_WIDTH - PLANE_W;

    if (g_Plane.y <= 0)
        g_Plane.y = 0;
    else if ((g_Plane.y+PLANE_H) > SCREEN_HEIGHT-1)
        g_Plane.y = SCREEN_HEIGHT - PLANE_H;

    PA_SetSpriteXY(g_screen, 0, g_Plane.x, g_Plane.y);
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
//bool bCheckCollision()
//{
//    s16 ii;
//    for (ii=0; ii<g_bulletNum; ii++)
//    {
//        if (PA_Distance(g_Plane.x+8, g_Plane.y+8, g_bullet[ii].x+4, g_bullet[ii].y+4) < 8*2)
//            return TRUE;
//    }
//
//    return FALSE;
//}

/********************************************
    void vBulletInit(Bullet *pBullet)
Routine Description:
    initial a bullet

Arguments:
    u16 uIndex; the index of bullet.
Return Value:
    none
********************************************/
void vBulletInit(u16 uIndex)
{
#define UP      1
#define LEFT    2
#define DOWN    3
#define RIGHT   4
    u8 tmp;
    s16 angle;
    Bullet *pBullet = &g_bullet[uIndex];

    tmp = PA_RandMinMax(1,4);

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

    angle = PA_GetAngle(pBullet->x, pBullet->y, g_Plane.x, g_Plane.y);
    pBullet->vx = PA_Cos(angle)>>8;
    pBullet->vy = -PA_Sin(angle)>>8;
}

void vBulletInitAll(void)
{
    u16 uIndex;

    PA_LoadSpritePal(0, 1, (void*)bullet_Pal);
    g_bullet_gfx = PA_CreateGfx(g_screen, (void*)bullet_Sprite, OBJ_SIZE_8X8, 1);

    for (uIndex=0; uIndex<g_bulletNum; uIndex++)
    {
        vBulletInit(uIndex);
        PA_CreateSpriteFromGfx(g_screen, uIndex+1, g_bullet_gfx,
                    OBJ_SIZE_8X8, 1, 1,
                    g_bullet[uIndex].x, g_bullet[uIndex].y);
    }
}

inline bool bCheckCollisionbyIndex(u16 ii)
{
    if (PA_Distance(g_Plane.x+8, g_Plane.y+8, g_bullet[ii].x+4, g_bullet[ii].y+4) < 8*2)
        return TRUE;
    else
        return FALSE;
}

void vMoveBullet(u8 *pGameState)
{
    u16 uIndex;

    for (uIndex=0; uIndex<g_bulletNum; uIndex++)
    {
        if(g_bullet[uIndex].x >= SCREEN_WIDTH || g_bullet[uIndex].x <= 0
            || g_bullet[uIndex].y >= SCREEN_HEIGHT || g_bullet[uIndex].y <= 0)
            vBulletInit(uIndex);
        g_bullet[uIndex].x += g_bullet[uIndex].vx;
        g_bullet[uIndex].y += g_bullet[uIndex].vy;
        PA_SetSpriteXY(g_screen, uIndex+1, g_bullet[uIndex].x, g_bullet[uIndex].y);
        
        if (bCheckCollisionbyIndex(uIndex))
        {
            PA_SetSpriteAnim(g_screen, 0, 3);
            vSoundPlayExplode();
            *pGameState = Game_Statis;
            return;
        }
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
}

void vGameStatic(u8 *pGameState)
{
    if(Pad.Newpress.Anykey || Stylus.Newpress)
    {
        vDestructSprites();
        g_count = 0;
        g_bulletNum = BULLET_MIN;

        *pGameState = Game_Init;
    }
}
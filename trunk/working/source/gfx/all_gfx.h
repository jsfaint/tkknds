//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char plane_Sprite[512] __attribute__ ((aligned (4))) ;  // Pal : plane_Pal
extern const unsigned char bullet_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : bullet_Pal

// Background files : 
extern const int bg_Info[3]; // BgMode, Width, Height
extern const unsigned short bg_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bg_Pal
extern const unsigned char bg_Tiles[4288] __attribute__ ((aligned (4))) ;  // Pal : bg_Pal
extern PAGfx_struct bg; // background pointer


// Palette files : 
extern const unsigned short plane_Pal[11] __attribute__ ((aligned (4))) ;
extern const unsigned short bullet_Pal[18] __attribute__ ((aligned (4))) ;
extern const unsigned short bg_Pal[15] __attribute__ ((aligned (4))) ;


#endif


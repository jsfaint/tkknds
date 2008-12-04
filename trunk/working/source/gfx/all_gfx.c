//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Sprite files : 
#include "plane.c"
#include "bullet.c"

// Background files : 
#include "bg.c"

// Palette files : 
#include "plane.pal.c"
#include "bullet.pal.c"
#include "bg.pal.c"

// Background Pointers :
PAGfx_struct bg = {(void*)bg_Map, 768, (void*)bg_Tiles, 4288, (void*)bg_Pal, (int*)bg_Info };


#ifdef __cplusplus
}
#endif


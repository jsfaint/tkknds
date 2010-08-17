#ifndef __SAVE_H__
#define __SAVE_H__

#include "header.h"

#define SUCCESS 0
#define FAILED  -1
/************ Macro *************/
#define MAX_SAVE_ITEM     10
#define MAX_NAME_LEN    32
#define SAVE_SIGNATURE "TKKNDS"
#define FILENAME "fat:/TKKNDS.dat"

/************* Structure ************/
struct {
	unsigned char szUsername[MAX_NAME_LEN];
	unsigned long count;
	unsigned char bulletNum;
} Score;

struct {
	unsigned char sound;
	unsigned char music;
} Option;

typedef struct {
	struct Score   top[MAX_SAVE_ITEM];
	struct Option option;
} Save, *PSave;

/*************Function**************/
int iCheckData(void); //if return failed, default the data.
int getPosition(long count, int bulletNum);
int iLoadData(PSave psData);
int iSaveData(Score *pScore);

#endif //__SAVE_H__

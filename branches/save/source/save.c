/*++
Copyright ? 2008
Module Name:
    save.c
Abstract:
    This file is the implement of game date handle function.
Author:
    jason (jsfaint@gmail.com) 2008-12-10
--*/
#include "save.h"


void vWriteDefault();
int iWritetoFile(Save sData);
int iInsertData(Score *pScore,  int *position);//insert the data

void vWriteDefault()
{
    FILE* fd;
    int   ret;
    Save  sData;
    int   iIndex;

    memset(&sData, 0, sizeof(Save));

    for(iIndex=0; iIndex<MAX_SAVE_ITEM; iIndex++)
    {
        sData.top[iIndex].count = 0;
        memcpy(sData.top[iIndex].szUsername, "none", sizeof("none"));
        sData.top[iIndex].bulletNum = 0;
    }

    fd = fopen("TKKNDS.dat", "wb");
    if (fd != NULL)
    {
        ret = fwrite(SAVE_SIGNATURE, sizeof(SAVE_SIGNATURE), 1, fd);
        ret = fwrite(&sData, sizeof(Save), 1, fd);
    }

    fclose(fd);
}

int iCheckData(void)
{
    char signature[sizeof(SAVE_SIGNATURE)];
    FILE* fd;
    int ret = 0;


    fd = fopen("TKKNDS.dat", "rb");
    if (fd != NULL)
    {
        ret = fread(signature, sizeof(SAVE_SIGNATURE), 1, fd);
        fclose(fd);

        if (strcmp(signature, SAVE_SIGNATURE) != 0)
        {
            vWriteDefault();
            return FAILED;
        }
        else
            return SUCCESS;
    }
    else
        vWriteDefault();

    return FAILED;
}


int getPosition(long count, int bulletNum)
{
    Save sData;
    int ii = 0, tmp;

    if(FAILED == iLoadData(&sData))
        return FAILED;

    for (ii=0; ii<MAX_SAVE_ITEM; ii++)
    {
        if (count > sData.top[ii].count) //compare count first
            return ii;
        else if (count == sData.top[ii].count) //if count equal, compare bullet
            if (bulletNum > sData.top[ii].bulletNum)
                return ii;
    }
}

int iLoadData(PSave psData)
{
    char signature[sizeof(SAVE_SIGNATURE)];
    FILE* fd;
    int ret = 0;

    memset(psData, 0, sizeof(Save));
    fd = fopen("TKKNDS.dat", "rb");
    if (fd != NULL)
    {
        ret = fread(signature, sizeof(SAVE_SIGNATURE), 1, fd);
        ret = fread(psData, sizeof(Save), 1, fd);
        fclose(fd);

        return SUCCESS;
    }

    fclose(fd);
    return FAILED;
}

int iWritetoFile(Save sData)
{
    char signature[sizeof(SAVE_SIGNATURE)];
    FILE* fd = NULL;
    int ret = 0;

    if (g_bLibfat) {
        fd = fopen("TKKNDS.dat", "wb");
        if (fd != NULL) {
            ret = fwrite(SAVE_SIGNATURE, sizeof(SAVE_SIGNATURE), 1, fd);
            ret = fwrite(&sData, sizeof(Save), 1, fd);
            fclose(fd);
            return SUCCESS;
        }
    }

    return FAILED;
}

int iInsertData(Score *pScore, int *position)
{
    Save sData;
    Score score;
    int ii = 0, tmp;

    memcpy(&score, pScore, sizeof(Score));

    if(FAILED == iLoadData(&sData))
        return FAILED;

    for (ii=0; ii<MAX_SAVE_ITEM; ii++)
    {
        if (score.count > sData.top[ii].count) //compare count first
        {
            tmp = ii;
            break;
        }
        else if (score.count == sData.top[ii].count) //if count equal, compare bullet
        {
            if (score.bulletNum > sData.top[ii].bulletNum)
            {
                tmp = ii;
                break;
            }
        }
    }

    if (ii >= MAX_SAVE_ITEM)
        return FAILED;

    // move forward
    for (ii = MAX_SAVE_ITEM-1; ii>tmp; ii--)
        memcpy(&sData.top[ii], &sData.top[ii-1], sizeof(Score));

    // insert
    memcpy(&sData.top[tmp], &score, sizeof(Score));

    if (SUCCESS == iWritetoFile(sData))
    {
       *position = tmp;
        return SUCCESS;
    }

    return FAILED;
}

int iSaveData(Score *pScore)
{
    int position;

    if(SUCCESS == iInsertData(pScore,  &position))
        return position;
    else
        return MAX_SAVE_ITEM;
}


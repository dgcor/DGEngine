/*****************************************************************************/
/* SFileGetFileInfo.cpp                   Copyright (c) Ladislav Zezula 2013 */
/*---------------------------------------------------------------------------*/
/* Description:                                                              */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 30.11.13  1.00  Lad  The first version of SFileGetFileInfo.cpp            */
/*****************************************************************************/

#define __STORMLIB_SELF__
#include "StormLib.h"
#include "StormCommon.h"

//-----------------------------------------------------------------------------
// Local functions

static bool GetInfo_ReturnError(DWORD dwErrCode)
{
    SetLastError(dwErrCode);
    return false;
}

static bool GetInfo_BufferCheck(void * pvFileInfo, DWORD cbFileInfo, DWORD cbData, LPDWORD pcbLengthNeeded)
{
    // Give the length needed to store the info
    if(pcbLengthNeeded != NULL)
        pcbLengthNeeded[0] = cbData;

    // Check for sufficient buffer
    if(cbData > cbFileInfo)
        return GetInfo_ReturnError(ERROR_INSUFFICIENT_BUFFER);

    // If the buffer size is sufficient, check for valid user buffer
    if(pvFileInfo == NULL)
        return GetInfo_ReturnError(ERROR_INVALID_PARAMETER);

    // Buffers and sizes are OK, we are ready to proceed file copying
    return true;
}

static bool GetInfo(void * pvFileInfo, DWORD cbFileInfo, const void * pvData, DWORD cbData, LPDWORD pcbLengthNeeded)
{
    // Verify buffer pointer and buffer size
    if(!GetInfo_BufferCheck(pvFileInfo, cbFileInfo, cbData, pcbLengthNeeded))
        return false;

    // Copy the data to the caller-supplied buffer
    memcpy(pvFileInfo, pvData, cbData);
    return true;
}

//-----------------------------------------------------------------------------
// Retrieves an information about an archive or about a file within the archive
//
//  hMpqOrFile - Handle to an MPQ archive or to a file
//  InfoClass  - Information to obtain
//  pvFileInfo - Pointer to buffer to store the information
//  cbFileInfo - Size of the buffer pointed by pvFileInfo
//  pcbLengthNeeded - Receives number of bytes necessary to store the information

bool WINAPI SFileGetFileInfo(
    HANDLE hMpqOrFile,
    SFileInfoClass InfoClass,
    void * pvFileInfo,
    DWORD cbFileInfo,
    LPDWORD pcbLengthNeeded)
{
    TMPQArchive * ha = NULL;
    TFileEntry * pFileEntry = NULL;
    TMPQFile * hf = NULL;

    // Validate archive/file handle
    if((int)InfoClass <= (int)SFileMpqFlags)
    {
        if((ha = IsValidMpqHandle(hMpqOrFile)) == NULL)
            return GetInfo_ReturnError(ERROR_INVALID_HANDLE);
    }
    else
    {
        if((hf = IsValidFileHandle(hMpqOrFile)) == NULL)
            return GetInfo_ReturnError(ERROR_INVALID_HANDLE);
        pFileEntry = hf->pFileEntry;
    }

    // Return info-class-specific data
    switch(InfoClass)
    {
        case SFileInfoFileTime:
            return GetInfo(pvFileInfo, cbFileInfo, &pFileEntry->FileTime, sizeof(ULONGLONG), pcbLengthNeeded);

        case SFileInfoFileSize:
            return GetInfo(pvFileInfo, cbFileInfo, &pFileEntry->dwFileSize, sizeof(DWORD), pcbLengthNeeded);

        default:
            break;
    }

    // Invalid info class
    return GetInfo_ReturnError(ERROR_INVALID_PARAMETER);
}

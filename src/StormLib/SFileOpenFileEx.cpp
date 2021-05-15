/*****************************************************************************/
/* SFileOpenFileEx.cpp                    Copyright (c) Ladislav Zezula 2003 */
/*---------------------------------------------------------------------------*/
/* Description :                                                             */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* xx.xx.99  1.00  Lad  The first version of SFileOpenFileEx.cpp             */
/*****************************************************************************/

#define __STORMLIB_SELF__
#include "StormLib.h"
#include "StormCommon.h"

/*****************************************************************************/
/* Public functions                                                          */
/*****************************************************************************/

//-----------------------------------------------------------------------------
// SFileOpenFileEx
//
//   hMpq          - Handle of opened MPQ archive
//   szFileName    - Name of file to open
//   dwSearchScope - Where to search
//   PtrFile       - Pointer to store opened file handle

bool WINAPI SFileOpenFileEx(HANDLE hMpq, const char * szFileName, DWORD dwSearchScope, HANDLE * PtrFile)
{
    TMPQArchive * ha = IsValidMpqHandle(hMpq);
    TFileEntry  * pFileEntry = NULL;
    TMPQFile    * hf = NULL;
    DWORD dwHashIndex = HASH_ENTRY_FREE;
    DWORD dwFileIndex = 0;
    bool bOpenByIndex = false;
    int nError = ERROR_SUCCESS;

    // Don't accept NULL pointer to file handle
    if(szFileName == NULL || *szFileName == 0)
        nError = ERROR_INVALID_PARAMETER;

    // When opening a file from MPQ, the handle must be valid
    if(dwSearchScope != SFILE_OPEN_LOCAL_FILE && ha == NULL)
        nError = ERROR_INVALID_HANDLE;

    // When not checking for existence, the pointer to file handle must be valid
    if(dwSearchScope != SFILE_OPEN_CHECK_EXISTS && PtrFile == NULL)
        nError = ERROR_INVALID_PARAMETER;

    // Prepare the file opening
    if(nError == ERROR_SUCCESS)
    {
        switch(dwSearchScope)
        {
            case SFILE_OPEN_FROM_MPQ:
            case SFILE_OPEN_BASE_FILE:
            case SFILE_OPEN_CHECK_EXISTS:

                // If this MPQ has no patches, open the file from this MPQ directly
                if(ha->haPatch == NULL || dwSearchScope == SFILE_OPEN_BASE_FILE)
                {
                    pFileEntry = GetFileEntryLocale2(ha, szFileName, lcFileLocale, &dwHashIndex);
                }

                // If this MPQ is a patched archive, open the file as patched
                else
                {
                    nError = ERROR_INVALID_PARAMETER;
                    break;
                }
                break;

            case SFILE_OPEN_ANY_LOCALE:

                nError = ERROR_INVALID_PARAMETER;
                break;

            case SFILE_OPEN_LOCAL_FILE:

                nError = ERROR_INVALID_PARAMETER;
                break;

            default:

                // Don't accept any other value
                nError = ERROR_INVALID_PARAMETER;
                break;
        }
    }

    // Check whether the file really exists in the MPQ
    if(nError == ERROR_SUCCESS)
    {
        // If we didn't find the file, try to open it using pseudo file name ("File
        if (pFileEntry == NULL || (pFileEntry->dwFlags & MPQ_FILE_EXISTS) == 0)
        {
            // Check the pseudo-file name ("File00000001.ext")
            if ((bOpenByIndex = IsPseudoFileName(szFileName, &dwFileIndex)) == true)
            {
                // Get the file entry for the file
                if (dwFileIndex < ha->dwFileTableSize)
                {
                    pFileEntry = ha->pFileTable + dwFileIndex;
                }
            }

            // Still not found?
            if (pFileEntry == NULL)
            {
                nError = ERROR_FILE_NOT_FOUND;
            }
        }

        // Perform some checks of invalid files
        if (pFileEntry != NULL)
        {
            // MPQ protectors use insanely amount of fake files, often with very high size.
            // We won't open any files whose compressed size is bigger than archive size
            // If the file is not compressed, its size cannot be bigger than archive size
            if ((pFileEntry->dwFlags & MPQ_FILE_COMPRESS_MASK) == 0 && (pFileEntry->dwFileSize > ha->FileSize))
            {
                nError = ERROR_FILE_CORRUPT;
                pFileEntry = NULL;
            }

            // Ignore unknown loading flags (example: MPQ_2016_v1_WME4_4.w3x)
//          if(pFileEntry->dwFlags & ~MPQ_FILE_VALID_FLAGS)
//          {
//              nError = ERROR_NOT_SUPPORTED;
//              pFileEntry = NULL;
//          }
        }
    }

    // Did the caller just wanted to know if the file exists?
    if(nError == ERROR_SUCCESS && dwSearchScope != SFILE_OPEN_CHECK_EXISTS)
    {
        // Allocate file handle
        hf = CreateFileHandle(ha, pFileEntry);
        if(hf != NULL)
        {
            // Get the hash index for the file
            if(dwHashIndex != HASH_ENTRY_FREE)
                hf->pHashEntry = ha->pHashTable + dwHashIndex;
            hf->dwHashIndex = dwHashIndex;

            // If the MPQ has sector CRC enabled, enable if for the file
            if(ha->dwFlags & MPQ_FLAG_CHECK_SECTOR_CRC)
                hf->bCheckSectorCRCs = true;

            // If we know the real file name, copy it to the file entry
            if(bOpenByIndex == false)
            {
                // If there is no file name yet, allocate it
                AllocateFileName(ha, pFileEntry, szFileName);

                // If the file is encrypted, we should detect the file key
                if(pFileEntry->dwFlags & MPQ_FILE_ENCRYPTED)
                {
                    hf->dwFileKey = DecryptFileKey(szFileName,
                                                   pFileEntry->ByteOffset,
                                                   pFileEntry->dwFileSize,
                                                   pFileEntry->dwFlags);
                }
            }
        }
        else
        {
            nError = ERROR_NOT_ENOUGH_MEMORY;
        }
    }

    // Give the file entry
    if(PtrFile != NULL)
        PtrFile[0] = hf;

    // Return error code
    if(nError != ERROR_SUCCESS)
        SetLastError(nError);
    return (nError == ERROR_SUCCESS);
}

//-----------------------------------------------------------------------------
// bool WINAPI SFileCloseFile(HANDLE hFile);

bool WINAPI SFileCloseFile(HANDLE hFile)
{
    TMPQFile * hf = (TMPQFile *)hFile;

    if(!IsValidFileHandle(hFile))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    // Free the structure
    FreeFileHandle(hf);
    return true;
}

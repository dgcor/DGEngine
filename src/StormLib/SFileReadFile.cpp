/*****************************************************************************/
/* SFileReadFile.cpp                      Copyright (c) Ladislav Zezula 2003 */
/*---------------------------------------------------------------------------*/
/* Description :                                                             */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* xx.xx.99  1.00  Lad  The first version of SFileReadFile.cpp               */
/* 24.03.99  1.00  Lad  Added the SFileGetFileInfo function                  */
/*****************************************************************************/

#define __STORMLIB_SELF__
#include "StormLib.h"
#include "StormCommon.h"

//-----------------------------------------------------------------------------
// Local functions

//  hf            - MPQ File handle.
//  pbBuffer      - Pointer to target buffer to store sectors.
//  dwByteOffset  - Position of sector in the file (relative to file begin)
//  dwBytesToRead - Number of bytes to read. Must be multiplier of sector size.
//  pdwBytesRead  - Stored number of bytes loaded
static int ReadMpqSectors(TMPQFile * hf, LPBYTE pbBuffer, DWORD dwByteOffset, DWORD dwBytesToRead, LPDWORD pdwBytesRead)
{
    ULONGLONG RawFilePos;
    TMPQArchive * ha = hf->ha;
    TFileEntry * pFileEntry = hf->pFileEntry;
    LPBYTE pbRawSector = NULL;
    LPBYTE pbOutSector = pbBuffer;
    LPBYTE pbInSector = pbBuffer;
    DWORD dwRawBytesToRead;
    DWORD dwRawSectorOffset = dwByteOffset;
    DWORD dwSectorsToRead = dwBytesToRead / ha->dwSectorSize;
    DWORD dwSectorIndex = dwByteOffset / ha->dwSectorSize;
    DWORD dwSectorsDone = 0;
    DWORD dwBytesRead = 0;
    int nError = ERROR_SUCCESS;

    // Note that dwByteOffset must be aligned to size of one sector
    // Note that dwBytesToRead must be a multiplier of one sector size
    // This is local function, so we won't check if that's true.
    // Note that files stored in single units are processed by a separate function

    // If there is not enough bytes remaining, cut dwBytesToRead
    if((dwByteOffset + dwBytesToRead) > hf->dwDataSize)
        dwBytesToRead = hf->dwDataSize - dwByteOffset;
    dwRawBytesToRead = dwBytesToRead;

    // Perform all necessary work to do with compressed files
    if(pFileEntry->dwFlags & MPQ_FILE_COMPRESS_MASK)
    {
        // If the sector positions are not loaded yet, do it
        if(hf->SectorOffsets == NULL)
        {
            nError = AllocateSectorOffsets(hf, true);
            if(nError != ERROR_SUCCESS || hf->SectorOffsets == NULL)
                return nError;
        }

        // TODO: If the raw data MD5s are not loaded yet, load them now
        // Only do it if the MPQ is of format 4.0
//      if(ha->pHeader->wFormatVersion >= MPQ_FORMAT_VERSION_4 && ha->pHeader->dwRawChunkSize != 0)
//      {
//          nError = AllocateRawMD5s(hf, true);
//          if(nError != ERROR_SUCCESS)
//              return nError;
//      }

        // Assign the temporary buffer as target for read operation
        dwRawSectorOffset = hf->SectorOffsets[dwSectorIndex];
        dwRawBytesToRead = hf->SectorOffsets[dwSectorIndex + dwSectorsToRead] - dwRawSectorOffset;

        // If the file is compressed, also allocate secondary buffer
        pbInSector = pbRawSector = STORM_ALLOC(BYTE, dwRawBytesToRead);
        if(pbRawSector == NULL)
            return ERROR_NOT_ENOUGH_MEMORY;
    }

    // Calculate raw file offset where the sector(s) are stored.
    RawFilePos = CalculateRawSectorOffset(hf, dwRawSectorOffset);

    // Set file pointer and read all required sectors
    if(FileStream_Read(ha->pStream, &RawFilePos, pbInSector, dwRawBytesToRead))
    {
        // Now we have to decrypt and decompress all file sectors that have been loaded
        for(DWORD i = 0; i < dwSectorsToRead; i++)
        {
            DWORD dwRawBytesInThisSector = ha->dwSectorSize;
            DWORD dwBytesInThisSector = ha->dwSectorSize;
            DWORD dwIndex = dwSectorIndex + i;

            // If there is not enough bytes in the last sector,
            // cut the number of bytes in this sector
            if(dwRawBytesInThisSector > dwBytesToRead)
                dwRawBytesInThisSector = dwBytesToRead;
            if(dwBytesInThisSector > dwBytesToRead)
                dwBytesInThisSector = dwBytesToRead;

            // If the file is compressed, we have to adjust the raw sector size
            if(pFileEntry->dwFlags & MPQ_FILE_COMPRESS_MASK)
                dwRawBytesInThisSector = hf->SectorOffsets[dwIndex + 1] - hf->SectorOffsets[dwIndex];

            // If the file is encrypted, we have to decrypt the sector
            if(pFileEntry->dwFlags & MPQ_FILE_ENCRYPTED)
            {
                BSWAP_ARRAY32_UNSIGNED(pbInSector, dwRawBytesInThisSector);

                // If we don't know the key, try to detect it by file content
                if(hf->dwFileKey == 0)
                {
                    nError = ERROR_UNKNOWN_FILE_KEY;
                    break;
                }

                DecryptMpqBlock(pbInSector, dwRawBytesInThisSector, hf->dwFileKey + dwIndex);
                BSWAP_ARRAY32_UNSIGNED(pbInSector, dwRawBytesInThisSector);
            }

            // If the sector is really compressed, decompress it.
            // WARNING : Some sectors may not be compressed, it can be determined only
            // by comparing uncompressed and compressed size !!!
            if(dwRawBytesInThisSector < dwBytesInThisSector)
            {
                int cbOutSector = dwBytesInThisSector;
                int cbInSector = dwRawBytesInThisSector;
                int nResult = 0;

                // Is the file compressed by PKWARE Data Compression Library ?
                if(pFileEntry->dwFlags & MPQ_FILE_IMPLODE)
                {
                    nResult = SCompExplode(pbOutSector, &cbOutSector, pbInSector, cbInSector);
                }

                // Did the decompression fail ?
                if(nResult == 0)
                {
                    nError = ERROR_FILE_CORRUPT;
                    break;
                }
            }
            else
            {
                if(pbOutSector != pbInSector)
                    memcpy(pbOutSector, pbInSector, dwBytesInThisSector);
            }

            // Move pointers
            dwBytesToRead -= dwBytesInThisSector;
            dwByteOffset += dwBytesInThisSector;
            dwBytesRead += dwBytesInThisSector;
            pbOutSector += dwBytesInThisSector;
            pbInSector += dwRawBytesInThisSector;
            dwSectorsDone++;
        }
    }
    else
    {
        nError = GetLastError();
    }

    // Free all used buffers
    if(pbRawSector != NULL)
        STORM_FREE(pbRawSector);

    // Give the caller thenumber of bytes read
    *pdwBytesRead = dwBytesRead;
    return nError;
}

static int ReadMpqFileSectorFile(TMPQFile * hf, void * pvBuffer, DWORD dwFilePos, DWORD dwBytesToRead, LPDWORD pdwBytesRead)
{
    TMPQArchive * ha = hf->ha;
    LPBYTE pbBuffer = (BYTE *)pvBuffer;
    DWORD dwTotalBytesRead = 0;                         // Total bytes read in all three parts
    DWORD dwSectorSizeMask = ha->dwSectorSize - 1;      // Mask for block size, usually 0x0FFF
    DWORD dwFileSectorPos;                              // File offset of the loaded sector
    DWORD dwBytesRead;                                  // Number of bytes read (temporary variable)
    int nError;

    // If the file position is at or beyond end of file, do nothing
    if(dwFilePos >= hf->dwDataSize)
    {
        *pdwBytesRead = 0;
        return ERROR_SUCCESS;
    }

    // If not enough bytes in the file remaining, cut them
    if(dwBytesToRead > (hf->dwDataSize - dwFilePos))
        dwBytesToRead = (hf->dwDataSize - dwFilePos);

    // Compute sector position in the file
    dwFileSectorPos = dwFilePos & ~dwSectorSizeMask;  // Position in the block

    // If the file sector buffer is not allocated yet, do it now
    if(hf->pbFileSector == NULL)
    {
        nError = AllocateSectorBuffer(hf);
        if(nError != ERROR_SUCCESS || hf->pbFileSector == NULL)
            return nError;
    }

    // Load the first (incomplete) file sector
    if(dwFilePos & dwSectorSizeMask)
    {
        DWORD dwBytesInSector = ha->dwSectorSize;
        DWORD dwBufferOffs = dwFilePos & dwSectorSizeMask;
        DWORD dwToCopy;

        // Is the file sector already loaded ?
        if(hf->dwSectorOffs != dwFileSectorPos)
        {
            // Load one MPQ sector into archive buffer
            nError = ReadMpqSectors(hf, hf->pbFileSector, dwFileSectorPos, ha->dwSectorSize, &dwBytesInSector);
            if(nError != ERROR_SUCCESS)
                return nError;

            // Remember that the data loaded to the sector have new file offset
            hf->dwSectorOffs = dwFileSectorPos;
        }
        else
        {
            if((dwFileSectorPos + dwBytesInSector) > hf->dwDataSize)
                dwBytesInSector = hf->dwDataSize - dwFileSectorPos;
        }

        // Copy the data from the offset in the loaded sector to the end of the sector
        dwToCopy = dwBytesInSector - dwBufferOffs;
        if(dwToCopy > dwBytesToRead)
            dwToCopy = dwBytesToRead;

        // Copy data from sector buffer into target buffer
        memcpy(pbBuffer, hf->pbFileSector + dwBufferOffs, dwToCopy);

        // Update pointers and byte counts
        dwTotalBytesRead += dwToCopy;
        dwFileSectorPos  += dwBytesInSector;
        pbBuffer         += dwToCopy;
        dwBytesToRead    -= dwToCopy;
    }

    // Load the whole ("middle") sectors only if there is at least one full sector to be read
    if(dwBytesToRead >= ha->dwSectorSize)
    {
        DWORD dwBlockBytes = dwBytesToRead & ~dwSectorSizeMask;

        // Load all sectors to the output buffer
        nError = ReadMpqSectors(hf, pbBuffer, dwFileSectorPos, dwBlockBytes, &dwBytesRead);
        if(nError != ERROR_SUCCESS)
            return nError;

        // Update pointers
        dwTotalBytesRead += dwBytesRead;
        dwFileSectorPos  += dwBytesRead;
        pbBuffer         += dwBytesRead;
        dwBytesToRead    -= dwBytesRead;
    }

    // Read the terminating sector
    if(dwBytesToRead > 0)
    {
        DWORD dwToCopy = ha->dwSectorSize;

        // Is the file sector already loaded ?
        if(hf->dwSectorOffs != dwFileSectorPos)
        {
            // Load one MPQ sector into archive buffer
            nError = ReadMpqSectors(hf, hf->pbFileSector, dwFileSectorPos, ha->dwSectorSize, &dwBytesRead);
            if(nError != ERROR_SUCCESS)
                return nError;

            // Remember that the data loaded to the sector have new file offset
            hf->dwSectorOffs = dwFileSectorPos;
        }

        // Check number of bytes read
        if(dwToCopy > dwBytesToRead)
            dwToCopy = dwBytesToRead;

        // Copy the data from the cached last sector to the caller's buffer
        memcpy(pbBuffer, hf->pbFileSector, dwToCopy);

        // Update pointers
        dwTotalBytesRead += dwToCopy;
    }

    // Store total number of bytes read to the caller
    *pdwBytesRead = dwTotalBytesRead;
    return ERROR_SUCCESS;
}

//-----------------------------------------------------------------------------
// SFileReadFile

bool WINAPI SFileReadFile(HANDLE hFile, void * pvBuffer, DWORD dwToRead, LPDWORD pdwRead, LPOVERLAPPED lpOverlapped)
{
    TMPQFile * hf = (TMPQFile *)hFile;
    DWORD dwBytesRead = 0;                      // Number of bytes read
    int nError = ERROR_SUCCESS;

    // Always zero the result
    if(pdwRead != NULL)
        *pdwRead = 0;

    // Check valid parameters
    if(!IsValidFileHandle(hFile))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return false;
    }

    if(pvBuffer == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    // Clear the last used compression
    hf->dwCompression0 = 0;

    // If the file is local file, read the data directly from the stream
    if(hf->pStream != NULL)
    {
        nError = ERROR_INVALID_PARAMETER;
    }

    // If the archive is a MPK archive, we need special way to read the file
    else if(hf->ha->dwSubType == MPQ_SUBTYPE_MPK)
    {
        nError = ERROR_INVALID_PARAMETER;
    }

    // If the file is single unit file, redirect it to read file
    else if(hf->pFileEntry->dwFlags & MPQ_FILE_SINGLE_UNIT)
    {
        nError = ERROR_INVALID_PARAMETER;
    }

    // Otherwise read it as sector based MPQ file
    else
    {
        nError = ReadMpqFileSectorFile(hf, pvBuffer, hf->dwFilePos, dwToRead, &dwBytesRead);
    }

    // Increment the file position
    hf->dwFilePos += dwBytesRead;

    // Give the caller the number of bytes read
    if(pdwRead != NULL)
        *pdwRead = dwBytesRead;

    // If the read operation succeeded, but not full number of bytes was read,
    // set the last error to ERROR_HANDLE_EOF
    if(nError == ERROR_SUCCESS && (dwBytesRead < dwToRead))
        nError = ERROR_HANDLE_EOF;

    // If something failed, set the last error value
    if(nError != ERROR_SUCCESS)
        SetLastError(nError);
    return (nError == ERROR_SUCCESS);
}

//-----------------------------------------------------------------------------
// SFileGetFileSize

DWORD WINAPI SFileGetFileSize(HANDLE hFile, LPDWORD pdwFileSizeHigh)
{
    ULONGLONG FileSize;
    TMPQFile * hf = (TMPQFile *)hFile;

    // Validate the file handle before we go on
    if(IsValidFileHandle(hFile))
    {
        // Make sure that the variable is initialized
        FileSize = 0;

        // If the file is patched file, we have to get the size of the last version
        if(hf->hfPatch != NULL)
        {
            // Walk through the entire patch chain, take the last version
            while(hf != NULL)
            {
                // Get the size of the currently pointed version
                FileSize = hf->pFileEntry->dwFileSize;

                // Move to the next patch file in the hierarchy
                hf = hf->hfPatch;
            }
        }
        else
        {
            // Is it a local file ?
            if(hf->pStream != NULL)
            {
                FileStream_GetSize(hf->pStream, &FileSize);
            }
            else
            {
                FileSize = hf->dwDataSize;
            }
        }

        // If opened from archive, return file size
        if(pdwFileSizeHigh != NULL)
            *pdwFileSizeHigh = (DWORD)(FileSize >> 32);
        return (DWORD)FileSize;
    }

    SetLastError(ERROR_INVALID_HANDLE);
    return SFILE_INVALID_SIZE;
}

DWORD WINAPI SFileSetFilePointer(HANDLE hFile, LONG lFilePos, LONG * plFilePosHigh, DWORD dwMoveMethod)
{
    TMPQFile * hf = (TMPQFile *)hFile;
    ULONGLONG OldPosition;
    ULONGLONG NewPosition;
    ULONGLONG FileSize;
    ULONGLONG DeltaPos;

    // If the hFile is not a valid file handle, return an error.
    if(!IsValidFileHandle(hFile))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return SFILE_INVALID_POS;
    }

    // Retrieve the file size for handling the limits
    if(hf->pStream != NULL)
    {
        FileStream_GetSize(hf->pStream, &FileSize);
    }
    else
    {
        FileSize = SFileGetFileSize(hFile, NULL);
    }

    // Handle the NULL and non-NULL values of plFilePosHigh
    // Non-NULL: The DeltaPos is combined from lFilePos and *lpFilePosHigh
    // NULL: The DeltaPos is sign-extended value of lFilePos
    DeltaPos = (plFilePosHigh != NULL) ? MAKE_OFFSET64(plFilePosHigh[0], lFilePos) : (ULONGLONG)(LONGLONG)lFilePos;

    // Get the relative point where to move from
    switch(dwMoveMethod)
    {
        case FILE_BEGIN:

            // Move relative to the file begin.
            OldPosition = 0;
            break;

        case FILE_CURRENT:

            // Retrieve the current file position
            if(hf->pStream != NULL)
            {
                FileStream_GetPos(hf->pStream, &OldPosition);
            }
            else
            {
                OldPosition = hf->dwFilePos;
            }
            break;

        case FILE_END:

            // Move relative to the end of the file
            OldPosition = FileSize;
            break;

        default:
            SetLastError(ERROR_INVALID_PARAMETER);
            return SFILE_INVALID_POS;
    }

    // Calculate the new position
    NewPosition = OldPosition + DeltaPos;

    // If moving backward, don't allow the new position go negative
    if((LONGLONG)DeltaPos < 0)
    {
        if(NewPosition > FileSize) // Position is negative
        {
            SetLastError(ERROR_NEGATIVE_SEEK);
            return SFILE_INVALID_POS;
        }
    }

    // If moving forward, don't allow the new position go past the end of the file
    else
    {
        if(NewPosition > FileSize)
            NewPosition = FileSize;
    }

    // Now apply the file pointer to the file
    if(hf->pStream != NULL)
    {
        if(!FileStream_Read(hf->pStream, &NewPosition, NULL, 0))
            return SFILE_INVALID_POS;
    }
    else
    {
        hf->dwFilePos = (DWORD)NewPosition;
    }

    // Return the new file position
    if(plFilePosHigh != NULL)
        *plFilePosHigh = (LONG)(NewPosition >> 32);
    return (DWORD)NewPosition;
}

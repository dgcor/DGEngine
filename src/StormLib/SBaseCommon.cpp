/*****************************************************************************/
/* SBaseCommon.cpp                        Copyright (c) Ladislav Zezula 2003 */
/*---------------------------------------------------------------------------*/
/* Common functions for StormLib, used by all SFile*** modules               */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 24.03.03  1.00  Lad  The first version of SFileCommon.cpp                 */
/* 19.11.03  1.01  Dan  Big endian handling                                  */
/* 12.06.04  1.01  Lad  Renamed to SCommon.cpp                               */
/* 06.09.10  1.01  Lad  Renamed to SBaseCommon.cpp                           */
/*****************************************************************************/

#define __STORMLIB_SELF__
#include "StormLib.h"
#include "StormCommon.h"

char StormLibCopyright[] = "StormLib v " STORMLIB_VERSION_STRING " Copyright Ladislav Zezula 1998-2014";

//-----------------------------------------------------------------------------
// Local variables

LCID    lcFileLocale = LANG_NEUTRAL;            // File locale
USHORT  wPlatform = 0;                          // File platform

//-----------------------------------------------------------------------------
// Conversion to uppercase/lowercase

// Converts ASCII characters to uppercase
// Converts slash (0x2F) to backslash (0x5C)
unsigned char AsciiToUpperTable[256] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x5C,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

// Converts ASCII characters to uppercase
// Does NOT convert slash (0x2F) to backslash (0x5C)
unsigned char AsciiToUpperTable_Slash[256] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

//-----------------------------------------------------------------------------
// Safe string functions (for ANSI builds)

char * StringCopy(char * szTarget, size_t cchTarget, const char * szSource)
{
    size_t cchSource = 0;

    if(cchTarget > 0)
    {
        cchSource = strlen(szSource);

        if(cchSource >= cchTarget)
            cchSource = cchTarget - 1;

        memcpy(szTarget, szSource, cchSource);
        szTarget[cchSource] = 0;
    }

    return szTarget + cchSource;
}

void StringCat(char * szTarget, size_t cchTargetMax, const char * szSource)
{
    // Get the current length of the target
    size_t cchTarget = strlen(szTarget);

    // Copy the string to the target
    if(cchTarget < cchTargetMax)
    {
        StringCopy(szTarget + cchTarget, (cchTargetMax - cchTarget), szSource);
    }
}

//-----------------------------------------------------------------------------
// Utility functions (UNICODE) only exist in the ANSI version of the library
// In ANSI builds, TCHAR = char, so we don't need these functions implemented

#ifdef _UNICODE
void StringCopy(TCHAR * szTarget, size_t cchTarget, const char * szSource)
{
    if(cchTarget > 0)
    {
        size_t cchSource = strlen(szSource);

        if(cchSource >= cchTarget)
            cchSource = cchTarget - 1;

        mbstowcs(szTarget, szSource, cchSource);
        szTarget[cchSource] = 0;
    }
}

void StringCopy(char * szTarget, size_t cchTarget, const TCHAR * szSource)
{
    if(cchTarget > 0)
    {
        size_t cchSource = _tcslen(szSource);

        if(cchSource >= cchTarget)
            cchSource = cchTarget - 1;

        wcstombs(szTarget, szSource, cchSource);
        szTarget[cchSource] = 0;
    }
}

void StringCopy(TCHAR * szTarget, size_t cchTarget, const TCHAR * szSource)
{
    if(cchTarget > 0)
    {
        size_t cchSource = _tcslen(szSource);

        if(cchSource >= cchTarget)
            cchSource = cchTarget - 1;

        memcpy(szTarget, szSource, cchSource * sizeof(TCHAR));
        szTarget[cchSource] = 0;
    }
}

void StringCat(TCHAR * szTarget, size_t cchTargetMax, const TCHAR * szSource)
{
    // Get the current length of the target
    size_t cchTarget = _tcslen(szTarget);

    // Copy the string to the target
    if(cchTarget < cchTargetMax)
    {
        StringCopy(szTarget + cchTarget, (cchTargetMax - cchTarget), szSource);
    }
}
#endif

//-----------------------------------------------------------------------------
// Storm hashing functions

#define STORM_BUFFER_SIZE       0x500
#define HASH_INDEX_MASK(ha) (ha->pHeader->dwHashTableSize ? (ha->pHeader->dwHashTableSize - 1) : 0)

static DWORD StormBuffer[STORM_BUFFER_SIZE];    // Buffer for the decryption engine
static bool  bMpqCryptographyInitialized = false;

void InitializeMpqCryptography()
{
    DWORD dwSeed = 0x00100001;
    DWORD index1 = 0;
    DWORD index2 = 0;
    int   i;

    // Initialize the decryption buffer.
    // Do nothing if already done.
    if(bMpqCryptographyInitialized == false)
    {
        for(index1 = 0; index1 < 0x100; index1++)
        {
            for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
            {
                DWORD temp1, temp2;

                dwSeed = (dwSeed * 125 + 3) % 0x2AAAAB;
                temp1  = (dwSeed & 0xFFFF) << 0x10;

                dwSeed = (dwSeed * 125 + 3) % 0x2AAAAB;
                temp2  = (dwSeed & 0xFFFF);

                StormBuffer[index2] = (temp1 | temp2);
            }
        }

        // Don't do that again
        bMpqCryptographyInitialized = true;
    }
}

//
// Note: Implementation of this function in WorldEdit.exe and storm.dll
// incorrectly treats the character as signed, which leads to the
// a buffer underflow if the character in the file name >= 0x80:
// The following steps happen when *pbKey == 0xBF and dwHashType == 0x0000
// (calculating hash index)
//
// 1) Result of AsciiToUpperTable_Slash[*pbKey++] is sign-extended to 0xffffffbf
// 2) The "ch" is added to dwHashType (0xffffffbf + 0x0000 => 0xffffffbf)
// 3) The result is used as index to the StormBuffer table,
// thus dereferences a random value BEFORE the begin of StormBuffer.
//
// As result, MPQs containing files with non-ANSI characters will not work between
// various game versions and localizations. Even WorldEdit, after importing a file
// with Korean characters in the name, cannot open the file back.
//
DWORD HashString(const char * szFileName, DWORD dwHashType)
{
    LPBYTE pbKey   = (BYTE *)szFileName;
    DWORD  dwSeed1 = 0x7FED7FED;
    DWORD  dwSeed2 = 0xEEEEEEEE;
    DWORD  ch;

    while(*pbKey != 0)
    {
        // Convert the input character to uppercase
        // Convert slash (0x2F) to backslash (0x5C)
        ch = AsciiToUpperTable[*pbKey++];

        dwSeed1 = StormBuffer[dwHashType + ch] ^ (dwSeed1 + dwSeed2);
        dwSeed2 = ch + dwSeed1 + dwSeed2 + (dwSeed2 << 5) + 3;
    }

    return dwSeed1;
}

DWORD HashStringSlash(const char * szFileName, DWORD dwHashType)
{
    LPBYTE pbKey   = (BYTE *)szFileName;
    DWORD  dwSeed1 = 0x7FED7FED;
    DWORD  dwSeed2 = 0xEEEEEEEE;
    DWORD  ch;

    while(*pbKey != 0)
    {
        // Convert the input character to uppercase
        // DON'T convert slash (0x2F) to backslash (0x5C)
        ch = AsciiToUpperTable_Slash[*pbKey++];

        dwSeed1 = StormBuffer[dwHashType + ch] ^ (dwSeed1 + dwSeed2);
        dwSeed2 = ch + dwSeed1 + dwSeed2 + (dwSeed2 << 5) + 3;
    }

    return dwSeed1;
}

//-----------------------------------------------------------------------------
// Calculates the hash table size for a given amount of files

// Returns the nearest higher power of two.
// If the value is already a power of two, returns the same value
DWORD GetNearestPowerOfTwo(DWORD dwFileCount)
{
    dwFileCount --;

    dwFileCount |= dwFileCount >> 1;
    dwFileCount |= dwFileCount >> 2;
    dwFileCount |= dwFileCount >> 4;
    dwFileCount |= dwFileCount >> 8;
    dwFileCount |= dwFileCount >> 16;

    return dwFileCount + 1;
}

//-----------------------------------------------------------------------------
// Decrypting MPQ data block

void DecryptMpqBlock(void * pvDataBlock, DWORD dwLength, DWORD dwKey1)
{
    LPDWORD DataBlock = (LPDWORD)pvDataBlock;
    DWORD dwValue32;
    DWORD dwKey2 = 0xEEEEEEEE;

    // Round to DWORDs
    dwLength >>= 2;

    // Decrypt the data block at array of DWORDs
    for(DWORD i = 0; i < dwLength; i++)
    {
        // Modify the second key
        dwKey2 += StormBuffer[MPQ_HASH_KEY2_MIX + (dwKey1 & 0xFF)];

        DataBlock[i] = DataBlock[i] ^ (dwKey1 + dwKey2);
        dwValue32 = DataBlock[i];

        dwKey1 = ((~dwKey1 << 0x15) + 0x11111111) | (dwKey1 >> 0x0B);
        dwKey2 = dwValue32 + dwKey2 + (dwKey2 << 5) + 3;
    }
}

// Function tries to detect file encryption key based on expected file content
// It is the same function like before, except that we know the value of the second DWORD
DWORD DetectFileKeyByKnownContent(void * pvEncryptedData, DWORD dwDecrypted0, DWORD dwDecrypted1)
{
    LPDWORD EncryptedData = (LPDWORD)pvEncryptedData;
    DWORD dwKey1PlusKey2;
    DWORD DataBlock[2];

    // Get the value of the combined encryption key
    dwKey1PlusKey2 = (EncryptedData[0] ^ dwDecrypted0) - 0xEEEEEEEE;

    // Try all 256 combinations of dwKey1
    for(DWORD i = 0; i < 0x100; i++)
    {
        DWORD dwSaveKey1;
        DWORD dwKey1 = dwKey1PlusKey2 - StormBuffer[MPQ_HASH_KEY2_MIX + i];
        DWORD dwKey2 = 0xEEEEEEEE;

        // Modify the second key and decrypt the first DWORD
        dwKey2 += StormBuffer[MPQ_HASH_KEY2_MIX + (dwKey1 & 0xFF)];
        DataBlock[0] = EncryptedData[0] ^ (dwKey1 + dwKey2);

        // Did we obtain the same value like dwDecrypted0?
        if(DataBlock[0] == dwDecrypted0)
        {
            // Save this key value
            dwSaveKey1 = dwKey1;

            // Rotate both keys
            dwKey1 = ((~dwKey1 << 0x15) + 0x11111111) | (dwKey1 >> 0x0B);
            dwKey2 = DataBlock[0] + dwKey2 + (dwKey2 << 5) + 3;

            // Modify the second key again and decrypt the second DWORD
            dwKey2 += StormBuffer[MPQ_HASH_KEY2_MIX + (dwKey1 & 0xFF)];
            DataBlock[1] = EncryptedData[1] ^ (dwKey1 + dwKey2);

            // Now compare the results
            if(DataBlock[1] == dwDecrypted1)
                return dwSaveKey1;
        }
    }

    // Key not found
    return 0;
}

DWORD DecryptFileKey(
    const char * szFileName,
    ULONGLONG MpqPos,
    DWORD dwFileSize,
    DWORD dwFlags)
{
    DWORD dwFileKey;
    DWORD dwMpqPos = (DWORD)MpqPos;

    // File key is calculated from plain name
    szFileName = GetPlainFileName(szFileName);
    dwFileKey = HashString(szFileName, MPQ_HASH_FILE_KEY);

    // Fix the key, if needed
    if(dwFlags & MPQ_FILE_FIX_KEY)
        dwFileKey = (dwFileKey + dwMpqPos) ^ dwFileSize;

    // Return the key
    return dwFileKey;
}

//-----------------------------------------------------------------------------
// Handle validation functions

TMPQArchive * IsValidMpqHandle(HANDLE hMpq)
{
    TMPQArchive * ha = (TMPQArchive *)hMpq;

    return (ha != NULL && ha->pHeader != NULL && ha->pHeader->dwID == ID_MPQ) ? ha : NULL;
}

TMPQFile * IsValidFileHandle(HANDLE hFile)
{
    TMPQFile * hf = (TMPQFile *)hFile;

    // Must not be NULL
    if(hf != NULL && hf->dwMagic == ID_MPQ_FILE)
    {
        // Local file handle?
        if(hf->pStream != NULL)
            return hf;

        // Also verify the MPQ handle within the file handle
        if(IsValidMpqHandle(hf->ha))
            return hf;
    }

    return NULL;
}

//-----------------------------------------------------------------------------
// Hash table and block table manipulation

// Retrieves the first hash entry for the given file.
// Every locale version of a file has its own hash entry
TMPQHash * GetFirstHashEntry(TMPQArchive * ha, const char * szFileName)
{
    DWORD dwHashIndexMask = HASH_INDEX_MASK(ha);
    DWORD dwStartIndex = ha->pfnHashString(szFileName, MPQ_HASH_TABLE_INDEX);
    DWORD dwName1 = ha->pfnHashString(szFileName, MPQ_HASH_NAME_A);
    DWORD dwName2 = ha->pfnHashString(szFileName, MPQ_HASH_NAME_B);
    DWORD dwIndex;

    // Set the initial index
    dwStartIndex = dwIndex = (dwStartIndex & dwHashIndexMask);

    // Search the hash table
    for(;;)
    {
        TMPQHash * pHash = ha->pHashTable + dwIndex;

        // If the entry matches, we found it.
        if(pHash->dwName1 == dwName1 && pHash->dwName2 == dwName2 && MPQ_BLOCK_INDEX(pHash) < ha->dwFileTableSize)
            return pHash;

        // If that hash entry is a free entry, it means we haven't found the file
        if(pHash->dwBlockIndex == HASH_ENTRY_FREE)
            return NULL;

        // Move to the next hash entry. Stop searching
        // if we got reached the original hash entry
        dwIndex = (dwIndex + 1) & dwHashIndexMask;
        if(dwIndex == dwStartIndex)
            return NULL;
    }
}

TMPQHash * GetNextHashEntry(TMPQArchive * ha, TMPQHash * pFirstHash, TMPQHash * pHash)
{
    DWORD dwHashIndexMask = HASH_INDEX_MASK(ha);
    DWORD dwStartIndex = (DWORD)(pFirstHash - ha->pHashTable);
    DWORD dwName1 = pHash->dwName1;
    DWORD dwName2 = pHash->dwName2;
    DWORD dwIndex = (DWORD)(pHash - ha->pHashTable);

    // Now go for any next entry that follows the pHash,
    // until either free hash entry was found, or the start entry was reached
    for(;;)
    {
        // Move to the next hash entry. Stop searching
        // if we got reached the original hash entry
        dwIndex = (dwIndex + 1) & dwHashIndexMask;
        if(dwIndex == dwStartIndex)
            return NULL;
        pHash = ha->pHashTable + dwIndex;

        // If the entry matches, we found it.
        if(pHash->dwName1 == dwName1 && pHash->dwName2 == dwName2 && MPQ_BLOCK_INDEX(pHash) < ha->dwFileTableSize)
            return pHash;

        // If that hash entry is a free entry, it means we haven't found the file
        if(pHash->dwBlockIndex == HASH_ENTRY_FREE)
            return NULL;
    }
}

//-----------------------------------------------------------------------------
// Common functions - MPQ File

TMPQFile * CreateFileHandle(TMPQArchive * ha, TFileEntry * pFileEntry)
{
    TMPQFile * hf;

    // Allocate space for TMPQFile
    hf = STORM_ALLOC(TMPQFile, 1);
    if(hf != NULL)
    {
        // Fill the file structure
        memset(hf, 0, sizeof(TMPQFile));
        hf->dwMagic = ID_MPQ_FILE;
        hf->pStream = NULL;
        hf->ha = ha;

        // If the called entered a file entry, we also copy informations from the file entry
        if(ha != NULL && pFileEntry != NULL)
        {
            // Set the raw position and MPQ position
            hf->RawFilePos = FileOffsetFromMpqOffset(ha, pFileEntry->ByteOffset);
            hf->MpqFilePos = pFileEntry->ByteOffset;

            // Set the data size
            hf->dwDataSize = pFileEntry->dwFileSize;
            hf->pFileEntry = pFileEntry;
        }
    }

    return hf;
}

// Loads a table from MPQ.
// Can be used for hash table, block table, sector offset table or sector checksum table
void * LoadMpqTable(
    TMPQArchive * ha,
    ULONGLONG ByteOffset,
    DWORD dwCompressedSize,
    DWORD dwTableSize,
    DWORD dwKey,
    bool * pbTableIsCut)
{
    ULONGLONG FileSize = 0;
    LPBYTE pbCompressed = NULL;
    LPBYTE pbMpqTable;
    LPBYTE pbToRead;
    DWORD dwBytesToRead = dwCompressedSize;
    int nError = ERROR_SUCCESS;

    // Allocate the MPQ table
    pbMpqTable = pbToRead = STORM_ALLOC(BYTE, dwTableSize);
    if(pbMpqTable != NULL)
    {
        // Check if the MPQ table is encrypted
        if(dwCompressedSize < dwTableSize)
        {
            // Allocate temporary buffer for holding compressed data
            pbCompressed = pbToRead = STORM_ALLOC(BYTE, dwCompressedSize);
            if(pbCompressed == NULL)
            {
                STORM_FREE(pbMpqTable);
                return NULL;
            }
        }

        // Get the file offset from which we will read the table
        // Note: According to Storm.dll from Warcraft III (version 2002),
        // if the hash table position is 0xFFFFFFFF, no SetFilePointer call is done
        // and the table is loaded from the current file offset
        if(ByteOffset == SFILE_INVALID_POS)
            FileStream_GetPos(ha->pStream, &ByteOffset);

        // On archives v 1.0, hash table and block table can go beyond EOF.
        // Storm.dll reads as much as possible, then fills the missing part with zeros.
        // Abused by Spazzler map protector which sets hash table size to 0x00100000
        // Abused by NP_Protect in MPQs v4 as well
        if(ha->pHeader->wFormatVersion == MPQ_FORMAT_VERSION_1)
        {
            // Cut the table size
            FileStream_GetSize(ha->pStream, &FileSize);
            if((ByteOffset + dwBytesToRead) > FileSize)
            {
                // Fill the extra data with zeros
                dwBytesToRead = (DWORD)(FileSize - ByteOffset);
                memset(pbMpqTable + dwBytesToRead, 0, (dwTableSize - dwBytesToRead));

                // Give the caller information that the table was cut
                if(pbTableIsCut != NULL)
                    pbTableIsCut[0] = true;
            }
        }

        // If everything succeeded, read the raw table from the MPQ
        if(FileStream_Read(ha->pStream, &ByteOffset, pbToRead, dwBytesToRead))
        {
            // First of all, decrypt the table
            if(dwKey != 0)
            {
                BSWAP_ARRAY32_UNSIGNED(pbToRead, dwCompressedSize);
                DecryptMpqBlock(pbToRead, dwCompressedSize, dwKey);
                BSWAP_ARRAY32_UNSIGNED(pbToRead, dwCompressedSize);
            }

            // Make sure that the table is properly byte-swapped
            BSWAP_ARRAY32_UNSIGNED(pbMpqTable, dwTableSize);
        }
        else
        {
            nError = GetLastError();
        }

        // If read failed, free the table and return
        if(nError != ERROR_SUCCESS)
        {
            STORM_FREE(pbMpqTable);
            pbMpqTable = NULL;
        }

        // Free the compression buffer, if any
        if(pbCompressed != NULL)
            STORM_FREE(pbCompressed);
    }

    // Return the MPQ table
    return pbMpqTable;
}

unsigned char * AllocateMd5Buffer(
    DWORD dwRawDataSize,
    DWORD dwChunkSize,
    LPDWORD pcbMd5Size)
{
    unsigned char * md5_array;
    DWORD cbMd5Size;

    // Sanity check
    assert(dwRawDataSize != 0);
    assert(dwChunkSize != 0);

    // Calculate how many MD5's we will calculate
    cbMd5Size = (((dwRawDataSize - 1) / dwChunkSize) + 1) * MD5_DIGEST_SIZE;

    // Allocate space for array or MD5s
    md5_array = STORM_ALLOC(BYTE, cbMd5Size);

    // Give the size of the MD5 array
    if(pcbMd5Size != NULL)
        *pcbMd5Size = cbMd5Size;
    return md5_array;
}

// Allocates sector buffer and sector offset table
int AllocateSectorBuffer(TMPQFile * hf)
{
    TMPQArchive * ha = hf->ha;

    // Caller of AllocateSectorBuffer must ensure these
    assert(hf->pbFileSector == NULL);
    assert(hf->pFileEntry != NULL);
    assert(hf->ha != NULL);

    // Don't allocate anything if the file has zero size
    if(hf->pFileEntry->dwFileSize == 0 || hf->dwDataSize == 0)
        return ERROR_SUCCESS;

    // Determine the file sector size and allocate buffer for it
    hf->dwSectorSize = (hf->pFileEntry->dwFlags & MPQ_FILE_SINGLE_UNIT) ? hf->dwDataSize : ha->dwSectorSize;
    hf->pbFileSector = STORM_ALLOC(BYTE, hf->dwSectorSize);
    hf->dwSectorOffs = SFILE_INVALID_POS;

    // Return result
    return (hf->pbFileSector != NULL) ? (int)ERROR_SUCCESS : (int)ERROR_NOT_ENOUGH_MEMORY;
}

// Allocates sector offset table
int AllocateSectorOffsets(TMPQFile * hf, bool bLoadFromFile)
{
    TMPQArchive * ha = hf->ha;
    TFileEntry * pFileEntry = hf->pFileEntry;
    DWORD dwSectorOffsLen;
    bool bSectorOffsetTableCorrupt = false;

    // Caller of AllocateSectorOffsets must ensure these
    assert(hf->SectorOffsets == NULL);
    assert(hf->pFileEntry != NULL);
    assert(hf->dwDataSize != 0);
    assert(hf->ha != NULL);

    // If the file is stored as single unit, just set number of sectors to 1
    if(pFileEntry->dwFlags & MPQ_FILE_SINGLE_UNIT)
    {
        hf->dwSectorCount = 1;
        return ERROR_SUCCESS;
    }

    // Calculate the number of data sectors
    // Note that this doesn't work if the file size is zero
    hf->dwSectorCount = ((hf->dwDataSize - 1) / hf->dwSectorSize) + 1;

    // Calculate the number of file sectors
    dwSectorOffsLen = (hf->dwSectorCount + 1) * sizeof(DWORD);

    // If MPQ_FILE_SECTOR_CRC flag is set, there will either be extra DWORD
    // or an array of MD5's. Either way, we read at least 4 bytes more
    // in order to save additional read from the file.
    if(pFileEntry->dwFlags & MPQ_FILE_SECTOR_CRC)
        dwSectorOffsLen += sizeof(DWORD);

    // Only allocate and load the table if the file is compressed
    if(pFileEntry->dwFlags & MPQ_FILE_COMPRESS_MASK)
    {
        __LoadSectorOffsets:

        // Allocate the sector offset table
        hf->SectorOffsets = STORM_ALLOC(DWORD, (dwSectorOffsLen / sizeof(DWORD)));
        if(hf->SectorOffsets == NULL)
            return ERROR_NOT_ENOUGH_MEMORY;

        // Only read from the file if we are supposed to do so
        if(bLoadFromFile)
        {
            ULONGLONG RawFilePos = hf->RawFilePos;

            // Append the length of the patch info, if any
            if(hf->pPatchInfo != NULL)
            {
                if((RawFilePos + hf->pPatchInfo->dwLength) < RawFilePos)
                    return ERROR_FILE_CORRUPT;
                RawFilePos += hf->pPatchInfo->dwLength;
            }

            // Load the sector offsets from the file
            if(!FileStream_Read(ha->pStream, &RawFilePos, hf->SectorOffsets, dwSectorOffsLen))
            {
                // Free the sector offsets
                STORM_FREE(hf->SectorOffsets);
                hf->SectorOffsets = NULL;
                return GetLastError();
            }

            // Swap the sector positions
            BSWAP_ARRAY32_UNSIGNED(hf->SectorOffsets, dwSectorOffsLen);

            // Decrypt loaded sector positions if necessary
            if(pFileEntry->dwFlags & MPQ_FILE_ENCRYPTED)
            {
                // If we don't know the file key, try to find it.
                if(hf->dwFileKey == 0)
                {
                    hf->SectorOffsets = NULL;
                    return ERROR_UNKNOWN_FILE_KEY;
                }

                // Decrypt sector positions
                DecryptMpqBlock(hf->SectorOffsets, dwSectorOffsLen, hf->dwFileKey - 1);
            }

            //
            // Validate the sector offset table
            //
            // Note: Some MPQ protectors put the actual file data before the sector offset table.
            // In this case, the sector offsets are negative (> 0x80000000).
            //

            for(DWORD i = 0; i < hf->dwSectorCount; i++)
            {
                DWORD dwSectorOffset1 = hf->SectorOffsets[i+1];
                DWORD dwSectorOffset0 = hf->SectorOffsets[i];

                // Every following sector offset must be bigger than the previous one
                if(dwSectorOffset1 <= dwSectorOffset0)
                {
                    bSectorOffsetTableCorrupt = true;
                    break;
                }

                // The sector size must not be bigger than compressed file size
                // Edit: Yes, but apparently, in original Storm.dll, the compressed
                // size is not checked anywhere. However, we need to do this check
                // in order to sector offset table malformed by MPQ protectors
                if((dwSectorOffset1 - dwSectorOffset0) > ha->dwSectorSize)
                {
                    bSectorOffsetTableCorrupt = true;
                    break;
                }
            }

            // If data corruption detected, free the sector offset table
            if(bSectorOffsetTableCorrupt)
            {
                STORM_FREE(hf->SectorOffsets);
                hf->SectorOffsets = NULL;
                return ERROR_FILE_CORRUPT;
            }

            //
            // There may be various extra DWORDs loaded after the sector offset table.
            // They are mostly empty on WoW release MPQs, but on MPQs from PTR,
            // they contain random non-zero data. Their meaning is unknown.
            //
            // These extra values are, however, include in the dwCmpSize in the file
            // table. We cannot ignore them, because compacting archive would fail
            //

            if(hf->SectorOffsets[0] > dwSectorOffsLen)
            {
                // MPQ protectors put some ridiculous values there. We must limit the extra bytes
                if(hf->SectorOffsets[0] > (dwSectorOffsLen + 0x400))
                    return ERROR_FILE_CORRUPT;

                // Free the old sector offset table
                dwSectorOffsLen = hf->SectorOffsets[0];
                STORM_FREE(hf->SectorOffsets);
                goto __LoadSectorOffsets;
            }
        }
        else
        {
            memset(hf->SectorOffsets, 0, dwSectorOffsLen);
            hf->SectorOffsets[0] = dwSectorOffsLen;
        }
    }

    return ERROR_SUCCESS;
}

// Frees the structure for MPQ file
void FreeFileHandle(TMPQFile *& hf)
{
    if(hf != NULL)
    {
        // If we have patch file attached to this one, free it first
        if(hf->hfPatch != NULL)
            FreeFileHandle(hf->hfPatch);

        // Then free all buffers allocated in the file structure
        if(hf->pbFileData != NULL)
            STORM_FREE(hf->pbFileData);
        if(hf->pPatchInfo != NULL)
            STORM_FREE(hf->pPatchInfo);
        if(hf->SectorOffsets != NULL)
            STORM_FREE(hf->SectorOffsets);
        if(hf->SectorChksums != NULL)
            STORM_FREE(hf->SectorChksums);
        if(hf->pbFileSector != NULL)
            STORM_FREE(hf->pbFileSector);
        if(hf->pStream != NULL)
            FileStream_Close(hf->pStream);
        STORM_FREE(hf);
        hf = NULL;
    }
}

// Frees the MPQ archive
void FreeArchiveHandle(TMPQArchive *& ha)
{
    if(ha != NULL)
    {
        // First of all, free the patch archive, if any
        if(ha->haPatch != NULL)
            FreeArchiveHandle(ha->haPatch);

        // Free the patch prefix, if any
        if(ha->pPatchPrefix != NULL)
            STORM_FREE(ha->pPatchPrefix);

        // Close the file stream
        FileStream_Close(ha->pStream);
        ha->pStream = NULL;

        // Free the file names from the file table
        if(ha->pFileTable != NULL)
        {
            for(DWORD i = 0; i < ha->dwFileTableSize; i++)
            {
                if(ha->pFileTable[i].szFileName != NULL)
                    STORM_FREE(ha->pFileTable[i].szFileName);
                ha->pFileTable[i].szFileName = NULL;
            }

            // Then free all buffers allocated in the archive structure
            STORM_FREE(ha->pFileTable);
        }

        if(ha->pHashTable != NULL)
            STORM_FREE(ha->pHashTable);
        STORM_FREE(ha);
        ha = NULL;
    }
}

// Verifies if the file name is a pseudo-name
bool IsPseudoFileName(const char * szFileName, DWORD * pdwFileIndex)
{
    DWORD dwFileIndex = 0;

    if(szFileName != NULL)
    {
        // Must be "File########.ext"
        if(!_strnicmp(szFileName, "File", 4))
        {
            // Check 8 digits
            for(int i = 4; i < 4+8; i++)
            {
                if(szFileName[i] < '0' || szFileName[i] > '9')
                    return false;
                dwFileIndex = (dwFileIndex * 10) + (szFileName[i] - '0');
            }

            // An extension must follow
            if(szFileName[12] == '.')
            {
                if(pdwFileIndex != NULL)
                    *pdwFileIndex = dwFileIndex;
                return true;
            }
        }
    }

    // Not a pseudo-name
    return false;
}

//-----------------------------------------------------------------------------
// Swapping functions

#ifndef STORMLIB_LITTLE_ENDIAN

//
// Note that those functions are implemented for Mac operating system,
// as this is the only supported platform that uses big endian.
//

// Swaps a signed 16-bit integer
int16_t SwapInt16(uint16_t data)
{
	return (int16_t)CFSwapInt16(data);
}

// Swaps an unsigned 16-bit integer
uint16_t SwapUInt16(uint16_t data)
{
	return CFSwapInt16(data);
}

// Swaps signed 32-bit integer
int32_t SwapInt32(uint32_t data)
{
	return (int32_t)CFSwapInt32(data);
}

// Swaps an unsigned 32-bit integer
uint32_t SwapUInt32(uint32_t data)
{
	return CFSwapInt32(data);
}

// Swaps signed 64-bit integer
int64_t SwapInt64(int64_t data)
{
       return (int64_t)CFSwapInt64(data);
}

// Swaps an unsigned 64-bit integer
uint64_t SwapUInt64(uint64_t data)
{
       return CFSwapInt64(data);
}

// Swaps array of unsigned 16-bit integers
void ConvertUInt16Buffer(void * ptr, size_t length)
{
    uint16_t * buffer = (uint16_t *)ptr;
    uint32_t nElements = (uint32_t)(length / sizeof(uint16_t));

    while(nElements-- > 0)
	{
		*buffer = SwapUInt16(*buffer);
		buffer++;
	}
}

// Swaps array of unsigned 32-bit integers
void ConvertUInt32Buffer(void * ptr, size_t length)
{
    uint32_t * buffer = (uint32_t *)ptr;
    uint32_t nElements = (uint32_t)(length / sizeof(uint32_t));

	while(nElements-- > 0)
	{
		*buffer = SwapUInt32(*buffer);
		buffer++;
	}
}

// Swaps array of unsigned 64-bit integers
void ConvertUInt64Buffer(void * ptr, size_t length)
{
    uint64_t * buffer = (uint64_t *)ptr;
    uint32_t nElements = (uint32_t)(length / sizeof(uint64_t));

	while(nElements-- > 0)
	{
		*buffer = SwapUInt64(*buffer);
		buffer++;
	}
}

// Swaps the TMPQHeader structure
void ConvertTMPQHeader(void *header, uint16_t version)
{
	TMPQHeader * theHeader = (TMPQHeader *)header;

    // Swap header part version 1
    if(version == MPQ_FORMAT_VERSION_1)
    {
	    theHeader->dwID = SwapUInt32(theHeader->dwID);
	    theHeader->dwHeaderSize = SwapUInt32(theHeader->dwHeaderSize);
	    theHeader->dwArchiveSize = SwapUInt32(theHeader->dwArchiveSize);
	    theHeader->wFormatVersion = SwapUInt16(theHeader->wFormatVersion);
	    theHeader->wSectorSize = SwapUInt16(theHeader->wSectorSize);
	    theHeader->dwHashTablePos = SwapUInt32(theHeader->dwHashTablePos);
	    theHeader->dwBlockTablePos = SwapUInt32(theHeader->dwBlockTablePos);
	    theHeader->dwHashTableSize = SwapUInt32(theHeader->dwHashTableSize);
	    theHeader->dwBlockTableSize = SwapUInt32(theHeader->dwBlockTableSize);
    }

	if(version == MPQ_FORMAT_VERSION_2)
	{
		theHeader->HiBlockTablePos64 = SwapUInt64(theHeader->HiBlockTablePos64);
        theHeader->wHashTablePosHi = SwapUInt16(theHeader->wHashTablePosHi);
		theHeader->wBlockTablePosHi = SwapUInt16(theHeader->wBlockTablePosHi);
    }

    if(version == MPQ_FORMAT_VERSION_3)
    {
        theHeader->ArchiveSize64 = SwapUInt64(theHeader->ArchiveSize64);
        theHeader->BetTablePos64 = SwapUInt64(theHeader->BetTablePos64);
        theHeader->HetTablePos64 = SwapUInt64(theHeader->HetTablePos64);
    }

    if(version == MPQ_FORMAT_VERSION_4)
	{
        theHeader->HashTableSize64    = SwapUInt64(theHeader->HashTableSize64);
        theHeader->BlockTableSize64   = SwapUInt64(theHeader->BlockTableSize64);
        theHeader->HiBlockTableSize64 = SwapUInt64(theHeader->HiBlockTableSize64);
        theHeader->HetTableSize64     = SwapUInt64(theHeader->HetTableSize64);
        theHeader->BetTableSize64     = SwapUInt64(theHeader->BetTableSize64);
    }
}

#endif  // STORMLIB_LITTLE_ENDIAN

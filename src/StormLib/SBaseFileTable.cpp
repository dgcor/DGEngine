/*****************************************************************************/
/* SBaseFileTable.cpp                     Copyright (c) Ladislav Zezula 2010 */
/*---------------------------------------------------------------------------*/
/* Description: Common handler for classic and new hash&block tables         */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 06.09.10  1.00  Lad  The first version of SBaseFileTable.cpp              */
/*****************************************************************************/

#define __STORMLIB_SELF__
#include "StormLib.h"
#include "StormCommon.h"

//-----------------------------------------------------------------------------
// Local defines

#define INVALID_FLAG_VALUE 0xCCCCCCCC
#define MAX_FLAG_INDEX     512

//-----------------------------------------------------------------------------
// Support for MPQ header

ULONGLONG FileOffsetFromMpqOffset(TMPQArchive * ha, ULONGLONG MpqOffset)
{
    if(ha->pHeader->wFormatVersion == MPQ_FORMAT_VERSION_1)
    {
        // For MPQ archive v1, any file offset is only 32-bit
        return (ULONGLONG)((DWORD)ha->MpqPos + (DWORD)MpqOffset);
    }
    else
    {
        // For MPQ archive v2+, file offsets are full 64-bit
        return ha->MpqPos + MpqOffset;
    }
}

ULONGLONG CalculateRawSectorOffset(
    TMPQFile * hf,
    DWORD dwSectorOffset)
{
    ULONGLONG RawFilePos;

    // Must be used for files within a MPQ
    assert(hf->ha != NULL);
    assert(hf->ha->pHeader != NULL);

    //
    // Some MPQ protectors place the sector offset table after the actual file data.
    // Sector offsets in the sector offset table are negative. When added
    // to MPQ file offset from the block table entry, the result is a correct
    // position of the file data in the MPQ.
    //
    // For MPQs version 1.0, the offset is purely 32-bit
    //

    RawFilePos = hf->RawFilePos + dwSectorOffset;
    if(hf->ha->pHeader->wFormatVersion == MPQ_FORMAT_VERSION_1)
        RawFilePos = (DWORD)hf->ha->MpqPos + (DWORD)hf->pFileEntry->ByteOffset + dwSectorOffset;

    // We also have to add patch header size, if patch header is present
    if(hf->pPatchInfo != NULL)
        RawFilePos += hf->pPatchInfo->dwLength;

    // Return the result offset
    return RawFilePos;
}

// This function converts the MPQ header so it always looks like version 4
int ConvertMpqHeaderToFormat4(
    TMPQArchive * ha,
    ULONGLONG MpqOffset,
    ULONGLONG FileSize,
    DWORD dwFlags,
    MTYPE MapType)
{
    TMPQHeader * pHeader = (TMPQHeader *)ha->HeaderData;
    ULONGLONG BlockTablePos64 = 0;
    USHORT wFormatVersion = BSWAP_INT16_UNSIGNED(pHeader->wFormatVersion);
    int nError = ERROR_SUCCESS;

    // If version 1.0 is forced, then the format version is forced to be 1.0
    // Reason: Storm.dll in Warcraft III ignores format version value
    if(dwFlags & MPQ_OPEN_FORCE_MPQ_V1)
        wFormatVersion = MPQ_FORMAT_VERSION_1;

    // Format-specific fixes
    switch(wFormatVersion)
    {
        case MPQ_FORMAT_VERSION_1:

            // Check for malformed MPQ header version 1.0
            BSWAP_TMPQHEADER(pHeader, MPQ_FORMAT_VERSION_1);
            if(pHeader->wFormatVersion != MPQ_FORMAT_VERSION_1 || pHeader->dwHeaderSize != MPQ_HEADER_SIZE_V1)
            {
                pHeader->wFormatVersion = MPQ_FORMAT_VERSION_1;
                pHeader->dwHeaderSize = MPQ_HEADER_SIZE_V1;
                ha->dwFlags |= MPQ_FLAG_MALFORMED;
            }

            //
            // Note: The value of "dwArchiveSize" member in the MPQ header
            // is ignored by Storm.dll and can contain garbage value
            // ("w3xmaster" protector).
            //

            if(pHeader->dwBlockTableSize > 1)  // Prevent empty MPQs being marked as malformed
            {
                if(pHeader->dwHashTablePos <= pHeader->dwHeaderSize || (pHeader->dwHashTablePos & 0x80000000))
                    ha->dwFlags |= MPQ_FLAG_MALFORMED;
                if(pHeader->dwBlockTablePos <= pHeader->dwHeaderSize || (pHeader->dwBlockTablePos & 0x80000000))
                    ha->dwFlags |= MPQ_FLAG_MALFORMED;
            }

            // Only low byte of sector size is really used
            if(pHeader->wSectorSize & 0xFF00)
                ha->dwFlags |= MPQ_FLAG_MALFORMED;
            pHeader->wSectorSize = pHeader->wSectorSize & 0xFF;

            // Fill the rest of the header
            memset((LPBYTE)pHeader + MPQ_HEADER_SIZE_V1, 0, sizeof(TMPQHeader) - MPQ_HEADER_SIZE_V1);
            pHeader->BlockTableSize64 = pHeader->dwBlockTableSize * sizeof(TMPQBlock);
            pHeader->HashTableSize64 = pHeader->dwHashTableSize * sizeof(TMPQHash);
            pHeader->ArchiveSize64 = pHeader->dwArchiveSize;

            // Block table position must be calculated as 32-bit value
            // Note: BOBA protector puts block table before the MPQ header, so it is negative
            BlockTablePos64 = (ULONGLONG)((DWORD)MpqOffset + pHeader->dwBlockTablePos);

            // Determine the archive size on malformed MPQs
            if(ha->dwFlags & MPQ_FLAG_MALFORMED)
            {
                nError = ERROR_BAD_FORMAT;
                break;
            }

            // EWIX_v8_7.w3x: TMPQHeader::dwBlockTableSize = 0x00319601
            // Size of TFileTable goes to ~200MB, so we artificially cut it
            if(BlockTablePos64 + (pHeader->dwBlockTableSize * sizeof(TMPQBlock)) > FileSize)
            {
                pHeader->dwBlockTableSize = (DWORD)((FileSize - BlockTablePos64) / sizeof(TMPQBlock));
                pHeader->BlockTableSize64 = pHeader->dwBlockTableSize * sizeof(TMPQBlock);
            }
            break;

        default:

            nError = ERROR_BAD_FORMAT;
            break;
    }

    // Handle case when block table is placed before the MPQ header
    // Used by BOBA protector
    if(BlockTablePos64 < MpqOffset)
        ha->dwFlags |= MPQ_FLAG_MALFORMED;
    return nError;
}

//-----------------------------------------------------------------------------
// Support for hash table

// Hash entry verification when the file table does not exist yet
static bool IsValidHashEntry1(TMPQArchive * ha, TMPQHash * pHash, TMPQBlock * pBlockTable)
{
    ULONGLONG ByteOffset;
    TMPQBlock * pBlock;

    // The block index is considered valid if it's less than block table size
    if(MPQ_BLOCK_INDEX(pHash) < ha->pHeader->dwBlockTableSize)
    {
        // Calculate the block table position
        pBlock = pBlockTable + MPQ_BLOCK_INDEX(pHash);

        // Check whether this is an existing file
        // Also we do not allow to be file size greater than 2GB
        if((pBlock->dwFlags & MPQ_FILE_EXISTS) && (pBlock->dwFSize & 0x80000000) == 0)
        {
            // The begin of the file must be within the archive
            ByteOffset = FileOffsetFromMpqOffset(ha, pBlock->dwFilePos);
            return (ByteOffset < ha->FileSize);
        }
    }

    return false;
}

// Returns a hash table entry in the following order:
// 1) A hash table entry with the preferred locale and platform
// 2) A hash table entry with the neutral|matching locale and neutral|matching platform
// 3) NULL
// Storm_2016.dll: 15020940
static TMPQHash * GetHashEntryLocale(TMPQArchive * ha, const char * szFileName, LCID lcLocale, BYTE Platform)
{
    TMPQHash * pFirstHash = GetFirstHashEntry(ha, szFileName);
    TMPQHash * pBestEntry = NULL;
    TMPQHash * pHash = pFirstHash;

    // Parse the found hashes
    while(pHash != NULL)
    {
        // Storm_2016.dll: 150209CB
        // If the hash entry matches both locale and platform, return it immediately
        // Note: We only succeed this check if the locale is non-neutral, because
        // some Warcraft III maps have several items with neutral locale&platform, which leads
        // to wrong item being returned
        if((lcLocale || Platform) && pHash->lcLocale == lcLocale && pHash->Platform == Platform)
            return pHash;

        // Storm_2016.dll: 150209D9
        // If (locale matches or is neutral) OR (platform matches or is neutral)
        // remember this as the best entry
        if(pHash->lcLocale == 0 || pHash->lcLocale == lcLocale)
        {
            if(pHash->Platform == 0 || pHash->Platform == Platform)
                pBestEntry = pHash;
        }

        // Get the next hash entry for that file
        pHash = GetNextHashEntry(ha, pFirstHash, pHash);
    }

    // At the end, return neutral hash (if found), otherwise NULL
    return pBestEntry;
}

static int BuildFileTableFromBlockTable(
    TMPQArchive * ha,
    TMPQBlock * pBlockTable)
{
    TFileEntry * pFileEntry;
    TMPQHeader * pHeader = ha->pHeader;
    TMPQBlock * pBlock;
    TMPQHash * pHashTableEnd;
    TMPQHash * pHash;
    LPDWORD DefragmentTable = NULL;
    DWORD dwItemCount = 0;
    DWORD dwFlagMask;

    // Sanity checks
    assert(ha->pFileTable != NULL);
    assert(ha->dwFileTableSize >= ha->dwMaxFileCount);

    // MPQs for Warcraft III doesn't know some flags, namely MPQ_FILE_SINGLE_UNIT and MPQ_FILE_PATCH_FILE
    dwFlagMask = (ha->dwFlags & MPQ_FLAG_WAR3_MAP) ? MPQ_FILE_VALID_FLAGS_W3X : MPQ_FILE_VALID_FLAGS;

    // If the hash table or block table is cut,
    // we will defragment the block table
    if(ha->dwFlags & (MPQ_FLAG_HASH_TABLE_CUT | MPQ_FLAG_BLOCK_TABLE_CUT))
    {
        // Sanity checks
        assert(pHeader->wFormatVersion == MPQ_FORMAT_VERSION_1);
        assert(pHeader->HiBlockTablePos64 == 0);

        // Allocate the translation table
        DefragmentTable = STORM_ALLOC(DWORD, pHeader->dwBlockTableSize);
        if(DefragmentTable == NULL)
            return ERROR_NOT_ENOUGH_MEMORY;

        // Fill the translation table
        memset(DefragmentTable, 0xFF, pHeader->dwBlockTableSize * sizeof(DWORD));
    }

    // Parse the entire hash table
    pHashTableEnd = ha->pHashTable + pHeader->dwHashTableSize;
    for(pHash = ha->pHashTable; pHash < pHashTableEnd; pHash++)
    {
        //
        // We need to properly handle these cases:
        // - Multiple hash entries (same file name) point to the same block entry
        // - Multiple hash entries (different file name) point to the same block entry
        //
        // Ignore all hash table entries where:
        // - Block Index >= BlockTableSize
        // - Flags of the appropriate block table entry
        //

        if(IsValidHashEntry1(ha, pHash, pBlockTable))
        {
            DWORD dwOldIndex = MPQ_BLOCK_INDEX(pHash);
            DWORD dwNewIndex = MPQ_BLOCK_INDEX(pHash);

            // Determine the new block index
            if(DefragmentTable != NULL)
            {
                // Need to handle case when multiple hash
                // entries point to the same block entry
                if(DefragmentTable[dwOldIndex] == HASH_ENTRY_FREE)
                {
                    DefragmentTable[dwOldIndex] = dwItemCount;
                    dwNewIndex = dwItemCount++;
                }
                else
                {
                    dwNewIndex = DefragmentTable[dwOldIndex];
                }

                // Fix the pointer in the hash entry
                pHash->dwBlockIndex = dwNewIndex;

                // Dump the relocation entry
//              printf("Relocating hash entry %08X-%08X: %08X -> %08X\n", pHash->dwName1, pHash->dwName2, dwBlockIndex, dwNewIndex);
            }

            // Get the pointer to the file entry and the block entry
            pFileEntry = ha->pFileTable + dwNewIndex;
            pBlock = pBlockTable + dwOldIndex;

            // ByteOffset is only valid if file size is not zero
            pFileEntry->ByteOffset = pBlock->dwFilePos;
            if(pFileEntry->ByteOffset == 0 && pBlock->dwFSize == 0)
                pFileEntry->ByteOffset = ha->pHeader->dwHeaderSize;

            // Fill the rest of the file entry
            pFileEntry->dwFileSize  = pBlock->dwFSize;
            pFileEntry->dwCmpSize   = pBlock->dwCSize;
            pFileEntry->dwFlags     = pBlock->dwFlags & dwFlagMask;
        }
    }

    // Free the translation table
    if(DefragmentTable != NULL)
    {
        // If we defragmented the block table in the process,
        // free some memory by shrinking the file table
        if(ha->dwFileTableSize > ha->dwMaxFileCount)
        {
            ha->pFileTable = STORM_REALLOC(TFileEntry, ha->pFileTable, ha->dwMaxFileCount);
            ha->pHeader->BlockTableSize64 = ha->dwMaxFileCount * sizeof(TMPQBlock);
            ha->pHeader->dwBlockTableSize = ha->dwMaxFileCount;
            ha->dwFileTableSize = ha->dwMaxFileCount;
        }

//      DumpFileTable(ha->pFileTable, ha->dwFileTableSize);

        // Free the translation table
        STORM_FREE(DefragmentTable);
    }

    return ERROR_SUCCESS;
}

//-----------------------------------------------------------------------------
// Support for file table

TFileEntry * GetFileEntryLocale2(TMPQArchive * ha, const char * szFileName, LCID lcLocale, LPDWORD PtrHashIndex)
{
    TMPQHash * pHash;

    // First, we have to search the classic hash table
    // This is because on renaming, deleting, or changing locale,
    // we will need the pointer to hash table entry
    if(ha->pHashTable != NULL)
    {
        pHash = GetHashEntryLocale(ha, szFileName, lcLocale, 0);
        if(pHash != NULL && MPQ_BLOCK_INDEX(pHash) < ha->dwFileTableSize)
        {
            if(PtrHashIndex != NULL)
                PtrHashIndex[0] = (DWORD)(pHash - ha->pHashTable);
            return ha->pFileTable + MPQ_BLOCK_INDEX(pHash);
        }
    }

    // Not found
    return NULL;
}

TFileEntry * GetFileEntryLocale(TMPQArchive * ha, const char * szFileName, LCID lcLocale)
{
    return GetFileEntryLocale2(ha, szFileName, lcLocale, NULL);
}

void AllocateFileName(TMPQArchive * ha, TFileEntry * pFileEntry, const char * szFileName)
{
    // Sanity check
    assert(pFileEntry != NULL);

    // If the file name is pseudo file name, free it at this point
    if(IsPseudoFileName(pFileEntry->szFileName, NULL))
    {
        if(pFileEntry->szFileName != NULL)
            STORM_FREE(pFileEntry->szFileName);
        pFileEntry->szFileName = NULL;
    }

    // Only allocate new file name if it's not there yet
    if(pFileEntry->szFileName == NULL)
    {
        pFileEntry->szFileName = STORM_ALLOC(char, strlen(szFileName) + 1);
        if(pFileEntry->szFileName != NULL)
            strcpy(pFileEntry->szFileName, szFileName);
    }
}

//-----------------------------------------------------------------------------
// Support for file tables - hash table, block table, hi-block table

static TMPQHash * LoadHashTable(TMPQArchive * ha)
{
    TMPQHeader * pHeader = ha->pHeader;
    ULONGLONG ByteOffset;
    TMPQHash * pHashTable = NULL;
    DWORD dwTableSize;
    DWORD dwCmpSize;
    bool bHashTableIsCut = false;

    // Note: It is allowed to load hash table if it is at offset 0.
    // Example: MPQ_2016_v1_ProtectedMap_HashOffsIsZero.w3x
//  if(pHeader->dwHashTablePos == 0 && pHeader->wHashTablePosHi == 0)
//      return NULL;

    // If the hash table size is zero, do nothing
    if(pHeader->dwHashTableSize == 0)
        return NULL;

    // Load the hash table for MPQ variations
    switch(ha->dwSubType)
    {
        case MPQ_SUBTYPE_MPQ:

            // Calculate the position and size of the hash table
            ByteOffset = FileOffsetFromMpqOffset(ha, MAKE_OFFSET64(pHeader->wHashTablePosHi, pHeader->dwHashTablePos));
            dwTableSize = pHeader->dwHashTableSize * sizeof(TMPQHash);
            dwCmpSize = (DWORD)pHeader->HashTableSize64;

            // Read, decrypt and uncompress the hash table
            pHashTable = (TMPQHash *)LoadMpqTable(ha, ByteOffset, dwCmpSize, dwTableSize, MPQ_KEY_HASH_TABLE, &bHashTableIsCut);
//          DumpHashTable(pHashTable, pHeader->dwHashTableSize);

            // If the hash table was cut, we can/have to defragment it
            if(pHashTable != NULL && bHashTableIsCut)
                ha->dwFlags |= (MPQ_FLAG_MALFORMED | MPQ_FLAG_HASH_TABLE_CUT);
            break;

        case MPQ_SUBTYPE_SQP:
            return NULL;
            break;

        case MPQ_SUBTYPE_MPK:
            return NULL;
            break;
    }

    // Return the loaded hash table
    return pHashTable;
}

TMPQBlock * LoadBlockTable(TMPQArchive * ha, bool /* bDontFixEntries */)
{
    TMPQHeader * pHeader = ha->pHeader;
    TMPQBlock * pBlockTable = NULL;
    ULONGLONG ByteOffset;
    DWORD dwTableSize;
    DWORD dwCmpSize;
    bool bBlockTableIsCut = false;

    // Note: It is possible that the block table starts at offset 0
    // Example: MPQ_2016_v1_ProtectedMap_HashOffsIsZero.w3x
//  if(pHeader->dwBlockTablePos == 0 && pHeader->wBlockTablePosHi == 0)
//      return NULL;

    // Do nothing if the block table size is zero
    if(pHeader->dwBlockTableSize == 0)
        return NULL;

    // Load the block table for MPQ variations
    switch(ha->dwSubType)
    {
        case MPQ_SUBTYPE_MPQ:

            // Calculate byte position of the block table
            ByteOffset = FileOffsetFromMpqOffset(ha, MAKE_OFFSET64(pHeader->wBlockTablePosHi, pHeader->dwBlockTablePos));
            dwTableSize = pHeader->dwBlockTableSize * sizeof(TMPQBlock);
            dwCmpSize = (DWORD)pHeader->BlockTableSize64;

            // Read, decrypt and uncompress the block table
            pBlockTable = (TMPQBlock * )LoadMpqTable(ha, ByteOffset, dwCmpSize, dwTableSize, MPQ_KEY_BLOCK_TABLE, &bBlockTableIsCut);

            // If the block table was cut, we need to remember it
            if(pBlockTable != NULL && bBlockTableIsCut)
                ha->dwFlags |= (MPQ_FLAG_MALFORMED | MPQ_FLAG_BLOCK_TABLE_CUT);
            break;

        case MPQ_SUBTYPE_SQP:

            return NULL;
            break;

        case MPQ_SUBTYPE_MPK:

            return NULL;
            break;
    }

    return pBlockTable;
}

int LoadAnyHashTable(TMPQArchive * ha)
{
    TMPQHeader * pHeader = ha->pHeader;

    // If the MPQ archive is empty, don't bother trying to load anything
    if(pHeader->dwHashTableSize == 0 && pHeader->HetTableSize64 == 0)
        return ERROR_BAD_FORMAT;

    // Try to load classic hash table
    // Note that we load the classic hash table even when HET table exists,
    // because if the MPQ gets modified and saved, hash table must be there
    if(pHeader->dwHashTableSize)
        ha->pHashTable = LoadHashTable(ha);

    // At least one of the tables must be present
    if(ha->pHetTable == NULL && ha->pHashTable == NULL)
        return ERROR_FILE_CORRUPT;

    // Set the maximum file count to the size of the hash table.
    // Note: We don't care about HET table limits, because HET table is rebuilt
    // after each file add/rename/delete.
    ha->dwMaxFileCount = (ha->pHashTable != NULL) ? pHeader->dwHashTableSize : HASH_TABLE_SIZE_MAX;
    return ERROR_SUCCESS;
}

static int BuildFileTable_Classic(TMPQArchive * ha)
{
    TMPQHeader * pHeader = ha->pHeader;
    TMPQBlock * pBlockTable;
    int nError = ERROR_SUCCESS;

    // Sanity checks
    assert(ha->pHashTable != NULL);
    assert(ha->pFileTable != NULL);

    // If the MPQ has no block table, do nothing
    if(pHeader->dwBlockTableSize == 0)
        return ERROR_SUCCESS;
    assert(ha->dwFileTableSize >= pHeader->dwBlockTableSize);

    // Load the block table
    // WARNING! ha->pFileTable can change in the process!!
    pBlockTable = (TMPQBlock *)LoadBlockTable(ha);
    if(pBlockTable != NULL)
    {
        nError = BuildFileTableFromBlockTable(ha, pBlockTable);
        STORM_FREE(pBlockTable);
    }
    else
    {
        nError = ERROR_NOT_ENOUGH_MEMORY;
    }

    return nError;
}

int BuildFileTable(TMPQArchive * ha)
{
    DWORD dwFileTableSize;
    bool bFileTableCreated = false;

    // Sanity checks
    assert(ha->pFileTable == NULL);
    assert(ha->dwFileTableSize == 0);
    assert(ha->dwMaxFileCount != 0);

    // Determine the allocation size for the file table
    dwFileTableSize = STORMLIB_MAX(ha->pHeader->dwBlockTableSize, ha->dwMaxFileCount);

    // Allocate the file table with size determined before
    ha->pFileTable = STORM_ALLOC(TFileEntry, dwFileTableSize);
    if(ha->pFileTable == NULL)
        return ERROR_NOT_ENOUGH_MEMORY;

    // Fill the table with zeros
    memset(ha->pFileTable, 0, dwFileTableSize * sizeof(TFileEntry));
    ha->dwFileTableSize = dwFileTableSize;

    // If we have hash table, we load the file table from the block table
    // Note: If block table is corrupt or missing, we set the archive as read only
    if(ha->pHashTable != NULL)
    {
        if(BuildFileTable_Classic(ha) != ERROR_SUCCESS)
            ha->dwFlags |= MPQ_FLAG_READ_ONLY;
        else
            bFileTableCreated = true;
    }

    // Return result
    return bFileTableCreated ? ERROR_SUCCESS : ERROR_FILE_CORRUPT;
}

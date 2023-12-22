// MPQ support routines for PhysicsFS.

#include "PhysFSArchiverMPQ.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>

#if defined(PHYSFS_EXTERNAL_STORMLIB)
#include <StormLib.h>
#else
#include "StormLib/StormLib.h"
#endif

#if defined(PHYSFS_DYNAMIC_STORMLIB)
#include "Utils/LoadLibrary.h"
#endif
#include "Utils/Log.h"

typedef bool (WINAPI* StormFileOpenArchive)(const TCHAR*, DWORD, DWORD, HANDLE*);
typedef bool (WINAPI* StormFileCloseArchive)(HANDLE);
typedef bool (WINAPI* StormFileOpenFileEx)(HANDLE, const char*, DWORD, HANDLE*);
typedef DWORD(WINAPI* StormFileGetFileSize)(HANDLE, LPDWORD);
typedef DWORD(WINAPI* StormFileSetFilePointer)(HANDLE, LONG, LONG*, DWORD);
typedef bool (WINAPI* StormFileReadFile)(HANDLE, void*, DWORD, LPDWORD, LPOVERLAPPED);
typedef bool (WINAPI* StormFileCloseFile)(HANDLE);
typedef bool (WINAPI* StormFileGetFileInfo)(HANDLE, SFileInfoClass, void*, DWORD, LPDWORD);

#if defined(PHYSFS_EXTERNAL_STORMLIB) || defined(PHYSFS_INTERNAL_STORMLIB)
static StormFileOpenArchive PHYSFS_SFileOpenArchive = SFileOpenArchive;
static StormFileCloseArchive PHYSFS_SFileCloseArchive = SFileCloseArchive;
static StormFileOpenFileEx PHYSFS_SFileOpenFileEx = SFileOpenFileEx;
static StormFileGetFileSize PHYSFS_SFileGetFileSize = SFileGetFileSize;
static StormFileSetFilePointer PHYSFS_SFileSetFilePointer = SFileSetFilePointer;
static StormFileReadFile PHYSFS_SFileReadFile = SFileReadFile;
static StormFileCloseFile PHYSFS_SFileCloseFile = SFileCloseFile;
static StormFileGetFileInfo PHYSFS_SFileGetFileInfo = SFileGetFileInfo;
#else
static StormFileOpenArchive PHYSFS_SFileOpenArchive = nullptr;
static StormFileCloseArchive PHYSFS_SFileCloseArchive = nullptr;
static StormFileOpenFileEx PHYSFS_SFileOpenFileEx = nullptr;
static StormFileGetFileSize PHYSFS_SFileGetFileSize = nullptr;
static StormFileSetFilePointer PHYSFS_SFileSetFilePointer = nullptr;
static StormFileReadFile PHYSFS_SFileReadFile = nullptr;
static StormFileCloseFile PHYSFS_SFileCloseFile = nullptr;
static StormFileGetFileInfo PHYSFS_SFileGetFileInfo = nullptr;
#endif

static const PHYSFS_Allocator* physfsAlloc{ nullptr };

struct MPQHandle
{
	PHYSFS_Io* io;
	HANDLE mpqHandle;
};

struct MPQFileHandle
{
	HANDLE fileHandle;
	PHYSFS_sint64 size;
};

static char* MPQ_strdup(const char* str)
{
	auto str2 = (char*)physfsAlloc->Malloc((PHYSFS_uint64)strlen(str) + 1);
	if (str2 != nullptr)
	{
		strcpy(str2, str);
	}
	return str2;
}

static PHYSFS_sint64 MPQ_read(PHYSFS_Io* io, void* buf, PHYSFS_uint64 len)
{
	auto handle = (MPQFileHandle*)io->opaque;
	DWORD dwBytesRead;
	DWORD dwBytesToRead = (DWORD)std::min(len, (PHYSFS_uint64)handle->size);
	PHYSFS_SFileReadFile(handle->fileHandle, buf, dwBytesToRead, &dwBytesRead, nullptr);
	if (dwBytesRead != dwBytesToRead)
	{
		return -1L;
	}
	return (PHYSFS_sint64)dwBytesRead;
}

static PHYSFS_sint64 MPQ_write(PHYSFS_Io* io, const void* b, PHYSFS_uint64 len)
{
	PHYSFS_setErrorCode(PHYSFS_ERR_READ_ONLY);
	return -1;
}

static PHYSFS_sint64 MPQ_tell(PHYSFS_Io* io)
{
	auto handle = (MPQFileHandle*)io->opaque;
	LONG FilePosHi = 0;
	DWORD FilePosLo = PHYSFS_SFileSetFilePointer(handle->fileHandle, 0, &FilePosHi, FILE_CURRENT);
	return (((PHYSFS_sint64)FilePosHi << 32) | (PHYSFS_sint64)FilePosLo);
}

static int MPQ_seek(PHYSFS_Io* io, PHYSFS_uint64 offset)
{
	auto handle = (MPQFileHandle*)io->opaque;
	LONG DeltaPosHi = (LONG)(offset >> 32);
	LONG DeltaPosLo = (LONG)(offset);
	PHYSFS_SFileSetFilePointer(handle->fileHandle, DeltaPosLo, &DeltaPosHi, FILE_BEGIN);
	return 1;
}

static PHYSFS_sint64 MPQ_length(PHYSFS_Io* io)
{
	auto handle = (MPQFileHandle*)io->opaque;
	if (handle->fileHandle != nullptr)
	{
		return handle->size;
	}
	return -1L;
}

static PHYSFS_Io* MPQ_duplicate(PHYSFS_Io* io)
{
	PHYSFS_setErrorCode(PHYSFS_ERR_UNSUPPORTED);
	return nullptr;
}

static int MPQ_flush(PHYSFS_Io* io)
{
	return 1;	// no write support
}

static void MPQ_destroy(PHYSFS_Io* io)
{
	auto handle = (MPQFileHandle*)io->opaque;
	if (handle != nullptr)
	{
		if (handle->fileHandle != nullptr)
		{
			PHYSFS_SFileCloseFile(handle->fileHandle);
		}
		physfsAlloc->Free(handle);
	}
	physfsAlloc->Free(io);
}

static const PHYSFS_Io MPQ_Io =
{
	0, nullptr,
	MPQ_read,
	MPQ_write,
	MPQ_seek,
	MPQ_tell,
	MPQ_length,
	MPQ_duplicate,
	MPQ_flush,
	MPQ_destroy
};

#if defined(PHYSFS_DYNAMIC_STORMLIB) && (defined(_WIN32) || defined(__unix__))
static void MPQ_LoadExternalStormLib()
{
	static bool tryLoadOnce = false;

	if (tryLoadOnce == true && PHYSFS_SFileOpenArchive != nullptr)
	{
		return;
	}
	else
	{
		SPDLOG_INFO("Loading external StormLib");
		tryLoadOnce = true;
	}

#if defined(_WIN32)
	auto hStormLib = Utils::LoadExternalLibrary("StormLib.dll");
#else
	auto hStormLib = Utils::LoadExternalLibrary("./libstorm.so");
#endif
	if (hStormLib == nullptr)
	{
		SPDLOG_INFO("External StormLib not found");
		return;
	}

	PHYSFS_SFileOpenArchive = (StormFileOpenArchive)Utils::GetFunctionAddress(hStormLib, "SFileOpenArchive");
	PHYSFS_SFileCloseArchive = (StormFileCloseArchive)Utils::GetFunctionAddress(hStormLib, "SFileCloseArchive");
	PHYSFS_SFileOpenFileEx = (StormFileOpenFileEx)Utils::GetFunctionAddress(hStormLib, "SFileOpenFileEx");
	PHYSFS_SFileGetFileSize = (StormFileGetFileSize)Utils::GetFunctionAddress(hStormLib, "SFileGetFileSize");
	PHYSFS_SFileSetFilePointer = (StormFileSetFilePointer)Utils::GetFunctionAddress(hStormLib, "SFileSetFilePointer");
	PHYSFS_SFileReadFile = (StormFileReadFile)Utils::GetFunctionAddress(hStormLib, "SFileReadFile");
	PHYSFS_SFileCloseFile = (StormFileCloseFile)Utils::GetFunctionAddress(hStormLib, "SFileCloseFile");
	PHYSFS_SFileGetFileInfo = (StormFileGetFileInfo)Utils::GetFunctionAddress(hStormLib, "SFileGetFileInfo");

#if defined(PHYSFS_EXTERNAL_STORMLIB) || defined(PHYSFS_INTERNAL_STORMLIB)
	if (PHYSFS_SFileOpenArchive == nullptr ||
		PHYSFS_SFileCloseArchive == nullptr ||
		PHYSFS_SFileOpenFileEx == nullptr ||
		PHYSFS_SFileGetFileSize == nullptr ||
		PHYSFS_SFileSetFilePointer == nullptr ||
		PHYSFS_SFileReadFile == nullptr ||
		PHYSFS_SFileCloseFile == nullptr ||
		PHYSFS_SFileGetFileInfo == nullptr)
	{
		SPDLOG_INFO("Using internal StormLib");
		PHYSFS_SFileOpenArchive = SFileOpenArchive;
		PHYSFS_SFileCloseArchive = SFileCloseArchive;
		PHYSFS_SFileOpenFileEx = SFileOpenFileEx;
		PHYSFS_SFileGetFileSize = SFileGetFileSize;
		PHYSFS_SFileSetFilePointer = SFileSetFilePointer;
		PHYSFS_SFileReadFile = SFileReadFile;
		PHYSFS_SFileCloseFile = SFileCloseFile;
		PHYSFS_SFileGetFileInfo = SFileGetFileInfo;
	}
	else
	{
		SPDLOG_INFO("Using external StormLib");
	}
#endif
}
#endif

static void* MPQ_openArchive(PHYSFS_Io* io, const char* name,
	int forWriting, int* claimed)
{
	assert(io != nullptr);

#if defined(PHYSFS_DYNAMIC_STORMLIB) && (defined(_WIN32) || defined(__unix__))
	MPQ_LoadExternalStormLib();
#endif

	if (PHYSFS_SFileOpenArchive == nullptr)
	{
		PHYSFS_setErrorCode(PHYSFS_ERR_UNSUPPORTED);
		return nullptr;
	}

	if (forWriting)
	{
		PHYSFS_setErrorCode(PHYSFS_ERR_READ_ONLY);
		return nullptr;
	}

	if (physfsAlloc == nullptr)
	{
		physfsAlloc = PHYSFS_getAllocator();
	}

	HANDLE hMpq = nullptr;
	if (PHYSFS_SFileOpenArchive(name, 0, MPQ_OPEN_READ_ONLY, &hMpq) == false)
	{
		return nullptr;
	}

	*claimed = 1;

	auto handle = (MPQHandle*)physfsAlloc->Malloc(sizeof(MPQHandle));
	if (handle == nullptr)
	{
		PHYSFS_SFileCloseArchive(hMpq);
		return nullptr;
	}

	handle->io = io;
	handle->mpqHandle = hMpq;

	return handle;
}

static PHYSFS_EnumerateCallbackResult MPQ_enumerate(void* opaque,
	const char* dname, PHYSFS_EnumerateCallback cb,
	const char* origdir, void* callbackdata)
{
	return PHYSFS_ENUM_ERROR;
}

static char* MPQ_getValidFilename(const char* filename)
{
	auto filename2 = MPQ_strdup(filename);
	if (filename2 == nullptr)
	{
		return nullptr;
	}

	auto chr = filename2;
	while (chr[0] != 0)
	{
		if (chr[0] == '/')
		{
			chr[0] = '\\';
		}
		chr++;
	}
	return filename2;
}

static PHYSFS_Io* MPQ_openRead(void* opaque, const char* filename)
{
	if (opaque == nullptr)
	{
		return nullptr;
	}

	auto filename2 = MPQ_getValidFilename(filename);
	if (filename2 == nullptr)
	{
		return nullptr;
	}

	HANDLE hFile;
	auto success = PHYSFS_SFileOpenFileEx(((MPQHandle*)opaque)->mpqHandle, filename2, 0, &hFile);
	physfsAlloc->Free(filename2);

	if (success == false)
	{
		return nullptr;
	}

	auto physfsIo = (PHYSFS_Io*)physfsAlloc->Malloc(sizeof(PHYSFS_Io));
	if (physfsIo == nullptr)
	{
		PHYSFS_SFileCloseFile(hFile);
		return nullptr;
	}

	auto handle = (MPQFileHandle*)physfsAlloc->Malloc(sizeof(MPQFileHandle));
	if (handle == nullptr)
	{
		physfsAlloc->Free(physfsIo);
		PHYSFS_SFileCloseFile(hFile);
		return nullptr;
	}

	DWORD dwFileSizeHi = 0xCCCCCCCC;
	DWORD dwFileSizeLo = PHYSFS_SFileGetFileSize(hFile, &dwFileSizeHi);
	if (dwFileSizeLo == SFILE_INVALID_SIZE || dwFileSizeHi != 0)
	{
		physfsAlloc->Free(physfsIo);
		physfsAlloc->Free(handle);
		PHYSFS_SFileCloseFile(hFile);
		return nullptr;
	}

	handle->fileHandle = hFile;
	handle->size = (PHYSFS_sint64)dwFileSizeLo;

	memcpy(physfsIo, &MPQ_Io, sizeof(PHYSFS_Io));
	physfsIo->opaque = handle;

	return physfsIo;
}

static PHYSFS_Io* MPQ_openWrite(void* opaque, const char* filename)
{
	PHYSFS_setErrorCode(PHYSFS_ERR_READ_ONLY);
	return nullptr;
}

static PHYSFS_Io* MPQ_openAppend(void* opaque, const char* filename)
{
	PHYSFS_setErrorCode(PHYSFS_ERR_READ_ONLY);
	return nullptr;
}

static int MPQ_remove(void* opaque, const char* name)
{
	PHYSFS_setErrorCode(PHYSFS_ERR_READ_ONLY);
	return 0;
}

static int MPQ_mkdir(void* opaque, const char* name)
{
	PHYSFS_setErrorCode(PHYSFS_ERR_READ_ONLY);
	return 0;
}

static int MPQ_stat(void* opaque, const char* filename, PHYSFS_Stat* stat)
{
	if (opaque == nullptr)
	{
		return 0;
	}

	auto filename2 = MPQ_getValidFilename(filename);
	if (filename2 == nullptr)
	{
		return 0;
	}

	HANDLE hFile;
	auto success = PHYSFS_SFileOpenFileEx(((MPQHandle*)opaque)->mpqHandle, filename2, 0, &hFile);
	physfsAlloc->Free(filename2);

	if (success == false)
	{
		return 0;
	}

	DWORD fileSize = 0;
	PHYSFS_SFileGetFileInfo(hFile, SFileInfoFileSize, &fileSize, sizeof(fileSize), nullptr);
	stat->filesize = fileSize;

	stat->modtime = 0;
	PHYSFS_SFileGetFileInfo(hFile, SFileInfoFileTime, &stat->modtime, sizeof(stat->modtime), nullptr);
	stat->createtime = stat->modtime;
	stat->accesstime = 0;
	stat->filetype = PHYSFS_FILETYPE_REGULAR;
	stat->readonly = 1;

	PHYSFS_SFileCloseFile(hFile);

	return 1;
}

static void MPQ_closeArchive(void* opaque)
{
	auto handle = (MPQHandle*)opaque;
	if (handle == nullptr)
	{
		return;
	}
	PHYSFS_SFileCloseArchive(handle->mpqHandle);
	handle->io->destroy(handle->io);
	physfsAlloc->Free(handle);
}

const PHYSFS_Archiver PHYSFS_Archiver_MPQ =
{
	0,
	{
		"MPQ",
		"Blizzard Entertainment format",
		"",
		"",
		1,	// supportsSymlinks
	},
	MPQ_openArchive,
	MPQ_enumerate,
	MPQ_openRead,
	MPQ_openWrite,
	MPQ_openAppend,
	MPQ_remove,
	MPQ_mkdir,
	MPQ_stat,
	MPQ_closeArchive
};

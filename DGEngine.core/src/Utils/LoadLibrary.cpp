#include "LoadLibrary.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__unix__)
#include <dlfcn.h>
#endif

namespace Utils
{
	void* LoadExternalLibrary(const char* libraryName)
	{
#if defined(_WIN32)
	return LoadLibraryA(libraryName);
#elif defined(__unix__)
	return dlopen(libraryName, RTLD_LAZY | RTLD_LOCAL);
#else
	return nullptr;
#endif
	}

	void* GetFunctionAddress(void* libraryHandle, const char* functionName)
	{
#if defined(_WIN32)
	return GetProcAddress((HMODULE)libraryHandle, functionName);
#elif defined(__unix__)
	return dlsym(libraryHandle, functionName);
#else
	return nullptr;
#endif
	}
}

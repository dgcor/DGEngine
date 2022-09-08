#pragma once

namespace Utils
{
	void* LoadExternalLibrary(const char* libraryName);

	void* GetFunctionAddress(void* libraryHandle, const char* functionName);
}

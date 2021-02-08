#pragma once
#include "Windows.h"
#include "string"
#ifdef COMPILE_DLL_XHTYE102474
_declspec(dllexport) void __stdcall inject(DWORD pid, std::string target, std::string replacement);
#else
_declspec(dllimport) void __stdcall inject(DWORD pid, std::string target, std::string replacement);
#endif
#include "windows.h"
#include "iostream"
#include "Header.h"

int main() 
{
	DWORD pid;
	HANDLE targetProcess;
	auto dllPath = "../../MyDll/Debug/MyDll.dll";
	auto injectDecoratedName = "?inject@@YGXKV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@0@Z";

	std::cout << "Pid: ";
	std::cin >> pid;

	targetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (targetProcess) {
		auto LoadLibraryAStartAddr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		auto DllPathRemoteParam = (LPVOID)VirtualAllocEx(targetProcess, NULL, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(targetProcess, DllPathRemoteParam, dllPath, strlen(dllPath) + 1, NULL);
		auto remoteThread = CreateRemoteThread(targetProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAStartAddr, DllPathRemoteParam, 0, NULL);
		WaitForSingleObject(remoteThread, INFINITE);
		std::cout << "DLL injected" << std::endl;
		CloseHandle(remoteThread);
		CloseHandle(targetProcess);
		fseek(stdin, 0, SEEK_SET);
		std::cin.get();
		inject(pid, "You were hacked", "static hack");
		std::cin.get();
		auto HModule = LoadLibrary(L"../../MyDll/Debug/MyDll.dll");
		typedef void __stdcall Tinject(DWORD pid, std::string target, std::string replacement);
		if (HModule != 0) {
			auto injectFromDll = (Tinject*)GetProcAddress(HModule, injectDecoratedName);
			if (injectFromDll == nullptr) {
				const int x = GetLastError();
				std::cout << x;
			}
			else {
				injectFromDll(pid, "static hack", "dynamic hack");
			}
		}
		FreeLibrary(HModule);
	}
	else {
		std::cout << "Error" << std::endl;
	}
	return 0;
}
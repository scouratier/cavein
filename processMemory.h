// Simple class that holds the data and methods for the memory manipulation
#include "common.h"

const int	interfaceMethodCount = 119;

class processMemory 
{
public:
	processMemory();
	processMemory(LPCWSTR,WCHAR*);

	HWND GethWnd();
	DWORD GetPid();
	HANDLE GethProcess();
	DWORD GetBaseAddress();

	BOOL SetDebugPrivileges();
	BOOL SetBaseAddress();

	SIZE_T Read(DWORD,CHAR*,SIZE_T);
	SIZE_T Write(DWORD,CHAR*,SIZE_T);

	BOOL GetEndScene();

private:
	LPCWSTR			windowName_;
	WCHAR*			processName_;
	HWND			hWnd_; 
	DWORD			pid_;
	HANDLE			hProcess_;
	DWORD			baseAddress_;
	WCHAR*			d3d9_;
	DWORD			d3d9ModuleBaseAddr_;
	HMODULE			dllHandle_;

	uintptr_t**		deviceFunctionAddresses_;
};
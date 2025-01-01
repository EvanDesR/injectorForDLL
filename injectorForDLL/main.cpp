//Injector Program
#include <windows.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//Written, using UNICODE for all strings, and UNICODE WinAPI functions, as windows converts ANSI (8bit chars) to UTF-16 (wchar_t) anyways
//This is because, char only supports 0-0255, and wchar_t supports 0-65535, which is needed to support all glpyhs(symbols) from all languages.
//If only ANSI was supported, then windows would have to convert between languages.
// So tldr: When working with window API, or any sysinterals, just use wchar_t , and L"string" for all strings.
// Using L infront of a literal, tells windows that it is to be represented as a wide char(2byte) string, and not a C-Style/ANSI(1 byte) char string.


//DLL path is not being written into memory buffer properly. After 

int findTarget(const WCHAR* target)
{
	HANDLE processSnapshot;
	PROCESSENTRY32 processEntry;
	int pid = 0;
	processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (processSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(processSnapshot, &processEntry))
	{
		CloseHandle(processSnapshot);
		return 0;
	}

	while (Process32Next(processSnapshot, &processEntry))
	{
		if (lstrcmpiW(target, processEntry.szExeFile) == 0)
		{
			pid = processEntry.th32ProcessID;
			break;
		}
	}
	CloseHandle(processSnapshot);
	return pid;
}


int main()
{
	HANDLE procHandle;
	PVOID remoteBuffer;
	PTHREAD_START_ROUTINE procLoadLibrary = NULL;
	int pid = 0;
	WCHAR dll[] = L"C:\\Users\\evand\\source\\repos\\calcDLL\\x64\\Release\\calcDLL.dll";  //CHANGE ME
	WCHAR target[] = L"notepad.exe";


	pid = findTarget(target);
	if (pid == 0)
	{
		printf("PID Not Found \n");
		return -1;
	}
	printf("PID: %d, Injecting", pid);

	procLoadLibrary = (PTHREAD_START_ROUTINE)GetProcAddress((GetModuleHandle(L"Kernel32")), "LoadLibraryW"); //get loadLibrary address in kernel32.dll
	if (procLoadLibrary == NULL)
	{
		printf("LoadLibraryA not found!\n");
		return -1;
	}

	procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)(pid)); //get handle for notepad

	if (procHandle != NULL)
	{
		remoteBuffer = VirtualAllocEx(procHandle, NULL, sizeof(dll), MEM_COMMIT, PAGE_READWRITE); //allocate remote buffer

		WriteProcessMemory(procHandle, remoteBuffer, (LPVOID)dll, sizeof(dll), NULL);
		//write dll path into memory

		//Create thread in notepad, and run loadlibrary with arg pointing to buffer (path)
		CreateRemoteThread(procHandle, NULL, 0, procLoadLibrary, remoteBuffer, 0, NULL);

		printf("Remote Buffer is at: 0x%p", remoteBuffer); //addr of buffer

		CloseHandle(procHandle);
	}
	return 1;
}
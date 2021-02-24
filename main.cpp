#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

PTCHAR GetArgs(VOID)
{
	PTCHAR p = GetCommandLine();

	if(*p == TEXT('"'))
	{
		while(*p == TEXT('"')) p++; // skip any leading quotes
		while(*p != TEXT('"')) p++; // skip over exe name
		while(*p == TEXT('"')) p++; // skip any trailing quotes
	}
	else  // didn't start with a quote, so skip non-whitespace
	{
		while((*p) && (*p != TEXT(' ')) && (*p != TEXT('\t'))) p++;
	}
	// skip whitespace
	while((*p) && ((*p == TEXT(' ')) || (*p == TEXT('\t')))) p++;
	return p;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	PTCHAR Args;
	Args = GetArgs();
	DWORD ExitCode;

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	// if CreateProcess fails, exit with GetLastError, otherwise exit with (process ID * -1)
	ExitCode = CreateProcess(NULL, Args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi) \
        ? pi.dwProcessId : GetLastError() * -1;

    WaitForSingleObject(pi.hProcess, -1);
    GetExitCodeProcess( pi.hProcess, &ExitCode );

	printf("%d\n", ExitCode);
	exit(ExitCode);
}

#pragma once
#include<windows.h>
#include <tlhelp32.h>
#include<iostream>
#include<cstring>
#define NAMELENGTH 260
class PROCESS_DATA
{
public:
	DWORD PID;
	WCHAR NAME[NAMELENGTH];
    LARGE_INTEGER last_time;
	bool is_x64;
	float CPU_PERCENT;
	PROCESS_DATA(PROCESSENTRY32 progress_snap);
	BOOL Is64BitOS();
	BOOL Is64BitPorcess(DWORD dwProcessID);
	float GetProcessCpuPercent(const HANDLE Process_handle, const DWORD dwElepsedTime, int number_of_cores);
};
#include"PROCESS_DATA.h"
PROCESS_DATA::PROCESS_DATA(PROCESSENTRY32 progress_snap)
{
	PID = progress_snap.th32ProcessID;
	progress_snap.szExeFile;
	is_x64 = Is64BitPorcess(PID);
	for (int i = 0; i < NAMELENGTH; ++i)
		NAME[i] = progress_snap.szExeFile[i];
}
BOOL PROCESS_DATA::Is64BitOS()
{
    typedef VOID(WINAPI* LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
    LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandleW(L"kernel32"), "GetNativeSystemInfo");
    if (fnGetNativeSystemInfo)
    {
        SYSTEM_INFO stInfo = { 0 };
        fnGetNativeSystemInfo(&stInfo);
        if (stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64
            || stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        {
            return TRUE;
        }
    }
    return FALSE;
}
BOOL PROCESS_DATA::Is64BitPorcess(DWORD dwProcessID)
{
	if (!Is64BitOS())
	{
		return FALSE;
	}
	else
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
		if (hProcess)
		{
			typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
			LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleW(L"kernel32"), "IsWow64Process");
			if (NULL != fnIsWow64Process)
			{
				BOOL bIsWow64 = FALSE;
				fnIsWow64Process(hProcess, &bIsWow64);
				CloseHandle(hProcess);
				if (bIsWow64)
				{
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
float PROCESS_DATA::GetProcessCpuPercent(const HANDLE Process_handle, const DWORD dwElepsedTime, int number_of_cores)
{
	float CPU_PERCENTAGE = 0;
	BOOL bRetCode = FALSE;

	FILETIME TRASH;
	FILETIME KernelTime, UserTime;
	LARGE_INTEGER lgKernelTime;
	LARGE_INTEGER lgUserTime;
	LARGE_INTEGER lgCurTime;

	bRetCode = GetProcessTimes(Process_handle, &TRASH, &TRASH, &KernelTime, &UserTime);
	if (bRetCode)
	{
		lgKernelTime.HighPart = KernelTime.dwHighDateTime;
		lgKernelTime.LowPart = KernelTime.dwLowDateTime;
		lgUserTime.HighPart = UserTime.dwHighDateTime;
		lgUserTime.LowPart = UserTime.dwLowDateTime;
		lgCurTime.QuadPart = (lgKernelTime.QuadPart + lgUserTime.QuadPart) / 10000;
		CPU_PERCENTAGE = ((float)(lgCurTime.QuadPart - last_time.QuadPart) * 100 / dwElepsedTime) / number_of_cores;
		last_time = lgCurTime;
	}
	else
	{
		CPU_PERCENTAGE = -1;
	}

	return CPU_PERCENTAGE;
}

#include"PROCESS_LIB.h"
PROCESS_LIB::PROCESS_LIB()
{
	reading = 0;
	processing = 0;
	processor_number = get_processor_number();
	PROCESS_COUNT = 0;
	for (int i = 0; i < MAXIUM_PROCESS; ++i)
		LIB[i] = NULL;
}
int PROCESS_LIB::Read_System_Process()
{
	check_processing();
	start_reading();
	PROCESS_COUNT = 0;
	for (int i = 0; i < PROCESS_COUNT; ++i)
	{
		delete LIB[i];
		LIB[i] = NULL;
	}
	PROCESSENTRY32 temp;
	temp.dwSize = sizeof(temp);
	HANDLE Process_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process_handle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	bool keep = Process32First(Process_handle, &temp);
	while (keep)
	{
		LIB[PROCESS_COUNT] = new PROCESS_DATA(temp);
		keep = Process32Next(Process_handle, &temp);
		++PROCESS_COUNT;
	}
	for (int i = 0; i < PROCESS_COUNT; ++i)
	{
		HANDLE temphandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, LIB[i]->PID);
		LIB[i]->CPU_PERCENT = LIB[i]->GetProcessCpuPercent(temphandle, delay, processor_number);
		CloseHandle(temphandle);
	}
    CloseHandle(Process_handle);
    /*Sleep(delay);
    for (int i = 0; i < PROCESS_COUNT; ++i)
    {
        HANDLE temphandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, LIB[i]->PID);
        LIB[i]->CPU_PERCENT = LIB[i]->GetProcessCpuPercent(temphandle, delay, processor_number);
        CloseHandle(temphandle);
    }
    stop_reading();
    return 0;*/
}
int PROCESS_LIB::Read_System_Process2()
{
    for (int i = 0; i < PROCESS_COUNT; ++i)
    {
        HANDLE temphandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, LIB[i]->PID);
        LIB[i]->CPU_PERCENT = LIB[i]->GetProcessCpuPercent(temphandle, delay, processor_number);
        CloseHandle(temphandle);
    }
    stop_reading();
    return 0;
}
void PROCESS_LIB::check_reading()
{
	for (;;)
	{
		if (reading)
			Sleep(100);
		else
			break;
	}
}
void PROCESS_LIB::check_processing()
{
	for (;;)
	{
		if (processing)
			Sleep(100);
		else
			break;
	}
}
PROCESS_DATA* PROCESS_LIB::Get_process_by_PID(int pid)
{
	check_reading();
	for (int i = 0; i < PROCESS_COUNT; ++i)
	{
		if (LIB[i]->PID == pid)
		{
			return LIB[i];
		}
	}
	return NULL;
}
PROCESS_DATA** PROCESS_LIB::Get_process_list()
{
	PROCESS_DATA** list = new PROCESS_DATA * [MAXIUM_PROCESS];;
	check_reading();
	for (int i = 0; i < MAXIUM_PROCESS; ++i)
		list[i] = NULL;
	for (int i = 0; i < PROCESS_COUNT; ++i)
		list[i] = LIB[i];
	return list;
}
int PROCESS_LIB::Get_Process_Count()
{
	return PROCESS_COUNT;
}
int PROCESS_LIB::get_processor_number()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}
void PROCESS_LIB::start_processing()
{
	processing = 1;
}
void PROCESS_LIB::stop_processing()
{
	processing = 0;
}
void PROCESS_LIB::start_reading()
{
	reading = 1;
}
void PROCESS_LIB::stop_reading()
{
	reading = 0;
}

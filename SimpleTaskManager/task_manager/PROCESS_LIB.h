#pragma once
#include "PROCESS_DATA.h"
#include <iomanip>
#define delay 500
using namespace std;
#define MAXIUM_PROCESS 500
class PROCESS_LIB
{
public:
	PROCESS_LIB();
    int Read_System_Process();
    int Read_System_Process2();
	void check_reading();
	void check_processing();
	PROCESS_DATA* Get_process_by_PID(int pid);
	PROCESS_DATA** Get_process_list();
	int Get_Process_Count();
	int get_processor_number();
	void start_processing();
	void stop_processing();
	void start_reading();
	void stop_reading();
	bool reading;
	bool processing;
	PROCESS_DATA* LIB[MAXIUM_PROCESS];
	int PROCESS_COUNT;
	int processor_number;
};

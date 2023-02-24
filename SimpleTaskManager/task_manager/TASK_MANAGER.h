#pragma once
#include "PROCESS_LIB.h"
#include <conio.h>
#include <algorithm>
#include <time.h>
#define FILENAME "data.txt"
bool cmp_by_pid(PROCESS_DATA* p1, PROCESS_DATA* p2);
bool cmp_by_name(PROCESS_DATA* p1, PROCESS_DATA* p2);
bool cmp_by_cpu_per(PROCESS_DATA* p1, PROCESS_DATA* p2);
class Task_Manager
{
public:
	char mode;
	PROCESS_LIB lib;
	Task_Manager();
	int start();
	int monitor();
	void write();
	int init();
	void Main_menu();
	void show_all_process();
	void show_process_data(PROCESS_DATA* process);
	void search_process();
};

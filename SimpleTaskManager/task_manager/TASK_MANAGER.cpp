#include "TASK_MANAGER.h"
Task_Manager::Task_Manager()
{
	mode = 'p';
}
int Task_Manager::start()
{
    ;
	return 0;
}
int Task_Manager::monitor()
{
    ;
}
void Task_Manager::write()
{
	time_t now = time(NULL);
	char time_str[20];
	_itoa(now, time_str, 10);
	for (int i = 0;; ++i)
	{
		if (time_str[i] == '\0')
		{
			time_str[i] = '.';
			time_str[i + 1] = 't';
			time_str[i + 2] = 'x';
			time_str[i + 3] = 't';
			time_str[i + 4] = '\0';
			break;
		}
	}
	FILE* pfile = fopen(time_str, "w+");
	PROCESS_DATA** list = lib.Get_process_list();
	for (int i = 0; list[i] != NULL; ++i)
	{
		fprintf(pfile, "%d %ws %f %d\n", list[i]->PID, list[i]->NAME, list[i]->CPU_PERCENT, list[i]->is_x64);
	}
	fclose(pfile);
}
int Task_Manager::init()
{
	mode = 'p';
    return 0;
}
void Task_Manager::Main_menu()
{
	for (;;)
	{
		system("cls");
		cout << "* * * * * * * * * * * * * * * * * * * * * * * *" << endl;
		cout << "*  欢迎使用任务管理器,请选择您想使用的功能 :) *" << endl;
		cout << "* * * * * * * * * * * * * * * * * * * * * * * *" << endl;
		if (mode == 'p')
			cout << "1.显示所有进程[按pid排序]（使用n，p，c键改变排序方式）" << endl;
		else if (mode == 'n')
			cout << "1.显示所有进程[按进程名称排序]（使用n，p，c键改变排序方式）" << endl;
		else
			cout << "1.显示所有进程[按CPU占用率排序]（使用n，p，c键改变排序方式）" << endl;
		cout << "2.搜索进程" << endl;
		cout << "3.退出" << endl;
		cout << "注意：请以管理员权限启动本程序" << endl;
		int choice;
		for (;;)
		{
			if (_kbhit())
			{
				choice = _getch();
				if (choice == '1')
				{
					show_all_process();
					break;
				}
				else if (choice == '2')
				{
					search_process();
					break;
				}
				else if (choice == '3')
				{
					return;
				}
				else if (choice == 'p')
				{
					mode = 'p';
					break;
				}
				else if (choice == 'n')
				{
					mode = 'n';
					break;
				}
				else if (choice == 'c')
				{
					mode = 'c';
					break;
				}
			}
		}
	}
}
void Task_Manager::show_all_process()
{
	lib.start_processing();
	system("cls");
	cout << setw(8) << left << "PID" << setw(60) << "进程名称" << setw(10) << "  CPU占用率" << setw(10) << "  程序位数" << endl;
	PROCESS_DATA** list = lib.Get_process_list();
	if (mode == 'n')
		sort(list, list + lib.Get_Process_Count(), cmp_by_name);
	else if (mode == 'p')
		sort(list, list + lib.Get_Process_Count(), cmp_by_pid);
	else
		sort(list, list + lib.Get_Process_Count(), cmp_by_cpu_per);
	for (int i = 0; list[i] != NULL; ++i)
	{
		show_process_data(list[i]);
	}
	delete[] list;
	lib.stop_processing();
	cout << "任意键继续" << endl;
	_getch();
}
void Task_Manager::show_process_data(PROCESS_DATA* process)
{
	wcout.imbue(locale("chs"));
	cout << setw(8) << left << process->PID;
	wcout << setw(60) << left << process->NAME;
	if (process->CPU_PERCENT == -1)
		cout << setw(10) << fixed << setprecision(1) << right << "N/A";
	else
		cout << setw(10) << fixed << setprecision(1) << right << process->CPU_PERCENT;
	if (process->is_x64)
		cout << right << setw(10) << "x64" << endl;
	else
		cout << right << setw(10) << "x32" << endl;
}
void Task_Manager::search_process()
{
	system("cls");
	cout << "请输入想要查找的进程PID:";
	int pid = 0;
	cin >> pid;
	lib.start_processing();
	PROCESS_DATA* process = lib.Get_process_by_PID(pid);
	if (process == NULL)
	{
		lib.stop_processing();
		cout << "没有找到该进程，请检查您的输入" << endl;
		cout << "任意键继续" << endl;
		_getch();
	}
	else
	{
		for (;;)
		{
			lib.start_processing();
			PROCESS_DATA* process = lib.Get_process_by_PID(pid);
			if (process == NULL)
			{
				system("cls");
				cout << "该进程已退出" << endl << "任意键返回主菜单";
				_getch();
				break;
			}
			system("cls");
			cout << setw(8) << left << "PID" << setw(60) << "进程名称" << setw(10) << "  CPU占用率" << setw(10) << "  程序位数" << endl;
			show_process_data(process);
			lib.stop_processing();
			cout << "任意键返回主菜单" << endl;
			{
				if (_kbhit())
				{
					_getch();
					break;
				}
			}
			Sleep(delay);
		}
	}
}
bool cmp_by_pid(PROCESS_DATA* p1, PROCESS_DATA* p2)
{
	if (p1 == NULL)
		return false;
	else if (p2 == NULL)
		return true;
	else
		return p1->PID < p2->PID;
}
bool cmp_by_name(PROCESS_DATA* p1, PROCESS_DATA* p2)
{
	if (p1 == NULL)
		return false;
	else if (p2 == NULL)
		return true;
	else
		return wcscmp(p1->NAME, p2->NAME) < 0;
}
bool cmp_by_cpu_per(PROCESS_DATA* p1, PROCESS_DATA* p2)
{
	if (p1 == NULL)
		return false;
	else if (p2 == NULL)
		return true;
	else
		return p1->CPU_PERCENT > p2->CPU_PERCENT;
}

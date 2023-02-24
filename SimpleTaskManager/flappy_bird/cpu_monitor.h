#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;
class cpu_monitor
{
public:
    double m_fOldCPUIdleTime;
    double m_fOldCPUKernelTime;
    double m_fOldCPUUserTime;
    double FileTimeToDouble(FILETIME* pFiletime)
    {
        return (double)((*pFiletime).dwHighDateTime * 4.294967296E9) + (double)(*pFiletime).dwLowDateTime;
    }
    BOOL Initialize()
    {
        FILETIME ftIdle, ftKernel, ftUser;
        BOOL flag = FALSE;
        if (flag = GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
        {
            m_fOldCPUIdleTime = FileTimeToDouble(&ftIdle);
            m_fOldCPUKernelTime = FileTimeToDouble(&ftKernel);
            m_fOldCPUUserTime = FileTimeToDouble(&ftUser);

        }
        return flag;
    }
    int GetCPUUseRate()
    {
        int nCPUUseRate = -1;
        FILETIME ftIdle, ftKernel, ftUser;
        if (GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
        {
            double fCPUIdleTime = FileTimeToDouble(&ftIdle);
            double fCPUKernelTime = FileTimeToDouble(&ftKernel);
            double fCPUUserTime = FileTimeToDouble(&ftUser);
            nCPUUseRate = (int)(100.0 - (fCPUIdleTime - m_fOldCPUIdleTime) / (fCPUKernelTime - m_fOldCPUKernelTime + fCPUUserTime - m_fOldCPUUserTime) * 100.0);
            m_fOldCPUIdleTime = fCPUIdleTime;
            m_fOldCPUKernelTime = fCPUKernelTime;
            m_fOldCPUUserTime = fCPUUserTime;
        }
        return nCPUUseRate;
    }
};

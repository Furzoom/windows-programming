#include <windows.h>
#include <process.h>
#include <stdio.h>

DWORD g_tlsUsedTime;
void InitStartTime();
DWORD GetUsedTime();
UINT WINAPI ThreadFunc(LPVOID)
{
    int i;

    InitStartTime();
    i = 10000 * 10000;
    while (i--)
    {

    }
    printf(" This thread is comming to end. Thread ID: %-5d, Used Time: %d\n",
        ::GetCurrentThreadId(), GetUsedTime());
    return 0;
}

int main(int argc, char *argv[])
{
    UINT uId;
    int i;
    HANDLE h[10];
    g_tlsUsedTime = ::TlsAlloc();
    for (i = 0; i < 10; i++)
    {
        h[i] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);
    }
    for (i = 0; i < 10; i++)
    {
        ::WaitForSingleObject(h[i], INFINITE);
        ::CloseHandle(h[i]);
    }
    ::TlsFree(g_tlsUsedTime);
    return 0;
}

void InitStartTime()
{
    DWORD dwStart = ::GetTickCount();
    ::TlsSetValue(g_tlsUsedTime, (LPVOID)dwStart);
}

DWORD GetUsedTime()
{
    DWORD dwElapsed = ::GetTickCount();
    dwElapsed -= (DWORD)::TlsGetValue(g_tlsUsedTime);
    return dwElapsed;
}
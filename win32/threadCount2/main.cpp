#include <stdio.h>
#include <windows.h>
#include <process.h>

BOOL g_bContinue = TRUE;
int g_nCount1 = 0;
int g_nCount2 = 0;
CRITICAL_SECTION g_cs;
UINT WINAPI ThreadFunc(LPVOID);

int main(int argc, char* argv[])
{
    UINT uId;
    HANDLE h[2];
    ::InitializeCriticalSection(&g_cs);
    h[0] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);
    h[1] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);

    Sleep(100);
    g_bContinue = FALSE;
    ::WaitForMultipleObjects(2, h, TRUE, INFINITE);
    ::CloseHandle(h[0]);
    ::CloseHandle(h[1]);
    ::DeleteCriticalSection(&g_cs);
    printf("g_nCount1 = %d\n", g_nCount1);
    printf("g_nCount2 = %d\n", g_nCount2);

    return 0;
}

UINT WINAPI ThreadFunc(LPVOID)
{
    while (g_bContinue)
    {
        ::EnterCriticalSection(&g_cs);
        g_nCount1 ++;
        g_nCount2 ++;
        ::LeaveCriticalSection(&g_cs);
    }
    return 0;
}
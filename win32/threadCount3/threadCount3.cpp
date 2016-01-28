#include <windows.h>
#include <process.h>
#include <stdio.h>

UINT g_nCount1 = 0;
UINT g_nCount2 = 0;
BOOL g_bContinue = TRUE;
static UINT WINAPI ThreadFunc(LPVOID);
int main(int argc, char* argv[])
{
    UINT uId;
    HANDLE h[2];

    h[0] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);
    h[1] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);

    Sleep(100);
    g_bContinue = FALSE;
    ::WaitForMultipleObjects(2, h, TRUE, INFINITE);
    ::CloseHandle(h[0]);
    ::CloseHandle(h[1]);
    printf("g_nCount1 = %u\n", g_nCount1);
    printf("g_nCount2 = %u\n", g_nCount2);
    return 0;
}

static UINT WINAPI ThreadFunc(LPVOID)
{
    while (g_bContinue)
    {
        ::InterlockedIncrement(&g_nCount1);
        ::InterlockedIncrement(&g_nCount2);
    }
    return 0;
}
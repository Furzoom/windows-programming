#include <windows.h>
#include <process.h>
#include <stdio.h>

HANDLE g_hEvent;
static UINT WINAPI ChildThread(LPVOID);

int main(int argc, char* argv[])
{
    HANDLE hChildThread;
    UINT uId;

    g_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    hChildThread = (HANDLE)::_beginthreadex(NULL, 0, ChildThread, NULL, 0, &uId);
    printf("Please input <ENTER> to continue: ");
    getchar();
    ::SetEvent(g_hEvent);
    ::WaitForSingleObject(hChildThread, INFINITE);
    printf("All work has been finished.\n");
    ::CloseHandle(g_hEvent);
    ::CloseHandle(hChildThread);
    return 0;
}

UINT WINAPI ChildThread(LPVOID)
{
    ::WaitForSingleObject(g_hEvent, INFINITE);
    printf("  Child thread is working...\n");
    Sleep(3000);
    return 0;
}

#include <stdio.h>
#include <windows.h>

DWORD WINAPI ThreadProc(LPVOID lpParam);

int main(int argc, char* argv[])
{
    HANDLE hThread;
    DWORD dwThreadId;

    hThread = ::CreateThread(NULL, NULL, ThreadProc, NULL, 0, &dwThreadId);
    printf("Now another thread has been created. ID = %d\n", dwThreadId);

    // wait for thread terminate
    ::WaitForSingleObject(hThread, INFINITE);
    ::CloseHandle(hThread);
    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    int i = 0;
    while (i < 20)
    {
        printf("I am from a thread, count = %d\n", i++);
    }
    return 0;
}

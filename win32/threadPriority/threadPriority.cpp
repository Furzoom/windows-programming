#include <windows.h>
#include <stdio.h>

static DWORD WINAPI threadIdle(LPVOID lpParam);
static DWORD WINAPI threadNormal(LPVOID lpParam);

int main(int argc, char* argv[])
{
    DWORD dwThreadId;
    HANDLE h[2];

    h[0] = ::CreateThread(NULL, 0, threadIdle, NULL, CREATE_SUSPENDED, &dwThreadId);
    ::SetThreadPriority(h[0], THREAD_PRIORITY_IDLE);
    ::ResumeThread(h[0]);

    h[1] = ::CreateThread(NULL, 0, threadNormal, NULL, CREATE_SUSPENDED, &dwThreadId);
    ::SetThreadPriority(h[1], THREAD_PRIORITY_NORMAL);
    ::ResumeThread(h[1]);

    ::WaitForMultipleObjects(sizeof(h) / sizeof(HANDLE), h, TRUE, INFINITE);

    ::CloseHandle(h[0]);
    ::CloseHandle(h[1]);
    return 0;
}

DWORD WINAPI threadIdle(LPVOID lpParam)
{
    for (int i = 0; i < 10; i++)
    {
        printf("Idle %d\n", i);
    }
    return 0;
}

DWORD WINAPI threadNormal(LPVOID lpParam)
{
    for (int i = 0; i < 10; i++)
    {
        printf("Normal %d\n", i);
    }
    return 0;
}
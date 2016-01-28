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

    h[1] = ::CreateThread(NULL, 0, threadNormal, NULL, 0, &dwThreadId);
//    ::SetThreadPriority(h[1], THREAD_PRIORITY_NORMAL);
//    ::ResumeThread(h[1]);

    DWORD dwRet;
    int iCount = 0;
    while (iCount != 2)
    {
        dwRet = ::WaitForMultipleObjects(2, h, FALSE, 1);
        switch (dwRet)
        {
        case WAIT_FAILED:
            printf("WaitForMultipleObjects failed!\n");
            break;
        case WAIT_TIMEOUT:
            printf("Time out!\n");
            iCount = 2;
            break;
        case WAIT_OBJECT_0 + 0:
            printf("Idle signaled\n");
            break;
        case WAIT_OBJECT_0 + 1:
            printf("Normal signaled\n");
            break;
        }
    }

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
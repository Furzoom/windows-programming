#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    TCHAR szCommandLine[] = "cmd";
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = TRUE;

    BOOL bRet = ::CreateProcess(
        NULL,
        szCommandLine,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi);
    if (bRet)
    {
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);
        printf("Process ID: %d\n", pi.dwProcessId);
        printf("Thread ID: %d\n", pi.dwThreadId);
    }
    getchar();
    return 0;
}
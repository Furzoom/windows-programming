#include <windows.h>
#include <stdio.h>


static BOOL TerminateProcessFromId(DWORD dwId);
static BOOL CreateProcessByCmdLine(TCHAR *cmd, LPDWORD pdwId);

int main(int argc, char* argv[])
{
    TCHAR szProcessName[] = TEXT("notepad");
    DWORD dwProcessId;
    if (CreateProcessByCmdLine(szProcessName, &dwProcessId))
    {
        puts("Will close it.(Press ENTER to continue)");
        getchar();
        if (TerminateProcessFromId(dwProcessId))
        {
            puts("Terminate OK!");
        }
        else
        {
            puts("Terminate Failure!");
        }
    }
    else
    {
        printf("CreateProcessByCmdLine failure!\n");
        exit(1);
    }
    
    getchar();
    return 0;
}

BOOL CreateProcessByCmdLine(TCHAR *cmd, LPDWORD pdwId)
{
    BOOL bRet;
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;

    bRet = ::CreateProcess(NULL, cmd, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
    if (bRet)
    {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        printf("Process id: %u\n", pi.dwProcessId);
        *pdwId = pi.dwProcessId;
    }
    return bRet;
}

BOOL TerminateProcessFromId(DWORD dwId)
{
    BOOL bRet = FALSE;
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
    if (hProcess != NULL)
    {
        bRet = ::TerminateProcess(hProcess, 0);
    }
    CloseHandle(hProcess);
    return bRet;
}

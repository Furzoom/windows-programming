#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>

static BOOL terminateProcessById(DWORD dwPID);
static DWORD getProcessIdByName(LPCTSTR szProcessName);

int main(int argc, char* argv[])
{
    DWORD dwProcessId;

    if (argc != 2)
    {
        printf("Usage: %s process\n", argv[0]);
        return 0;
    }
    if (0 == (dwProcessId = getProcessIdByName(argv[1])))
    {
        printf("Not found %s\n", argv[1]);
        return 0;
    }
    
    if (terminateProcessById(dwProcessId))
    {
        printf("Terminate %s Success\n", argv[1]);
    }
    else 
    {
        printf("Error %ul\n", ::GetLastError());
    }
    return 0;
}

BOOL terminateProcessById(DWORD dwPID)
{
    BOOL bSuccess;
    HANDLE hProcess;

    bSuccess = FALSE;
    hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwPID);

    if (hProcess != NULL)
    {
        bSuccess = ::TerminateProcess(hProcess, 0);
        ::CloseHandle(hProcess);
    }
    return bSuccess;
}

DWORD getProcessIdByName(LPCTSTR szProcessName)
{
    PROCESSENTRY32 pe32;
    DWORD dwProcessId;

    pe32.dwSize = sizeof(pe32);
    dwProcessId = 0;

    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot Failed!\n");
        exit(1);
    }
    
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    while (bMore)
    {
        if (strcmp(pe32.szExeFile, szProcessName) == 0)
        {
            dwProcessId = pe32.th32ProcessID;
            break;
        }
        bMore = ::Process32Next(hProcessSnap, &pe32);
    }
    CloseHandle(hProcessSnap);

    return dwProcessId;
}


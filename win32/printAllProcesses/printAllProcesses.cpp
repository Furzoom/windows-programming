#include <windows.h>
#include <TlHelp32.h> // for snapshot
#include <stdio.h>

int main(int argc, char* argv[])
{
    PROCESSENTRY32 pe32;
    int iProcessCount;

    // initial its size
    pe32.dwSize = sizeof(pe32);
    iProcessCount = 0;

    // create snapshot for all processes
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot failuer!\n");
        exit(1);
    }

    // traversal snapshot
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    while (bMore) 
    {
        printf("%-20.20s%6u%4u%6u%6u%6u%3ld\n", pe32.szExeFile, 
            pe32.th32ProcessID, pe32.cntUsage, pe32.th32DefaultHeapID, 
            pe32.th32ModuleID, pe32.th32ParentProcessID, pe32.pcPriClassBase);
        bMore = ::Process32Next(hProcessSnap, &pe32);
        iProcessCount ++;
    }
    
    // clear snapshot
    ::CloseHandle(hProcessSnap);

    printf("Processes count: %d\n\n", iProcessCount);

    getchar();
    return 0;
}
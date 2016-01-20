#include <windows.h>
#include <stdio.h>

#define PAGE 4096
#define MAX_ADDR_COUNT 1024

HANDLE g_hProcess;
int g_nListCnt;
DWORD g_arList[MAX_ADDR_COUNT];

static BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);
static BOOL FindFirst(DWORD dwValue);
static BOOL FindNext(DWORD dwValue);
static BOOL WriteMemory(DWORD dwAddr, DWORD dwValue);

static void ShowList();

int main(int argc, char* argv[])
{
    TCHAR szFileName[] = TEXT("memTest.exe");
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    BOOL bRet = ::CreateProcess(NULL, szFileName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!bRet)
    {
        fprintf(stderr, "CreateProcess Failure!\n");
        exit(1);
    }
    ::CloseHandle(pi.hThread);
    g_hProcess = pi.hProcess;

    int iVal;
    printf("Input Val = ");
    scanf("%d", &iVal);
    getchar(); // for \n
    FindFirst(iVal);
    ShowList();

    while (g_nListCnt > 1)
    {
        printf("Input Val = ");
        scanf("%d", &iVal);
        FindNext(iVal);
        ShowList();
        getchar(); // for \n
    }

    printf("New Value = ");
    scanf("%d", &iVal);
    getchar(); // for \n
    if (WriteMemory(g_arList[0], iVal))
    {
        printf("Write data success\n");
    }

    printf("Will terminate %s.(Press ENTER to continue)\n", szFileName);
    getchar(); // wait

    if (::TerminateProcess(pi.hProcess, 0))
    {
        printf("Terminate %s success!\n", szFileName);
    }
    ::CloseHandle(pi.hProcess);
    getchar(); // wait
    return 0;
}

BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
    BYTE arBytes[PAGE];
    if (!::ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, PAGE, NULL))
    {
        return FALSE;
    }
    LPDWORD pdw;
    for (int i = 0; i < PAGE - 3; i++)
    {
        pdw = (LPDWORD)&arBytes[i];
        if (*pdw == dwValue)
        {
            if (g_nListCnt >= MAX_ADDR_COUNT)
            {
                return FALSE;
            }
            g_arList[g_nListCnt++] = dwBaseAddr + i;
        }
    }
    return TRUE;
}

BOOL FindFirst(DWORD dwValue)
{
    const DWORD dwOneGB = 1024 * 1024 * 1024;   // 1GB
    const DWORD dwOnePage = PAGE;               // 4KB
    if (g_hProcess == NULL)
    {
        return FALSE;
    }
    
    DWORD dwBase;
    OSVERSIONINFO vi = {sizeof(vi)};
    ::GetVersionEx(&vi);
    if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        dwBase = 4 * 1024 * 1024;   // 4MB Windows 98
    }
    else
    {
        dwBase = 640 * 1024;        // 64KB Windows NT
    }
   
    while (dwBase < 2 * dwOneGB)
    {
        CompareAPage(dwBase, dwValue);
        dwBase += dwOnePage;
    }
    return TRUE;
}

void ShowList()
{
    for (int i = 0; i < g_nListCnt; i++)
    {
        printf("%08lX\n", g_arList[i]);
    }
}

BOOL FindNext(DWORD dwValue)
{
    int nOldCnt = g_nListCnt;
    g_nListCnt = 0;

    BOOL bRet = FALSE;
    DWORD dwReadValue;
    for (int i = 0; i < nOldCnt; i++)
    {
        if (::ReadProcessMemory(g_hProcess, (LPVOID)g_arList[i], &dwReadValue, sizeof(DWORD), NULL))
        {
            if (dwReadValue == dwValue)
            {
                g_arList[g_nListCnt++] = g_arList[i];
                bRet = TRUE;
            }
        }
    }
    return bRet;
}

BOOL WriteMemory(DWORD dwAddr, DWORD dwValue)
{
    return ::WriteProcessMemory(g_hProcess, (LPVOID)dwAddr, &dwValue, sizeof(dwValue), NULL);
}

#include <windows.h>
#include "resource.h"
#include <tchar.h>
#include <stdio.h>

#define MAX_SIZE    1024

const char g_szClassName[] = "Control";

static void logMsg(LPTSTR szFormat, ...);

LRESULT CALLBACK ShowDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_ADD:
        {
            TCHAR *buffer, *input;
            BOOL bSuccess;
            int nTimes;
            int i;
            nTimes = GetDlgItemInt(hwnd, IDE_TIMES, &bSuccess, TRUE);
            logMsg("nTimes = %d\t bSuccess = %d\n", nTimes, bSuccess);
            if (bSuccess && nTimes > 0)
            {
                int lenOutput = GetWindowTextLength(GetDlgItem(hwnd, IDE_OUTPUT));
                int lenInput = GetWindowTextLength(GetDlgItem(hwnd, IDE_INPUT));
                int length = lenInput * nTimes + lenOutput;
                if (length > 0)
                {
                    buffer = (TCHAR *)calloc(length + nTimes * 2 + 1, sizeof(TCHAR));
                    input = (TCHAR *)calloc(lenInput + 1, sizeof(TCHAR));
                    GetDlgItemText(hwnd, IDE_OUTPUT, buffer, lenOutput + 1);
                    GetDlgItemText(hwnd, IDE_INPUT, input, lenInput + 1);
                    for (i = 0; i < nTimes; i++)
                    {
                        _sntprintf_s(buffer + lenOutput + (lenInput + 2) * i, lenInput + 2, lenInput + 2, TEXT("\r\n%s"), input);
                    }
                    SetDlgItemText(hwnd, IDE_OUTPUT, buffer);
                    logMsg("input: %s\n", input);
                    logMsg("output: %s\n", buffer);
                }
            }
        }
            break;
        case ID_REMOVE:
            break;
        case ID_CLEAR:
            SetDlgItemText(hwnd, IDE_OUTPUT, "");
            break;
        case IDOK:

            break;
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;
        default:
            break;
        }
    default:
        return FALSE;
    }
    return TRUE;
}
// Step 4: the window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case ID_DIALOG_SHOW:
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SHOW), hwnd, ShowDlgProc);
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    //    WNDCLASS wc;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    // step 1: registering the window class
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = 0;
    wc.lpfnWndProc      = WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName     = MAKEINTRESOURCE(IDR_MENU);
    wc.lpszClassName    = g_szClassName;
    wc.hIconSm          = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
        //    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Create window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Control",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void logMsg(LPTSTR szFormat, ...)
{
    static TCHAR szBuffer[1024];
    va_list pArgs;
    va_start(pArgs, szFormat);
    _vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), sizeof(szBuffer) / sizeof(TCHAR), szFormat, pArgs);
    va_end(pArgs);
    OutputDebugString(szBuffer);
}

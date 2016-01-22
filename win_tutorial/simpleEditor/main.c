#include <windows.h>
#include "resource.h"
#include <CommCtrl.h>

const char g_szClassName[] = "simpleEditor";

static BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName);
static BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName);

// Step 4: the window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        {
            HFONT hfDefault;
            HWND hEdit, hTool, hStatus;
            TBBUTTON tbb[3];
            TBADDBITMAP tbab;
            int statWidths[] = {100, -1};

            // toolbar
            hTool = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_TOOL, GetModuleHandle(NULL), NULL);
            if (hTool == NULL)
            {
                MessageBox(hwnd, "Could not create Toolbar!", "Error", MB_OK | MB_ICONSTOP);
            }
            SendMessage(hTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

            tbab.hInst = HINST_COMMCTRL;
            tbab.nID = IDB_STD_SMALL_COLOR;
            SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)&tbab);
            memset(tbb, 0, sizeof(tbb));
            tbb[0].iBitmap = STD_FILENEW;
            tbb[0].fsState = TBSTATE_ENABLED;
            tbb[0].fsStyle = TBSTYLE_BUTTON;
            tbb[0].idCommand = ID_FILE_NEW;

            tbb[1].iBitmap = STD_FILEOPEN;
            tbb[1].fsState = TBSTATE_ENABLED;
            tbb[1].fsStyle = TBSTYLE_BUTTON;
            tbb[1].idCommand = ID_FILE_OPEN;

            tbb[2].iBitmap = STD_FILESAVE;
            tbb[2].fsState = TBSTATE_ENABLED;
            tbb[2].fsStyle = TBSTYLE_BUTTON;
            tbb[2].idCommand = ID_FILE_SAVE;

            SendMessage(hTool, TB_ADDBUTTONS, sizeof(tbb)/sizeof(TBBUTTON), (LPARAM)&tbb);

            // status
            hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);
            SendMessage(hStatus, SB_SETPARTS, sizeof(statWidths) / sizeof (int), (LPARAM)statWidths);
            SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"Hi there :)");
            SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)"Hi here ^_^");

            // main Edit
            hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE 
                | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                0, 0, 100, 100, 
                hwnd, 
                (HMENU)IDC_MAIN_EDIT, 
                GetModuleHandle(NULL), 
                NULL);
            if (hEdit == NULL)
            {
                MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONSTOP);
            }
            hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
        }
        break;
    case WM_SIZE:
        {
            HWND hTool;
            RECT rcTool;
            int iToolHeight;

            HWND hStatus;
            RECT rcStatus;
            int iStatusHeight;

            HWND hEdit;
            int iEditHeight;
            RECT rcClient;
            
            // size toolbar
            hTool = GetDlgItem(hwnd, IDC_MAIN_TOOL);
            SendMessage(hTool, TB_AUTOSIZE, 0, 0);
            GetWindowRect(hTool, &rcTool);
            iToolHeight = rcTool.bottom - rcTool.top;

            // size status
            hStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
            SendMessage(hStatus, WM_SIZE, 0, 0);
            GetWindowRect(hStatus, &rcStatus);
            iStatusHeight = rcStatus.bottom - rcStatus.top;

            // Calculate remaining height and size edit
            GetClientRect(hwnd, &rcClient);
            iEditHeight = rcClient.bottom - iToolHeight - iStatusHeight;
            hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
            SetWindowPos(hEdit, NULL, 0, iToolHeight, rcClient.right, iEditHeight, SWP_NOZORDER);
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) 
        {
        case ID_FILE_OPEN:
            {
                OPENFILENAME ofn;
                char szFileName[MAX_PATH] = "";
                memset(&ofn, 0, sizeof(ofn));

                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                ofn.lpstrDefExt = "txt";

                if (GetOpenFileName(&ofn))
                {
                    LoadTextFileToEdit(GetDlgItem(hwnd, IDC_MAIN_EDIT), szFileName);
                }
            }
            break;
        case ID_FILE_SAVE:
            {
                OPENFILENAME ofn;
                char szFileName[MAX_PATH] = "";
                memset(&ofn, 0, sizeof(ofn));

                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
                ofn.lpstrDefExt = "txt";

                if (GetSaveFileName(&ofn))
                {
                    SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_MAIN_EDIT), szFileName);
                }
            }
            break;
        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        default:
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
        "Simple Editor",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;

    hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwFileSize;
        dwFileSize = GetFileSize(hFile, NULL);
        if (dwFileSize != 0xFFFFFFFF)
        {
            LPSTR pszFileText;
            pszFileText = GlobalAlloc(GPTR, dwFileSize + 1);
            if (pszFileText != NULL)
            {
                DWORD dwRead;
                if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
                {
                    pszFileText[dwFileSize] = 0;
                    if (SetWindowText(hEdit, pszFileText))
                        bSuccess = TRUE;
                }
                GlobalFree(pszFileText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
    HANDLE hFile;
    BOOL bSuccess = FALSE;
    hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwTextLength;
        dwTextLength = GetWindowTextLength(hEdit);
        if (dwTextLength > 0)
        {
            LPSTR pszText;
            DWORD dwBufferSize = dwTextLength + 1;
            pszText = GlobalAlloc(GPTR, dwBufferSize);
            if (pszText != NULL)
            {
                if (GetWindowText(hEdit, pszText, dwBufferSize))
                {
                    DWORD dwWritten;
                    if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
                        bSuccess = TRUE;
                }
                GlobalFree(pszText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}

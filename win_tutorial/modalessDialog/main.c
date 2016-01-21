#include <windows.h>
#include "resource.h"

const char g_szClassName[] = "modalessdialog";
HWND g_hToolbar = NULL;

LRESULT CALLBACK ToolDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_PRESS:
            MessageBox(hwnd, "Hi!", "This is a message", MB_OK | MB_ICONEXCLAMATION);
            break;
        case IDC_OTHER:
            MessageBox(hwnd, "Bye!", "This is also a message", MB_OK | MB_ICONEXCLAMATION);
            break;
        }
        break;
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
    case WM_CREATE:
        g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR), hwnd, ToolDlgProc);
        if (g_hToolbar != NULL)
        {
            ShowWindow(g_hToolbar, SW_SHOW);
        }
        else
        {
            MessageBox(hwnd, "CreateDialog returned NULL", "Warning!", MB_OK | MB_ICONINFORMATION);
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case ID_DIALOG_SHOW:
            ShowWindow(g_hToolbar, SW_SHOW);
            break;
        case ID_DIALOG_HIDE:
            ShowWindow(g_hToolbar, SW_HIDE);
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        DestroyWindow(g_hToolbar);
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
        "Modaless Dialog",
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
        if (!IsDialogMessage(g_hToolbar, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

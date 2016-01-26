#include <windows.h>
#include "resource.h"

const char g_szClassName[] = "Bitmap";
HBITMAP g_hbmpTank = NULL;

// Step 4: the window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        g_hbmpTank = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_TANK));
        if (g_hbmpTank == NULL)
        {
            MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONERROR);
        }
        break;
    case WM_PAINT:
        {
            BITMAP bm;
            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(hwnd, &ps);
            HDC hdcMem = CreateCompatibleDC(hdc);

            HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, g_hbmpTank);

            GetObject(g_hbmpTank, sizeof(bm), &bm);
            BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
            BitBlt(hdc, 0, bm.bmHeight, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND);
            BitBlt(hdc, 0, bm.bmHeight * 2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT);

            SelectObject(hdcMem, hbmpOld);
            DeleteDC(hdcMem);
            
            EndPaint(hwnd, &ps);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        DeleteObject(g_hbmpTank);
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
    wc.hbrBackground    = (HBRUSH)(GetStockObject(GRAY_BRUSH));
    wc.lpszMenuName     = NULL;
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
        "Bitmap",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 340, 220,
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

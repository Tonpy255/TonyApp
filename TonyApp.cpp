#include <Windows.h>
#include "resource.h"
#define IDC_BUTTON_TEST 1001

static HFONT g_hFont = NULL;

LRESULT CALLBACK MainWndProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
    switch (uMsg) {
        case WM_CREATE: { 
            LOGFONTA lf = {0};
            lf.lfHeight = -32;
            lf.lfWeight = FW_BOLD;
            lf.lfCharSet = DEFAULT_CHARSET;
            lstrcpyA(lf.lfFaceName, "Arial");
            g_hFont = CreateFontIndirectA(&lf);

            CreateWindowA(
                "BUTTON",
                "Press Me",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                200, 350,
                200, 50,
                hwnd,
                (HMENU)IDC_BUTTON_TEST,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL
            );
            return 0;
        }
 
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HFONT hOldFont = (HFONT)SelectObject(hdc, g_hFont);
            SetTextColor(hdc, RGB(0, 0, 200));
            SetBkMode(hdc, TRANSPARENT);

            RECT rect;
            GetClientRect(hwnd, &rect);

            DrawTextA(hdc, "TonySystem 0.2", -1, &rect,
                      DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(hdc, hOldFont);
            EndPaint(hwnd, &ps);
            return 0;
        } 
 
        case WM_SIZE: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
 
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_BUTTON_TEST) {
                MessageBoxA(hwnd, "Clicked", "tips", MB_OK);
            }
            return 0;
        }

        case WM_DESTROY: {
            if (g_hFont) {
                DeleteObject(g_hFont);
                g_hFont = NULL;
            }
            PostQuitMessage(0);
            return 0;
        }

        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSEXA wnd = {0};

    wnd.cbSize        = sizeof(WNDCLASSEXA);
    wnd.style         = CS_HREDRAW | CS_DBLCLKS | CS_VREDRAW;
    wnd.lpfnWndProc   = MainWndProc;
    wnd.cbClsExtra    = 0;
    wnd.cbWndExtra    = 0;
    wnd.hInstance     = hInstance;
    wnd.hCursor       = LoadCursorA(NULL, IDC_ARROW);
    wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wnd.lpszClassName = "TonySystem";

    HICON hIcon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    wnd.hIcon     = hIcon;
    wnd.hIconSm   = hIcon;

    if (!RegisterClassExA(&wnd)) {
        MessageBoxA(NULL, "Register Failed", "Error", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowA("TonySystem", "TonySystem0.2", WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 600, 450,
                              NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBoxA(NULL, "Create Failed", "Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

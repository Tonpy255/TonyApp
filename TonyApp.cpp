#include <Windows.h>
#include "resource.h"
#define FIRST_BUTTON 1
#define SECOND_BUTTON 2

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
                "First Button",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                200, 50,
                200, 50,
                hwnd,
                (HMENU)FIRST_BUTTON,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL
            );
            CreateWindowA(
                "BUTTON",
                "Second Button",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                200, 350,
                200, 50,
                hwnd,
                (HMENU)SECOND_BUTTON,
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

            DrawTextA(hdc, "TonyApp 0.3.1", -1, &rect,
                      DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SelectObject(hdc, hOldFont);
            EndPaint(hwnd, &ps);
            return 0;
        } 
 
        case WM_SIZE: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
 
        case WM_COMMAND: {
        	WORD what = LOWORD(wParam);
        	int a = 0;
            if (what == FIRST_BUTTON) {
                a = MessageBoxA(hwnd, "First Clicked", "tips", MB_YESNO);
            } else if (what == SECOND_BUTTON) {
            	a = MessageBoxA(hwnd, "Second Clicked", "tips", MB_YESNO);
			}
			if(a){
				if (a == IDYES) {
					MessageBoxA(hwnd, "Yes!", "tips", MB_OK);
				} else {
					MessageBoxA(hwnd, "No!", "tips", MB_OK);
				}
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
    wnd.lpszClassName = "TonyApp";

    HICON hIcon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    wnd.hIcon     = hIcon;
    wnd.hIconSm   = hIcon;

    if (!RegisterClassExA(&wnd)) {
        MessageBoxA(NULL, "Register Failed", "Error", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowA("TonyApp", "TonyApp", WS_OVERLAPPEDWINDOW,
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

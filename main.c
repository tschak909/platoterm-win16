#include <windows.h>
#include <stdbool.h>
#include <winsock.h>
#include "screen.h"
#include "io.h"
#include "keyboard.h"
#include "terminal.h"
#include "touch.h"

#define WM_SOCKET WM_USER + 1

unsigned int already_started=0;
bool doing_io=false;

extern COLORREF background;
int replaylen, buflen;
int ySize;

padByte buffer[16384];
padByte replay_buffer[16384];

long FAR PASCAL _export WndProc (HWND, UINT, UINT, LONG);

 int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
 {
     static char szAppName[] = "PLATOTERM";
     HWND hwnd;
     MSG msg;
     WNDCLASS wndclass;

     if (!hPrevInstance)
     {
         wndclass.style = CS_HREDRAW | CS_VREDRAW;
         wndclass.lpfnWndProc = WndProc;
         wndclass.cbClsExtra = 0;
         wndclass.cbWndExtra = 0;
         wndclass.hInstance = hInstance;
         wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
         wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
         wndclass.hbrBackground = GetStockObject(BLACK_BRUSH);
         wndclass.lpszMenuName = NULL;
         wndclass.lpszClassName = szAppName;

         RegisterClass(&wndclass);
     }

    ySize = GetSystemMetrics(SM_CYSCREEN);

    if (ySize < 512)
    {
        ySize -=(GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2) - 32;
        hwnd = CreateWindow(szAppName, "PLATO Terminal", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, 128, 0, 512, ySize, NULL, NULL, hInstance, NULL);
    }
    else
    {
        hwnd = CreateWindow(szAppName, "PLATO Terminal", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 512, 512 + (GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2), NULL, NULL, hInstance, NULL);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    terminal_init();
    WSAAsyncSelect(io_init(),hwnd,WM_SOCKET,FD_READ);

    while (TRUE)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
 }

 long FAR PASCAL _export WndProc(HWND hwnd, UINT message, UINT wParam, LONG lParam)
 {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (message)
    {
        case WM_SOCKET:
            switch(WSAGETSELECTEVENT(lParam))
            {
                case FD_READ:
                    if (io_main()>0)
                        {
                            rect.top = rect.left = 0;
                            rect.bottom = rect.right = 512;
                            InvalidateRect(hwnd,&rect,false);
                        }
                    break;
            }
            break;
        case WM_LBUTTONDOWN:
            touch_main(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd,&ps);
            screen_set_hdc(hdc);
            if (already_started == 0)
            {
                screen_init(hwnd);
                already_started = 1;
            }
            if (buflen > 0)
            {
                ShowPLATO(buffer,buflen);
                buflen=0;
            }
            else if (replaylen > 0)
            {
                ShowPLATO(replay_buffer,replaylen);
            }
            EndPaint(hwnd,&ps);
            break;
        case WM_KEYDOWN:
            return keyboard_main(wParam);
        case WM_KEYUP:
            return keyboard_main_up(wParam);
        case WM_DESTROY:
            io_done();
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd,message,wParam,lParam);
 }

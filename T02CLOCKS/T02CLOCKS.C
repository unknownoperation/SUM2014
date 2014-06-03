/* Korneev Oleg 03.06.2014 */

#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "my window class"
#define TimerId 30
#define PI 3.14159265358979323846

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* Main programm function 
*  Arguments:
*    -handle of instance:
*       HINSTANCE hInstance;
*    -handle previous instance:
*       HINSTANCE hPrevInstance;
*    -command line:
*       CHAR *CmdLine;
*    -show window flag:
*       INT ShowCmd;
*  returns:
*    (INT)
*/
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.hIcon = LoadIcon(NULL, IDC_HAND);
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "ERROR REGISTER CLASS", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "WINDOW", WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_MAXIMIZE);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
} /* End of 'WinMain' function */

/* Window process function 
*  Arguments:
*    - window handle:
*        HWND hWnd;
*    - message number:
*        UINT Msg;
*    - word parameter:
*        WPARAM wParam;
*    - long parameter:
*        LPARAM lParam;
*  returns:
*    (LRESULT)
*/
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i;
  static HBITMAP hBm, hBufBm;
  static BITMAP Bm;
  static INT W, H;
  HDC hMemDC, hBufDC;
  static HDC hDC;
  static SYSTEMTIME tm;
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt;
  double sec, min, hour;
  switch(Msg)
  {
  case WM_CHAR:
    if ((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, TimerId, 10, NULL);
    hBm = LoadImage(NULL, "clocks.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBm, sizeof(BITMAP), &Bm);     
    GetLocalTime(&tm);
    hBufDC = GetDC(hWnd);
    hDC = CreateCompatibleDC(hBufDC);
    ReleaseDC(hWnd, hBufDC);
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    if (hBufBm != NULL)
      DeleteObject(hBufBm);
    hBufDC = GetDC(hWnd);
    hBufBm = CreateCompatibleBitmap(hBufDC, W, H);
    ReleaseDC(hWnd, hBufDC);
    SelectObject(hDC, hBufBm);
    return 0;
  case WM_PAINT:
    hBufDC = BeginPaint(hWnd, &ps);
    //GetClientRect(hWnd, &rc);   
    //Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);
    StretchBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, Bm.bmWidth, Bm.bmHeight, SRCCOPY);
    
    sec = tm.wSecond / 30.0 * PI; 
    min = tm.wMinute / 30.0 * PI;
    hour = (tm.wHour % 12) / 6.0 * PI;
    for (i = -100; i++; i < 100)
    {
      MoveToEx(hDC, W / 2, H / 2 + i, NULL);
      LineTo(hDC, W / 2.0  * sin(sec) + W / 2.0, H / 2.0 * -1.0 * cos(sec) + H / 2.0);
      MoveToEx(hDC, W / 2, H / 2 + i, NULL);
      LineTo(hDC, W / 2.0  * sin(min) + W / 1.8, H / 2.0 * -1.0 * cos(min) + H / 1.8);
      MoveToEx(hDC, W / 2, H / 2 + i, NULL);
      LineTo(hDC, W / 2.0  * sin(hour) + W / 2.5, H / 2.0 * -1.0 * cos(hour) + H / 2.5);
    }  
    BitBlt(hBufDC, 0, 0, W, H, hDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    GetLocalTime(&tm);
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, TimerId);
    DeleteObject(hBm);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* End of 'T02CLOCKS' file */

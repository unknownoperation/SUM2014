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

/* Drawing hand function 
*  Arguments:
*    - DC handle:
*        HDC hDC;
*    - coordinates of center:
*        INT Xc, Yc;
*    - width and height of hand:
*        INT L, W;
*    - drawing angle:
*        DOUBLE Angle;
*  returns:
*    (VOID)
*/
VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, DOUBLE Angle )
{
  INT i;
  POINT pts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pts_draw[sizeof pts / sizeof pts[0]];
  DOUBLE si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pts / sizeof pts[0]; i++)
  {
    pts_draw[i].x = Xc + (pts[i].x * co - pts[i].y * si);
    pts_draw[i].y = Yc - (pts[i].x * si + pts[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
} /* End of 'DrawArrow' function */

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
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);
    StretchBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, Bm.bmWidth, Bm.bmHeight, SRCCOPY);
    /*
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
    */
    DrawArrow(hDC, W / 2, H / 2, H / 3.5, W / 20, (-(tm.wHour % 12 + tm.wMinute / 60.0) / 12.0) * 2 * PI);
    DrawArrow(hDC, W / 2, H / 2, H / 2.5, W / 25, (-(tm.wMinute + tm.wSecond / 60.0) / 60.0) * 2 * PI);
    DrawArrow(hDC, W / 2, H / 2, H / 2.1, W / 30, (-(tm.wSecond + tm.wMilliseconds / 1000.0) / 60.0) * 2 * PI);
    DrawArrow(hDC, W / 2, H / 2, H / 2, W / 32, (-tm.wMilliseconds / 1000.0) * 2 * PI);

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

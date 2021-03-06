/* Korneev Oleg 05.06.2014 */

#include <math.h>
#include <windows.h>

#include "def.h"
#include "image.h"
#include "vec.h"

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
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
} /* End of 'DrawArrow' function */

/* Flip fullscreen mode function.
 * Arguments: none.
 * Returns: (VOID).
 */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE; 
  static RECT SaveRC;               

  if (!IsFullScreen)
  {
    RECT rc;

    GetWindowRect(hWnd, &SaveRC);

    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'FlipFullScreen' function */

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
  static VEC A, B;
  static MATR C = {{
    {1, 0, 0, 10},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
}, D;
  static HBITMAP hBm, hBufBm;
  static BITMAP Bm;
  static INT W, H;
  HDC hMemDC, hBufDC;
  static HDC hDC;
  static SYSTEMTIME tm;
  PAINTSTRUCT ps;
  static double Fi, Ksi, Rad, Alpha = 0.0, Rr = 0.0;

  switch(Msg)
  {
  case WM_CHAR:
    if ((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    if ((CHAR)wParam == 'f')
      FlipFullScreen(hWnd);
    return 0;
  case WM_CREATE:
    A.X = 1.0;
    A.Y = 30.0;
    A.Z = 700.0;
    Rad = VecLen(A);
    Rr = VecLen(VecMulNum(A, 2));
    
    D = MatrTranspose(C);

    D = MatrRotateY(60);
    SetTimer(hWnd, TimerId, 10, NULL);
    hBm = LoadImage(NULL, "clocks.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBm, sizeof(BITMAP), &Bm);     
    GetLocalTime(&tm);
    hBufDC = GetDC(hWnd);
    hDC = CreateCompatibleDC(hBufDC);
    ReleaseDC(hWnd, hBufDC);
    Rad = 500.0;
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

    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 0, 0));
   
    Rectangle(hDC, 0, 0, W, H);

    for (Fi = Alpha; Fi < PI * 2.0 + Alpha; Fi += 0.009)
      for (Ksi = Alpha; Ksi < PI + Alpha; Ksi += 0.009)                            
        if (Rad * sin(Ksi) * cos(Fi) > 0)
          SetPixel(hDC, W / 2 + Rad * sin(Ksi) * sin(Fi), H / 2 + Rad * cos(Ksi), RGB(fabs(cos(Ksi)) * 200, fabs(cos(Fi)) * 200 + 50, fabs(cos(Rad)) * 255));
    Alpha += 0.01;
    Rr += 0.1;
    Rad += sin(Rr) * 10;
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

/* Korneev Oleg 02.06.2014 */

#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "my window class"
#define TimerId 30

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

void Eyes( HDC hDC, INT Xc, INT Yc, INT W, INT H )
{
  INT x, y, xe, ye, r = W / 16;
  DOUBLE si, co, len;
  
  
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, 0, 0, W / 2, H);
  Ellipse(hDC, W / 2, 0, W, H);

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(255, 255, 0));
  len = sqrt((Xc - W / 4) * (Xc - W / 4) + (Yc - H / 2) * (Yc - H / 2));
  if (len == 0.0)
    si = 0, co = 0;
  x = W / 4;
  y = H / 2;

  co = (Xc - x) / len;
  si = (Yc - y) / len;

  if (len > W / 4 - r)
    len = W / 4 - r;

  xe = x + co * len;
  ye = y + si * len;

  Ellipse(hDC, xe - r, ye - r, xe + r, ye + r);
  
  len = sqrt((Xc - W * 3 / 4) * (Xc - W * 3 / 4) + (Yc - H / 2) * (Yc - H / 2));
  if (len == 0.0)
    si = 0, co = 0;
  x = W * 3 / 4;
  y = H / 2;

  co = (Xc - x) / len;
  si = (Yc - y) / len;

  if (len > W / 4 - r)
    len = W / 4 - r;

  xe = x + co * len;
  ye = y + si * len;

  Ellipse(hDC, xe - r, ye - r, xe + r, ye + r);
 

}

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
  HDC hDC;
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt;
  switch(Msg)
  {
  case WM_CHAR:
    if ((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, TimerId, 10, NULL);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rc);
    
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    GetClientRect(hWnd, &rc);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    hDC = GetDC(hWnd);
    Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    Eyes(hDC, pt.x, pt.y, rc.right, rc.bottom);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, TimerId);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* End of 'T01FWIN' file */

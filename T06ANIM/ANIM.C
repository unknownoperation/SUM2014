/* FILENAME: ANIM.C
 * PROGRAMMER: OK2
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* ��������� �������� �������� */
static ok2ANIM OK2_Anim;

/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ������ ��� ��������� ���� */
static INT
  OK2_MouseGlobalX, OK2_MouseGlobalY, /* ������� ���������� */
  OK2_MouseXOld, OK2_MouseYOld,       /* ����������� �� �������� ����� ���������� */
  OK2_MouseGlobalWheel;               /* ��������� ������ ���� */

/* ������� ��������� ����������� �������� ���������.
 * ���������:
 *   - ��� �������:
 *       INT Code;
 *   - �������� ��������� ('word parameter') -
 *     ��� ��������� �� ����:
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter') -
 *     (MSLLHOOKSTRUCT *) ��� �������� ���������;
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
static LRESULT CALLBACK OK2_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    OK2_MouseGlobalX = hs->pt.x;
    OK2_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    OK2_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'OK2_MouseHook' function */

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID OK2_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  OK2_Anim.hWnd = hWnd;
  OK2_Anim.hDC = CreateCompatibleDC(hDC);
  OK2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  OK2_Anim.W = 30;
  OK2_Anim.H = 30;
  OK2_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* �������������� ������ */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
  
  /* �������������� ������ ��������� �� ���� */
  SetWindowsHookEx(WH_MOUSE_LL, OK2_MouseHook, GetModuleHandle(NULL), 0);
} /* End of 'OK2_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID OK2_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
  {
    OK2_Anim.Units[i]->Close(OK2_Anim.Units[i], &OK2_Anim);
    free(OK2_Anim.Units[i]);
  }
  DeleteObject(OK2_Anim.hBmFrame);
  DeleteDC(OK2_Anim.hDC);
  memset(&OK2_Anim, 0, sizeof(ok2ANIM));
} /* End of 'OK2_AnimInit' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID OK2_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(OK2_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(OK2_Anim.hBmFrame);
  OK2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(OK2_Anim.hDC, OK2_Anim.hBmFrame);

  /* ���������� ������� */
  OK2_Anim.W = W;
  OK2_Anim.H = H;

  ReleaseDC(OK2_Anim.hWnd, hDC);
} /* End of 'OK2_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID OK2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  /* ���������� ����� */
  GetKeyboardState(OK2_Anim.Keys);
  for (i = 0; i < 256; i++)
    OK2_Anim.KeyClick[i] = OK2_Anim.Keys[i] && !OK2_Anim.KeysOld[i];
  memcpy(OK2_Anim.KeysOld, OK2_Anim.Keys, sizeof(OK2_Anim.Keys));


  /* ���������� ������� */
  OK2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* ���������� ����� */
  QueryPerformanceCounter(&li);

  /* ���������� ����� */
  OK2_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  OK2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* ��������� ����� */
  if (OK2_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    OK2_Anim.DeltaTime = 0;
  }
  else
    OK2_Anim.DeltaTime = OK2_Anim.GlobalDeltaTime;

  OK2_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    OK2_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  /* ������� ���� */
  SelectObject(OK2_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(OK2_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(OK2_Anim.hDC, RGB(0, 110, 0));
  Rectangle(OK2_Anim.hDC, 0, 0, OK2_Anim.W, OK2_Anim.H);

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
    OK2_Anim.Units[i]->Response(OK2_Anim.Units[i], &OK2_Anim);

  /* ��������� �������� */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
  {
    SelectObject(OK2_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(OK2_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(OK2_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(OK2_Anim.hDC, RGB(55, 155, 255));
    OK2_Anim.Units[i]->Render(OK2_Anim.Units[i], &OK2_Anim);
  }

  /* ���� */
  /*  ������ */
  OK2_Anim.MsWheel = OK2_MouseGlobalWheel;
  OK2_MouseGlobalWheel = 0;
  /* ���������� ������� */
  pt.x = OK2_MouseGlobalX;
  pt.y = OK2_MouseGlobalY;
  ScreenToClient(OK2_Anim.hWnd, &pt);
  OK2_Anim.MsX = pt.x;
  OK2_Anim.MsY = pt.y;
  /* ������������� ����������� */
  OK2_Anim.MsDeltaX = OK2_MouseGlobalX - OK2_MouseXOld;
  OK2_Anim.MsDeltaY = OK2_MouseGlobalY - OK2_MouseYOld;
  OK2_MouseXOld = OK2_MouseGlobalX;
  OK2_MouseYOld = OK2_MouseGlobalY;

    /* �������� */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* ��������� ����� ���������� � �������� */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ��������� �������� ��������� */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* ������ */
        memcpy(OK2_Anim.JButsOld, OK2_Anim.JButs, sizeof(OK2_Anim.JButs));
        for (i = 0; i < 32; i++)
          OK2_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          OK2_Anim.JButsClick[i] = OK2_Anim.JButs[i] && !OK2_Anim.JButsOld[i];

        /* ��� */
        OK2_Anim.JX = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1;
        OK2_Anim.JY = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASZ)
          OK2_Anim.JZ = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASR)
          OK2_Anim.JR = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASU)
          OK2_Anim.JU = 2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            OK2_Anim.JPOV = 0;
          else
            OK2_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  FrameCounter++;
} /* End of 'OK2_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID OK2_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(OK2_Anim.hWnd);
  BitBlt(hDC, 0, 0, OK2_Anim.W, OK2_Anim.H, OK2_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(OK2_Anim.hWnd, hDC);
} /* End of 'OK2_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ok2UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID OK2_AnimAddUnit( ok2UNIT *Unit )
{
  if (OK2_Anim.NumOfUnits < OK2_MAX_UNITS)
  {
    OK2_Anim.Units[OK2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &OK2_Anim);
  }
} /* End of 'OK2_AnimAddUnit' function */

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID OK2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(OK2_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(OK2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    /* ��� ������ ��������:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(OK2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(OK2_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* ��������������� ������ ���� */
    SetWindowPos(OK2_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'OK2_AnimFlipFullScreen' function */

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID OK2_AnimSetPause( BOOL NewPauseFlag )
{
  OK2_Anim.IsPause = NewPauseFlag;
} /* End of 'OK2_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */

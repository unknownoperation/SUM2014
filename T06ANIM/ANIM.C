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

/* Системный контекст анимации */
static ok2ANIM OK2_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Данные для обработки мыши */
static INT
  OK2_MouseGlobalX, OK2_MouseGlobalY, /* Текущие координаты */
  OK2_MouseXOld, OK2_MouseYOld,       /* Сохраненные от прошлого кадра координаты */
  OK2_MouseGlobalWheel;               /* Состояние колеса мыши */

/* Функция обработки захваченных мышинных сообщений.
 * АРГУМЕНТЫ:
 *   - код захвата:
 *       INT Code;
 *   - параметр сообшения ('word parameter') -
 *     код сообщения от мыши:
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter') -
 *     (MSLLHOOKSTRUCT *) для мышинных сообщений;
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
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

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
  
  /* инициализируем захват сообщений от мыши */
  SetWindowsHookEx(WH_MOUSE_LL, OK2_MouseHook, GetModuleHandle(NULL), 0);
} /* End of 'OK2_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
  {
    OK2_Anim.Units[i]->Close(OK2_Anim.Units[i], &OK2_Anim);
    free(OK2_Anim.Units[i]);
  }
  DeleteObject(OK2_Anim.hBmFrame);
  DeleteDC(OK2_Anim.hDC);
  memset(&OK2_Anim, 0, sizeof(ok2ANIM));
} /* End of 'OK2_AnimInit' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(OK2_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(OK2_Anim.hBmFrame);
  OK2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(OK2_Anim.hDC, OK2_Anim.hBmFrame);

  /* Сохранение размера */
  OK2_Anim.W = W;
  OK2_Anim.H = H;

  ReleaseDC(OK2_Anim.hWnd, hDC);
} /* End of 'OK2_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  /* Обновление ввода */
  GetKeyboardState(OK2_Anim.Keys);
  for (i = 0; i < 256; i++)
    OK2_Anim.KeyClick[i] = OK2_Anim.Keys[i] && !OK2_Anim.KeysOld[i];
  memcpy(OK2_Anim.KeysOld, OK2_Anim.Keys, sizeof(OK2_Anim.Keys));


  /* Обновление таймера */
  OK2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Обновление кадра */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  OK2_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  OK2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (OK2_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    OK2_Anim.DeltaTime = 0;
  }
  else
    OK2_Anim.DeltaTime = OK2_Anim.GlobalDeltaTime;

  OK2_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    OK2_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* очистка фона */
  SelectObject(OK2_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(OK2_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(OK2_Anim.hDC, RGB(0, 110, 0));
  Rectangle(OK2_Anim.hDC, 0, 0, OK2_Anim.W, OK2_Anim.H);

  /* опрос на изменение состояний объектов */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
    OK2_Anim.Units[i]->Response(OK2_Anim.Units[i], &OK2_Anim);

  /* рисование объектов */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
  {
    SelectObject(OK2_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(OK2_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(OK2_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(OK2_Anim.hDC, RGB(55, 155, 255));
    OK2_Anim.Units[i]->Render(OK2_Anim.Units[i], &OK2_Anim);
  }

  /* Мышь */
  /*  колесо */
  OK2_Anim.MsWheel = OK2_MouseGlobalWheel;
  OK2_MouseGlobalWheel = 0;
  /* абсолютная позиция */
  pt.x = OK2_MouseGlobalX;
  pt.y = OK2_MouseGlobalY;
  ScreenToClient(OK2_Anim.hWnd, &pt);
  OK2_Anim.MsX = pt.x;
  OK2_Anim.MsY = pt.y;
  /* относительное перемещение */
  OK2_Anim.MsDeltaX = OK2_MouseGlobalX - OK2_MouseXOld;
  OK2_Anim.MsDeltaY = OK2_MouseGlobalY - OK2_MouseYOld;
  OK2_MouseXOld = OK2_MouseGlobalX;
  OK2_MouseYOld = OK2_MouseGlobalY;

    /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(OK2_Anim.JButsOld, OK2_Anim.JButs, sizeof(OK2_Anim.JButs));
        for (i = 0; i < 32; i++)
          OK2_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          OK2_Anim.JButsClick[i] = OK2_Anim.JButs[i] && !OK2_Anim.JButsOld[i];

        /* Оси */
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

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(OK2_Anim.hWnd);
  BitBlt(hDC, 0, 0, OK2_Anim.W, OK2_Anim.H, OK2_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(OK2_Anim.hWnd, hDC);
} /* End of 'OK2_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ok2UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimAddUnit( ok2UNIT *Unit )
{
  if (OK2_Anim.NumOfUnits < OK2_MAX_UNITS)
  {
    OK2_Anim.Units[OK2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &OK2_Anim);
  }
} /* End of 'OK2_AnimAddUnit' function */

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(OK2_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(OK2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(OK2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(OK2_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(OK2_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'OK2_AnimFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimSetPause( BOOL NewPauseFlag )
{
  OK2_Anim.IsPause = NewPauseFlag;
} /* End of 'OK2_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */

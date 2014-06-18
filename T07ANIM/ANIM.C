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
ok2ANIM OK2_Anim;

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

PIXELFORMATDESCRIPTOR pfd = {0};
UINT OK2_ShaderProg;

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
BOOL OK2_AnimInit( HWND hWnd )
{
  LARGE_INTEGER li;
  INT i;

  /* Init window parametrs */
  OK2_Anim.hDC = GetDC(hWnd);
  OK2_Anim.hWnd = hWnd;
  OK2_Anim.W = 30;
  OK2_Anim.H = 30;
  OK2_Anim.NumOfUnits = 0;
  
  /*** Инициализация OpenGL ***/

  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(OK2_Anim.hDC, &pfd);
  DescribePixelFormat(OK2_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(OK2_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  OK2_Anim.hRC = wglCreateContext(OK2_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(OK2_Anim.hDC, OK2_Anim.hRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(OK2_Anim.hRC);
    ReleaseDC(OK2_Anim.hWnd, OK2_Anim.hDC);
    memset(&OK2_Anim, 0, sizeof(ok2ANIM));
    return FALSE;
  }
  /* параметры OpenGL по-умолчанию */

  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
  
  /* инициализируем захват сообщений от мыши */
  SetWindowsHookEx(WH_MOUSE_LL, OK2_MouseHook, GetModuleHandle(NULL), 0);

  /* Параметры проецирования */
  OK2_Anim.Wp = 4.0, OK2_Anim.Hp = 3.0,     /* размеры обрасти проецирования */
  OK2_Anim.ProjDist = 1.0,              /* расстояние до плоскости проекции */
  OK2_Anim.FarClip = 1000.0,
  OK2_Anim.ProjSize = 1.0;

  OK2_Anim.MatrWorld = /* матрица преобразования мировой СК */
  OK2_Anim.MatrView =  /* матрица преобразования видовой СК */
  OK2_Anim.MatrProjection = MatrIdenity(); /* матрица проекции */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return TRUE;
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
  /* отключаем контексты */
  wglMakeCurrent(NULL, NULL);

  wglDeleteContext(OK2_Anim.hRC);
  ReleaseDC(OK2_Anim.hWnd, OK2_Anim.hDC);

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
  DBL ratio_x = 1, ratio_y = 1;

   /* Сохранение размера */
  OK2_Anim.W = W;
  OK2_Anim.H = H;
  /* Поле просмотра */
  glViewport(0, 0, W, H);
  
  if (W > H)
    ratio_x = (DBL)W / H;
  else
    ratio_y = (DBL)H / W;
  OK2_Anim.Wp = OK2_Anim.ProjSize * ratio_x;
  OK2_Anim.Hp = OK2_Anim.ProjSize * ratio_y;
  
  OK2_Anim.MatrProjection =
      MatrProjection(-OK2_Anim.Wp / 2, OK2_Anim.Wp / 2,
                     -OK2_Anim.Hp / 2, OK2_Anim.Hp / 2,
                      OK2_Anim.ProjDist, OK2_Anim.FarClip);
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

  /* опрос на изменение состояний объектов */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
    OK2_Anim.Units[i]->Response(OK2_Anim.Units[i], &OK2_Anim);
  
  /* очистка фона */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.3, 0.5, 0.7, 1.0);

  /* рисование объектов */
  for (i = 0; i < OK2_Anim.NumOfUnits; i++)
    OK2_Anim.Units[i]->Render(OK2_Anim.Units[i], &OK2_Anim);  

  FrameCounter++;
} /* End of 'OK2_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_AnimCopyFrame( VOID )
{
  /* вывод кадра */
  SwapBuffers(OK2_Anim.hDC);
  
  glFinish();

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
      rc.right - rc.left, rc.bottom - rc.top,
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

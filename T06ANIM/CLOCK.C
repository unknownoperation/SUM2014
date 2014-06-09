/* FILENAME: SAMPLE.C
 * PROGRAMMER: OK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

#define PI 3.14159265358979323846

/* Структура описания объекта анимации */
typedef struct tagok2UNIT_CLOCK
{
  OK2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
} ok2UNIT_CLOCK;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitInit( ok2UNIT_CLOCK *Unit, ok2ANIM *Ani )
{
} /* End of 'ClockUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitClose( ok2UNIT_CLOCK *Unit, ok2ANIM *Ani )
{
} /* End of 'ClockUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitResponse( ok2UNIT_CLOCK *Unit, ok2ANIM *Ani )
{
} /* End of 'ClockUnitResponse' function */

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
static VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, DOUBLE Angle )
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


/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CLOCK *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID ClockUnitRender( ok2UNIT_CLOCK *Unit, ok2ANIM *Ani )
{                                          
  static INT X = 0, Y = 0;
  
  SYSTEMTIME tm;

  Y += (INT)Ani->JR;
  X += (INT)Ani->JZ;
  X %= Ani->W / 2;
  Y %= Ani->H / 2;
  

  GetLocalTime(&tm);

  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(0, 255, 0));  

  DrawArrow(Ani->hDC, Ani->W / 2 + X, Ani->H / 2 + Y, Ani->H / 3.5, Ani->W / 20, (-(tm.wHour % 12 + tm.wMinute / 60.0) / 12.0) * 2 * PI);
  SetDCPenColor(Ani->hDC, RGB(0, 0, 255));
  DrawArrow(Ani->hDC, Ani->W / 2 + X, Ani->H / 2 + Y, Ani->H / 2.5, Ani->W / 25, (-(tm.wMinute + tm.wSecond / 60.0) / 60.0) * 2 * PI);
  SetDCPenColor(Ani->hDC, RGB(255, 0, 0));
  DrawArrow(Ani->hDC, Ani->W / 2 + X, Ani->H / 2 + Y, Ani->H / 2.1, Ani->W / 30, (-(tm.wSecond + tm.wMilliseconds / 1000.0) / 60.0) * 2 * PI);
  SetDCPenColor(Ani->hDC, RGB(0, 255, 255));
  DrawArrow(Ani->hDC, Ani->W / 2 + X, Ani->H / 2 + Y, Ani->H / 2, Ani->W / 32, (-tm.wMilliseconds / 1000.0) * 2 * PI);

} /* End of 'OK2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ok2UNIT *) указатель на созданный объект анимации.
 */
ok2UNIT * OK2_ClockUnitCreate( VOID )
{
  ok2UNIT_CLOCK *Unit;

  if ((Unit = (ok2UNIT_CLOCK *)OK2_AnimUnitCreate(sizeof(ok2UNIT_CLOCK))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)ClockUnitInit;
  Unit->Close = (VOID *)ClockUnitClose;
  Unit->Response = (VOID *)ClockUnitResponse;
  Unit->Render = (VOID *)ClockUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (ok2UNIT *)Unit;
} /* End of 'OK2_ClockUnitCreate' function */

/* END OF 'CLOCK.C' FILE */

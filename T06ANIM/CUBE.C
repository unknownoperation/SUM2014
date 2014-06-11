/* FILENAME: SAMPLE.C
 * PROGRAMMER: OK2
 * PURPOSE: Cube unit handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"

typedef struct
{
  INT P0, P1;
} EDGE;

/* Структура описания объекта анимации */
typedef struct tagok2UNIT_CUBE
{
  OK2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  ok2GOBJ Cow;
} ok2UNIT_CUBE;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitInit( ok2UNIT_CUBE *Unit, ok2ANIM *Ani )
{
  OK2_RndGObjLoad(&Unit->Cow, "porsche.object");
} /* End of 'CubeUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitClose( ok2UNIT_CUBE *Unit, ok2ANIM *Ani )
{
  OK2_RndGObjFree(&Unit->Cow);
} /* End of 'CubeUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitResponse( ok2UNIT_CUBE *Unit, ok2ANIM *Ani )
{
  OK2_RndWs = Ani->W;
  OK2_RndHs = Ani->H;
  OK2_RndWp = OK2_RndHp * Ani->W / Ani->H;
  OK2_RndMatrProjection = MatrProjection(-OK2_RndWp / 2, OK2_RndWp / 2, -OK2_RndHp / 2, OK2_RndHp / 2, OK2_RndProjDist, 1000.0);
} /* End of 'CubeUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitRender( ok2UNIT_CUBE *Unit, ok2ANIM *Ani )
{                                                               
  INT i, s = 5, N = 0;
  VEC p = {1, 0, 0};
  POINT pt;
  static DBL Delta = 0.1;

  Delta += Ani->JZ * Ani->GlobalDeltaTime;

  OK2_RndMatrView = MatrLookAt(PointTransform(VecSet(0, 0, Ani->JX * Delta + 0.1), MatrRotateX(80 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));

  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  srand(30);
  for (i = 0; i < N; i++)
  {
    p.X = 2.0 * rand() / RAND_MAX - 1;
    p.Y = 2.0 * rand() / RAND_MAX - 1;
    p.Z = 2.0 * rand() / RAND_MAX - 1;
    pt = OK2_RndWorldToScreen(p);
    Ellipse(Ani->hDC, pt.x - s, pt.y - s, pt.x + s, pt.y + s);
  }
  srand(30);
  for (i = 0; i < N; i++)
  {
    p.X = 2.0 * rand() / RAND_MAX - 1;
    p.Y = 2.0 * rand() / RAND_MAX - 1;
    p.Z = 2.0 * rand() / RAND_MAX - 1;
    pt = OK2_RndWorldToScreen(p);
    if (i == 0)
      MoveToEx(Ani->hDC, pt.x, pt.y, NULL);
    else
      LineTo(Ani->hDC, pt.x, pt.y);
  }
  /* OK2_RndGObjDraw(&Unit->Cow, Ani->hDC); */

  OK2_RndMatrWorld = MatrRotateY(Ani->Time * 30);

  OK2_RndMatrWorld = MatrMulMatr(OK2_RndMatrWorld, MatrScale(0.10, 0.10, 0.10));
  
  OK2_RndGObjDraw(&Unit->Cow, Ani->hDC);
  for (i = 0; i < 12; i++)
  {
    OK2_RndMatrWorld = MatrMulMatr(MatrRotate(30, 1.0, 0.0, 1.0), OK2_RndMatrWorld);
    OK2_RndMatrWorld = MatrMulMatr(MatrTranslate(0.0, 5.0, 0.0), OK2_RndMatrWorld);
    OK2_RndGObjDraw(&Unit->Cow, Ani->hDC);
  }
} /* End of 'OK2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ok2UNIT *) указатель на созданный объект анимации.
 */
ok2UNIT * OK2_CubeUnitCreate( VOID )
{
  ok2UNIT_CUBE *Unit;

  if ((Unit = (ok2UNIT_CUBE *)OK2_AnimUnitCreate(sizeof(ok2UNIT_CUBE))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)CubeUnitInit;
  Unit->Close = (VOID *)CubeUnitClose;
  Unit->Response = (VOID *)CubeUnitResponse;
  Unit->Render = (VOID *)CubeUnitRender;
} /* End of 'OK2_CubeUnitCreate' function */

/* END OF 'CUBE.C' FILE */

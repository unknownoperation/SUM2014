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

  static FLT Delta = 0.1;
  MATR WVP;

  Delta += Ani->JZ * Ani->GlobalDeltaTime;

  //Ani->MatrWorld = MatrRotateY(30);
  //Ani->MatrView = MatrLookAt(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrView = MatrLookAt(PointTransform(VecSet(20, 20, Ani->JX * Delta + 20), MatrRotateX(80 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateY(Ani->DeltaTime * 80));
  

  for (i = 0; i < 12; i++)
  {
    
    Ani->MatrWorld = MatrMulMatr(MatrTranslate(0.0, 5.0, 0.0), Ani->MatrWorld);
    Ani->MatrWorld = MatrMulMatr(MatrRotate(30, 1.0, 0.0, 1.0), Ani->MatrWorld);
    
    WVP = MatrMulMatr(Ani->MatrWorld,
      MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
    glLoadMatrixf(WVP.A[0]);
    OK2_RndGObjDraw(&Unit->Cow, Ani->hDC);
  }
  
  //OK2_RndGObjDraw(&Unit->Cow, Ani->hDC); 
  
  
  
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
  return (ok2UNIT *)Unit;
} /* End of 'OK2_CubeUnitCreate' function */

/* END OF 'CUBE.C' FILE */

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
  ok2GEOM Geom[30];
  FLT Scale[30];
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
  //OK2_GeomLoad(&Unit->Geom, ".object");
  OK2_GeomLoad(&Unit->Geom[0], "imac\\imac.object");
  Unit->Scale[0] = 50.0;

  OK2_GeomLoad(&Unit->Geom[1], "Mustang\\Mustang.object"); 
  Unit->Scale[1] = 0.005;

  OK2_GeomLoad(&Unit->Geom[2], "Avent\\Avent.object");
  Unit->Scale[2] = 1.0;

  OK2_GeomLoad(&Unit->Geom[4], "Houses\\house1.object"); 
  Unit->Scale[4] = 1.0;

  OK2_GeomLoad(&Unit->Geom[3], "BMW_M3_GTR\\BMW_M3_GTR.object");    
  Unit->Scale[3] = 0.001;

  OK2_GeomLoad(&Unit->Geom[5], "x6\\x6.object");  
  Unit->Scale[5] = 3.0;
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

static VOID DrawUnitInPosition( ok2GEOM *Geo, ok2ANIM *Ani, FLT Scale, FLT Rad, FLT Angle )
{
  FLT unscale = 1.0 / Scale;
                                                                             
  Ani->MatrWorld = MatrMulMatr(MatrRotate(Angle, 0.0, 1.0, 0.0), Ani->MatrWorld);
  Ani->MatrWorld = MatrMulMatr(MatrTranslate(Rad, 0.0, 0.0), Ani->MatrWorld);
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrScale(Scale, Scale, Scale));
  OK2_GeomDraw(Geo);
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrScale(unscale, unscale, unscale));
  Ani->MatrWorld = MatrMulMatr(MatrTranslate(-Rad, 0.0, 0.0), Ani->MatrWorld);
  //Ani->MatrWorld = MatrMulMatr(MatrRotate(-Angle, 0.0, 1.0, 0.0), Ani->MatrWorld);
}

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
  /*
  INT i, s = 5, N = 0;
  VEC p = {1, 0, 0};

  static FLT Delta = 0.1;
  MATR WVP;

  Delta += Ani->JZ * Ani->GlobalDeltaTime;

  //Ani->MatrWorld = MatrRotateY(30);
  //Ani->MatrView = MatrLookAt(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrView = MatrLookAt(PointTransform(VecSet(20, 20, Ani->JX * Delta + 20), MatrRotateX(80 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateX(Ani->DeltaTime * 80));
  

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
  
  */

  
  INT i;
  MATR WVP;
  static DBL time;

  /* оси и позиция наблюдателя */
  Ani->MatrWorld = MatrIdenity();
  Ani->MatrView =
    MatrLookAt(
      PointTransform(PointTransform(VecSet(25, 25, 25), MatrRotateY(Ani->JR * 180)), MatrRotateZ(Ani->JY * 180)),
      VecSet(0, 0, 0), VecSet(0, 1, 0));
  WVP = MatrMulMatr(OK2_Anim.MatrWorld, MatrMulMatr(OK2_Anim.MatrView, OK2_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);

  glLineWidth(3);
  glBegin(GL_LINES);
    glColor3d(1, 0.5, 0.5);
    glVertex3d(-3, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0.5, 1, 0.5);
    glVertex3d(0, -3, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0.5, 0.5, 1);
    glVertex3d(0, 0, -3);
    glVertex4d(0, 0, 1, 0);
  glEnd();
  glColorMask(1, 1, 1, 0);
  for (i = -3; i < 30; i++)
  {
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3d(-0.1, -0.1, i);
    glVertex3d(-0.1,  0.1, i);
    glVertex3d( 0.1, -0.1, i);
    glVertex3d( 0.1,  0.1, i);
    glEnd();
  }

  /* Рисуем примитивы */
  time += Ani->GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    OK2_ShadProgClose(OK2_ShaderProg);
    OK2_ShaderProg = OK2_ShadProgInit("a.vert", "a.frag");
  }


  glLineWidth(1);
  if (Ani->Keys['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Ani->MatrWorld = MatrTranslate(0, 0, 0.30 * sin(Ani->Time));
  //Ani->MatrView = MatrRotateY(0.30 * Ani->Time);
  /*
  for (i = 0; i < 12; i++)
  {
    Ani->MatrWorld = MatrMulMatr(MatrRotate(30.0, 0.0, 1.0, 0.0), Ani->MatrWorld);
    Ani->MatrWorld = MatrMulMatr(MatrTranslate(1.0, 0.0, 0.0), Ani->MatrWorld);
    OK2_GeomDraw(&Unit->Geom);
  }
  */
  for (i = 0; i < 6; i++)
    DrawUnitInPosition(&Unit->Geom[i], Ani, Unit->Scale[i], 10.0, 30.0);

  //Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrScale(50, 50, 50));
   
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

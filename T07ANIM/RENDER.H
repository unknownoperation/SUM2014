/* FILENAME: VEC.H
 * PROGRAMMER: OK2
 * PURPOSE: Matrix handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} ok2CAMERA;

/* Тип представления объекта - набора вершин и треугольников и ребер */
typedef struct tagok2GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} ok2GOBJ;

/* Функция загрузки геометрического объекта */
BOOL OK2_RndGObjLoad( ok2GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID OK2_RndGObjFree( ok2GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID OK2_RndGObjDraw( ok2GOBJ *GObj, HDC hDC );

#endif /* __RENDER_H_ */

/* End of 'RENDER.H' file */

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

/* Глобальная камера */
extern ok2CAMERA OK2_RndCam;

/* Параметры проецирования */
extern DBL
  OK2_RndWs, OK2_RndHs, /* размеры кадра в точках */
  OK2_RndWp, OK2_RndHp, /* размеры обрасти проецирования */
  OK2_RndProjDist;      /* расстояние до плоскости проекции */

/* Матрицы */
extern MATR
  OK2_RndMatrWorld,      /* матрица преобразования мировой СК */
  OK2_RndMatrView,       /* матрица преобразования видовой СК */
  OK2_RndMatrProjection; /* матрица проекции */

static MATR
  OK2_RndMatrWorldViewProj;  /* Итоговая матрица преобразования */

/* Функция преобразования из мировой системы коорлинат в кадр */
POINT OK2_RndWorldToScreen( VEC P );

/* Функция загрузки геометрического объекта */
BOOL OK2_RndGObjLoad( ok2GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID OK2_RndGObjFree( ok2GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID OK2_RndGObjDraw( ok2GOBJ *GObj, HDC hDC );

/* Функция перевычисления итоговой матрицы преобразования */
VOID OK2_RndMatrSetup( VOID );

#endif /* __RENDER_H_ */

/* End of 'RENDER.H' file */

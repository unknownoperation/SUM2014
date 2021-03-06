/* FILENAME: VEC.H
 * PROGRAMMER: OK2
 * PURPOSE: Matrix handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������ � ������������ */
typedef struct
{
  VEC
    Loc,             /* ����� ������������ */
    Dir, Up, Right,  /* �������������� ������� */
    At;              /* ����� ���������� */
} ok2CAMERA;

/* ��� ������������� ������� - ������ ������ � ������������� � ����� */
typedef struct tagok2GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} ok2GOBJ;

/* ������� �������� ��������������� ������� */
BOOL OK2_RndGObjLoad( ok2GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ��������������� ������� */
VOID OK2_RndGObjFree( ok2GOBJ *GObj );

/* ������� ��������� ��������������� ������� */
VOID OK2_RndGObjDraw( ok2GOBJ *GObj, HDC hDC );

#endif /* __RENDER_H_ */

/* End of 'RENDER.H' file */

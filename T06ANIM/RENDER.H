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

/* ���������� ������ */
extern ok2CAMERA OK2_RndCam;

/* ��������� ������������� */
extern DBL
  OK2_RndWs, OK2_RndHs, /* ������� ����� � ������ */
  OK2_RndWp, OK2_RndHp, /* ������� ������� ������������� */
  OK2_RndProjDist;      /* ���������� �� ��������� �������� */

/* ������� */
extern MATR
  OK2_RndMatrWorld,      /* ������� �������������� ������� �� */
  OK2_RndMatrView,       /* ������� �������������� ������� �� */
  OK2_RndMatrProjection; /* ������� �������� */

static MATR
  OK2_RndMatrWorldViewProj;  /* �������� ������� �������������� */

/* ������� �������������� �� ������� ������� ��������� � ���� */
POINT OK2_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� ������� */
BOOL OK2_RndGObjLoad( ok2GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ��������������� ������� */
VOID OK2_RndGObjFree( ok2GOBJ *GObj );

/* ������� ��������� ��������������� ������� */
VOID OK2_RndGObjDraw( ok2GOBJ *GObj, HDC hDC );

/* ������� �������������� �������� ������� �������������� */
VOID OK2_RndMatrSetup( VOID );

#endif /* __RENDER_H_ */

/* End of 'RENDER.H' file */

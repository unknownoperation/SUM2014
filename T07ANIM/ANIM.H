/* FILENAME: ANIM.H
 * PROGRAMMER: OK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"
#include "geom.h"
#include "shader.h"

/* ������������ ���������� ��������� �������� */
#define OK2_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagok2UNIT ok2UNIT;

/* ��������� �������� ��������� �������� */
typedef struct tagok2ANIM
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HDC hDC;            /* �������� ���� ������ */
  HGLRC hRC;              /* �������� ���������� */

  /* ������ ��������� �������� � �� ���������� */
  ok2UNIT *Units[OK2_MAX_UNITS]; 
  INT NumOfUnits;

  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */

  /* ���������� ����� */
  BYTE Keys[256], KeysOld[256], KeyClick[256]; /* �������� ������ ���������� */

  /* Mouse variables */
  INT MsX, MsY, MsWheel, MsDeltaY, MsDeltaX;

  /* Joystick variables */
  INT JButsOld[32], JButs[32], JButsClick[32];
  DBL JX, JY, JZ, JR, JU, JPOV;
  /* ��������� ������������� */
  DBL
    Wp, Hp,    /* ������� ������� ������������� */
    ProjDist,  /* ���������� �� ��������� �������� */
    ProjSize,  /*  */
    FarClip;   /* ������� ��������� ��������� */
  /* ������� */
  MATR
    MatrWorld,      /* ������� �������������� ������� �� */
    MatrView,       /* ������� �������������� ������� �� */
    MatrProjection; /* ������� �������� */

} ok2ANIM;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( ok2UNIT *Unit, ok2ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( ok2UNIT *Unit, ok2ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( ok2UNIT *Unit, ok2ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( ok2UNIT *Unit, ok2ANIM *Ani );
 */
#define OK2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ok2UNIT *Unit, ok2ANIM *Ani );       \
  VOID (*Close)( ok2UNIT *Unit, ok2ANIM *Ani );      \
  VOID (*Response)( ok2UNIT *Unit, ok2ANIM *Ani );   \
  VOID (*Render)( ok2UNIT *Unit, ok2ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagok2UNIT
{
  OK2_UNIT_BASE_FIELDS; /* ������� ���� */
};

/***
 * ������� ���������� ���������
 ***/

/* ������� ������������� �������� */
BOOL OK2_AnimInit( HWND hWnd );

/* ������� ��������������� �������� */
VOID OK2_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������ */
VOID OK2_AnimResize( INT W, INT H );

/* ������� ���������� ����� �������� */
VOID OK2_AnimRender( VOID );

/* ������� ������ ����� �������� */
VOID OK2_AnimCopyFrame( VOID );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ��������� */
VOID OK2_AnimFlipFullScreen( VOID );

/* ������� ��������� ����� �������� */
VOID OK2_AnimSetPause( BOOL NewPauseFlag );

/***
 * ������� ��������� �������� ��������
 ***/

/* ������� ���������� � ������� ������� �������� */
VOID OK2_AnimAddUnit( ok2UNIT *Unit );

/* ������� �������� ������� �������� */
ok2UNIT * OK2_AnimUnitCreate( INT Size );

extern UINT OK2_ShaderProg;

/* ��������� �������� �������� */
extern ok2ANIM OK2_Anim;

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */

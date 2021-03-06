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

/* ��������� �������� ������� �������� */
typedef struct tagok2UNIT_PHOTO
{
  OK2_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */
  IMAGE And, Xor;       /* Logo pictures */
} ok2UNIT_PHOTO;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_PHOTO *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PhotoUnitInit( ok2UNIT_PHOTO *Unit, ok2ANIM *Ani )
{
  ImageLoad(&Unit->And, "logo.bmp");
  ImageLoad(&Unit->Xor, "logo_xor.bmp");
} /* End of 'PhotoUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_PHOTO *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PhotoUnitClose( ok2UNIT_PHOTO *Unit, ok2ANIM *Ani )
{
} /* End of 'PhotoUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_PHOTO *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PhotoUnitResponse( ok2UNIT_PHOTO *Unit, ok2ANIM *Ani )
{
} /* End of 'PhotoUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_PHOTO *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PhotoUnitRender( ok2UNIT_PHOTO *Unit, ok2ANIM *Ani )
{                                                               
  static INT X = 0, Y = 0;
  X += (INT)Ani->JX;
  Y += (INT)Ani->JY;
  X %= Ani->W;
  Y %= Ani->H;
  DrawImage(Ani->hDC, &(Unit->Xor), Ani->W - Unit->And.W + X, Y, 0);
  DrawImage(Ani->hDC, &(Unit->And), Ani->W - Unit->And.W + X, Y, 1);
} /* End of 'OK2_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ok2UNIT *) ��������� �� ��������� ������ ��������.
 */
ok2UNIT * OK2_PhotoUnitCreate( VOID )
{
  ok2UNIT_PHOTO *Unit;

  if ((Unit = (ok2UNIT_PHOTO *)OK2_AnimUnitCreate(sizeof(ok2UNIT_PHOTO))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)PhotoUnitInit;
  Unit->Close = (VOID *)PhotoUnitClose;
  Unit->Response = (VOID *)PhotoUnitResponse;
  Unit->Render = (VOID *)PhotoUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (ok2UNIT *)Unit;
} /* End of 'OK2_PhotoUnitCreate' function */

/* END OF 'PHOTO.C' FILE */

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
typedef struct tagok2UNIT_COW
{
  OK2_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */
  IMAGE And, Xor;       /* Logo pictures */
} ok2UNIT_COW;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitInit( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
  ImageLoad(&Unit->And, "cow_and.bmp");
  ImageLoad(&Unit->Xor, "cow_xor.bmp");
} /* End of 'CowUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitClose( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
} /* End of 'CowUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitResponse( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitRender( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
  DBL
    x = Unit->ShiftX + sin(Ani->Time - Unit->ShiftX) * Ani->W,
    y = Unit->ShiftY + sin(Ani->Time - Unit->ShiftY) * Ani->H;

  /*
  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30);
  */
  DrawImage(Ani->hDC, &(Unit->Xor), x, y, 0);
  DrawImage(Ani->hDC, &(Unit->And), x, y, 1);
} /* End of 'OK2_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ok2UNIT *) ��������� �� ��������� ������ ��������.
 */
ok2UNIT * OK2_CowUnitCreate( VOID )
{
  ok2UNIT_COW *Unit;

  if ((Unit = (ok2UNIT_COW *)OK2_AnimUnitCreate(sizeof(ok2UNIT_COW))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (ok2UNIT *)Unit;
} /* End of 'OK2_CowUnitCreate' function */

/***********************************************************/

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( ok2UNIT *Unit, ok2ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, 
    sprintf(Buf, 
      "FPS: %.3f MsX: %i MsY: %i MsDeltaX: %i MsDeltaY: %i MsWheel: %i", 
      Ani->FPS, Ani->MsX, Ani->MsY, Ani->MsDeltaX, Ani->MsDeltaY, Ani->MsWheel));
} /* End of 'OK2_AnimUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ok2UNIT *) ��������� �� ��������� ������ ��������.
 */
ok2UNIT * OK2_InfoUnitCreate( VOID )
{
  ok2UNIT *Unit;

  if ((Unit = OK2_AnimUnitCreate(sizeof(ok2UNIT))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'OK2_InfoUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */

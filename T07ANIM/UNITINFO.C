/* FILENAME: UNITINFO.C
 * PROGRAMMER: OK2
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagok2UNIT_INFO
{
  OK2_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
} ok2UNIT_INFO;

/* ������� ������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitInit( ok2UNIT_INFO *Unit, ok2ANIM *Ani )
{
} /* End of 'InfoUnitInit' function */

/* ������� ��������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitClose( ok2UNIT_INFO *Unit, ok2ANIM *Ani )
{
} /* End of 'InfoUnitClose' function */

/* ������� ���������� ����������� ���������� ���������������
 * ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitResponse( ok2UNIT_INFO *Unit, ok2ANIM *Ani )
{
  /*
  if (Ani->Keys[VK_LMENU] && Ani->KeyClick[VK_RETURN])
    OK2_AnimFlipFullScreen();
  if (Ani->KeyClick['P'])
    OK2_AnimSetPause(!Ani->IsPause);
  */
} /* End of 'InfoUnitResponse' function */

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2UNIT_INFO *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( ok2UNIT_INFO *Unit, ok2ANIM *Ani )
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);

} /* End of 'InfoUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ok2UNIT_INFO *) ��������� �� ��������� ������ ��������.
 */
ok2UNIT * OK2_InfoUnitCreate( VOID )
{
  ok2UNIT_INFO *Unit;

  if ((Unit = (ok2UNIT_INFO *)OK2_AnimUnitCreate(sizeof(ok2UNIT_INFO))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)InfoUnitResponse;
  return (ok2UNIT *)Unit;
} /* End of 'OK2_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */

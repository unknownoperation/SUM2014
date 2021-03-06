/* FILENAME: UNIT.C
 * PROGRAMMER: OK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OK2_AnimUnitInit( ok2UNIT *Unit, ok2ANIM *Ani )
{
} /* End of 'OK2_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OK2_AnimUnitClose( ok2UNIT *Unit, ok2ANIM *Ani )
{
} /* End of 'OK2_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OK2_AnimUnitResponse( ok2UNIT *Unit, ok2ANIM *Ani )
{
} /* End of 'OK2_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ok2UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ok2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID OK2_AnimUnitRender( ok2UNIT *Unit, ok2ANIM *Ani )
{
} /* End of 'OK2_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ok2UNIT *) ��������� �� ��������� ������ ��������.
 */
ok2UNIT * OK2_AnimUnitCreate( INT Size )
{
  ok2UNIT *Unit;

  if (Size < sizeof(ok2UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* ��������� ���� ��-��������� */
  Unit->Size = Size;
  Unit->Init = OK2_AnimUnitInit;
  Unit->Close = OK2_AnimUnitClose;
  Unit->Response = OK2_AnimUnitResponse;
  Unit->Render = OK2_AnimUnitRender;
  return Unit;       
} /* End of 'OK2_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */


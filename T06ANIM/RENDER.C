/* FILENAME: RENDER.C
 * PROGRAMMER: OK2
 * PURPOSE: Render handele module
 * LAST UPDATE: 10.06.2014
 */
#include <stdio.h>

#include "render.h"

/* ���������� ������ */
ok2CAMERA OK2_RndCam;

/* ��������� ������������� */
DBL
  OK2_RndWs = 30.0, OK2_RndHs = 30.0,   /* ������� ����� � ������ */
  OK2_RndWp = 4.0, OK2_RndHp = 4.0,     /* ������� ������� ������������� */
  OK2_RndProjDist = 1.0;              /* ���������� �� ��������� �������� */

/* ������� */
MATR                       
  OK2_RndMatrWorld, /* ������� �������������� ������� �� */
  OK2_RndMatrView,  /* ������� �������������� ������� �� */
  OK2_RndMatrProjection; /* ������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT OK2_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* �������������� �� */
  Pp = PointTransform(P, OK2_RndMatrWorldViewProj);
  
  /* ������������� */
  Ps.x = (Pp.X + 0.5) * (OK2_RndWs - 1);
  Ps.y = (-Pp.Y + 0.5) * (OK2_RndHs - 1);
  return Ps;
} /* End of 'OK2_RndWorldToScreen' function */

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       ok2GOBJ *GObj;
 *   - ��� ����� (*.OBJ):
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL OK2_RndGObjLoad( ok2GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* �������� ������ */
  memset(GObj, 0, sizeof(ok2GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* ������� ���������� ������ � ������ */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* �������� ������ ��� ������� � ����� ��� ������ ������� ������
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* ��������� ������ */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 || 
      sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf + 2, "%i %i %i", &a, &b, &c);
      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'OK2_RndGObjLoad' function */

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       ok2GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID OK2_RndGObjFree( ok2GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(ok2GOBJ));
} /* End of 'OK2_RndGObjFree' function */

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       ok2GOBJ *GObj;
 *   - �������� ���������� ������:
 *       HDC hDC;
 * ������������ ��������: ���.
 */
VOID OK2_RndGObjDraw( ok2GOBJ *GObj, HDC hDC )
{
  INT i, j, s = 1;
  POINT *pts;

  OK2_RndMatrSetup();

  /* �������� ������ ��� �������� ����� */
  if ((pts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;
  for (i = 0; i < GObj->NumOfV; i++)
  {
    pts[i] = OK2_RndWorldToScreen(GObj->V[i]);
    /* Ellipse(hDC, pts[i].x - s, pts[i].y - s, pts[i].x + s, pts[i].y + s); */
  }
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n0 = GObj->F[i][0],
      n1 = GObj->F[i][1],
      n2 = GObj->F[i][2];

    MoveToEx(hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(hDC, pts[n1].x, pts[n1].y);
    LineTo(hDC, pts[n2].x, pts[n2].y);
    LineTo(hDC, pts[n0].x, pts[n0].y);
  }
  free(pts);

  /*
  for (i = 0; i < GObj->NumOfV; i++)
  {
    pt[0] = OK2_RndWorldToScreen(GObj->V[i]);
    Ellipse(hDC, pt[0].x - s, pt[0].y - s, pt[0].x + s, pt[0].y + s);
  }
  for (i = 0; i < GObj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      pt[j] = OK2_RndWorldToScreen(GObj->V[GObj->F[i][j]]);
    MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
    LineTo(hDC, pt[1].x, pt[1].y);
    LineTo(hDC, pt[2].x, pt[2].y);
    LineTo(hDC, pt[0].x, pt[0].y);
  }
  */
} /* End of 'OK2_RndGObjDraw' function */

/* ������� �������������� �������� ������� ��������������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID OK2_RndMatrSetup( VOID )
{
  OK2_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(OK2_RndMatrWorld, OK2_RndMatrView),
    OK2_RndMatrProjection);
} /* End of 'OK2_RndMatrSetup' function */

/* End of 'RENDER.C' file */

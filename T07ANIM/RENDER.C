/* FILENAME: RENDER.C
 * PROGRAMMER: OK2
 * PURPOSE: Render handele module
 * LAST UPDATE: 13.06.2014
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

/* Глобальная камера */
ok2CAMERA OK2_RndCam;

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ok2GOBJ *GObj;
 *   - имя файла (*.OBJ):
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL OK2_RndGObjLoad( ok2GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* обнулили данные */
  memset(GObj, 0, sizeof(ok2GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* считаем количество вершин и граней */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* выделяем память под вершины и грани как единый участок памяти
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* считываем данные */
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

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ok2GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_RndGObjFree( ok2GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(ok2GOBJ));
} /* End of 'OK2_RndGObjFree' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ok2GOBJ *GObj;
 *   - контекст устройства вывода:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID OK2_RndGObjDraw( ok2GOBJ *GObj, HDC hDC )
{
  
  INT i, s = 1;
  
  glBegin(GL_LINES);
  glColor3d(1, 1, 1);
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n0 = GObj->F[i][0],
      n1 = GObj->F[i][1],
      n2 = GObj->F[i][2];
                                   
    glVertex3fv(&GObj->V[n0].X);
    glVertex3fv(&GObj->V[n1].X);
    glVertex3fv(&GObj->V[n2].X);
    glVertex3fv(&GObj->V[n0].X);
  }
  glEnd();
} /* End of 'OK2_RndGObjDraw' function */

/* End of 'RENDER.C' file */

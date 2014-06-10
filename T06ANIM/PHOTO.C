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

/* Структура описания объекта анимации */
typedef struct tagok2UNIT_PHOTO
{
  OK2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
  IMAGE And, Xor;       /* Logo pictures */
} ok2UNIT_PHOTO;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_PHOTO *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PhotoUnitInit( ok2UNIT_PHOTO *Unit, ok2ANIM *Ani )
{
  ImageLoad(&Unit->And, "logo.bmp");
  ImageLoad(&Unit->Xor, "logo_xor.bmp");
} /* End of 'PhotoUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_PHOTO *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PhotoUnitClose( ok2UNIT_PHOTO *Unit, ok2ANIM *Ani )
{
} /* End of 'PhotoUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_PHOTO *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PhotoUnitResponse( ok2UNIT_PHOTO *Unit, ok2ANIM *Ani )
{
} /* End of 'PhotoUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_PHOTO *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ok2UNIT *) указатель на созданный объект анимации.
 */
ok2UNIT * OK2_PhotoUnitCreate( VOID )
{
  ok2UNIT_PHOTO *Unit;

  if ((Unit = (ok2UNIT_PHOTO *)OK2_AnimUnitCreate(sizeof(ok2UNIT_PHOTO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
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

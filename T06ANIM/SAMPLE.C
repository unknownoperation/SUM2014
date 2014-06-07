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
typedef struct tagok2UNIT_COW
{
  OK2_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;   /* Смещение */
  INT Type;             /* Вид */
} ok2UNIT_COW;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitInit( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
} /* End of 'CowUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitClose( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
} /* End of 'CowUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitResponse( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowUnitRender( ok2UNIT_COW *Unit, ok2ANIM *Ani )
{
  DBL
    x = Unit->ShiftX + sin(Ani->Time + Unit->ShiftX) * Ani->W,
    y = Unit->ShiftY + sin(Ani->Time + Unit->ShiftY) * Ani->H;

  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30);
} /* End of 'OK2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ok2UNIT *) указатель на созданный объект анимации.
 */
ok2UNIT * OK2_CowUnitCreate( VOID )
{
  ok2UNIT_COW *Unit;

  if ((Unit = (ok2UNIT_COW *)OK2_AnimUnitCreate(sizeof(ok2UNIT_COW))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
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

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ok2UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ok2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitRender( ok2UNIT *Unit, ok2ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'OK2_AnimUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ok2UNIT *) указатель на созданный объект анимации.
 */
ok2UNIT * OK2_InfoUnitCreate( VOID )
{
  ok2UNIT *Unit;

  if ((Unit = OK2_AnimUnitCreate(sizeof(ok2UNIT))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'OK2_InfoUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */

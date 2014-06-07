/* FILENAME: ANIM.H
 * PROGRAMMER: OK2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Максимальное количество элементов анимации */
#define OK2_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagok2UNIT ok2UNIT;

/* Структура хранения контекста анимации */
typedef struct tagok2ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  ok2UNIT *Units[OK2_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE Keys[256];     /* Сотояние клавиш клавиатуры и мыши */
} ok2ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( ok2UNIT *Unit, ok2ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( ok2UNIT *Unit, ok2ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( ok2UNIT *Unit, ok2ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( ok2UNIT *Unit, ok2ANIM *Ani );
 */
#define OK2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ok2UNIT *Unit, ok2ANIM *Ani );       \
  VOID (*Close)( ok2UNIT *Unit, ok2ANIM *Ani );      \
  VOID (*Response)( ok2UNIT *Unit, ok2ANIM *Ani );   \
  VOID (*Render)( ok2UNIT *Unit, ok2ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagok2UNIT
{
  OK2_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
VOID OK2_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID OK2_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID OK2_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID OK2_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID OK2_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID OK2_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID OK2_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID OK2_AnimAddUnit( ok2UNIT *Unit );

/* Функция создания объекта анимации */
ok2UNIT * OK2_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */

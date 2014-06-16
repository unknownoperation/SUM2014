/***
 * Работа с геометрическими объектами
 ***/

/* Тип представления текстурных координат */
typedef struct tagok2UV
{
  FLT U, V; /* компоненты текстурных координат */
} ok2UV;

/* Функиця инициализации текстурных координат */
__inline ok2UV OK2_UVSet( FLT U, FLT V )
{
  ok2UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'OK2_UVSet' function */

/* Теп представления вершины */
typedef struct tagok2VERTEX
{
  VEC P;    /* координаты */
  ok2UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} ok2VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagok2PRIM_TYPE
{
  OK2_PRIM_TRIMESH, OK2_PRIM_GRID
} ok2PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagok2PRIM
{
  ok2PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  ok2VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} ok2PRIM;

/* Цвет по-умолчанию */
extern COLOR OK2_DefaultColor;

/* Функция создания примитива */
BOOL OK2_PrimCreate( ok2PRIM *P, ok2PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL OK2_PrimCreateSphere( ok2PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID OK2_PrimFree( ok2PRIM *P );

/* Функция отрисовки примитива */
VOID OK2_PrimDraw( ok2PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagok2MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} ok2MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagok2GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  ok2MATERIAL *Mtls;         /* таблица материалов */
  ok2PRIM *Prims;            /* массив примитивов */
} ok2GEOM;

/* Функция добавления материала к объекту */
INT OK2_GeomAddMaterial( ok2GEOM *G,
                         ok2MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT OK2_GeomAddPrim( ok2GEOM *G, ok2PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID OK2_GeomFree( ok2GEOM *G );

/* Функция рисования */
VOID OK2_GeomDraw( ok2GEOM *G );

/* Функция загрузки геометрического объекта */
BOOL OK2_GeomLoad( ok2GEOM *G, CHAR *FileName );


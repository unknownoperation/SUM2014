/***
 * ������ � ��������������� ���������
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagok2UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} ok2UV;

/* ������� ������������� ���������� ��������� */
__inline ok2UV OK2_UVSet( FLT U, FLT V )
{
  ok2UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'OK2_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagok2VERTEX
{
  VEC P;    /* ���������� */
  ok2UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} ok2VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagok2PRIM_TYPE
{
  OK2_PRIM_TRIMESH, OK2_PRIM_GRID
} ok2PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagok2PRIM
{
  ok2PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  ok2VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} ok2PRIM;

/* ���� ��-��������� */
extern COLOR OK2_DefaultColor;

/* ������� �������� ��������� */
BOOL OK2_PrimCreate( ok2PRIM *P, ok2PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL OK2_PrimCreateSphere( ok2PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� ��������� */
VOID OK2_PrimFree( ok2PRIM *P );

/* ������� ��������� ��������� */
VOID OK2_PrimDraw( ok2PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagok2MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} ok2MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagok2GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  ok2MATERIAL *Mtls;         /* ������� ���������� */
  ok2PRIM *Prims;            /* ������ ���������� */
} ok2GEOM;

/* ������� ���������� ��������� � ������� */
INT OK2_GeomAddMaterial( ok2GEOM *G,
                         ok2MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT OK2_GeomAddPrim( ok2GEOM *G, ok2PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID OK2_GeomFree( ok2GEOM *G );

/* ������� ��������� */
VOID OK2_GeomDraw( ok2GEOM *G );

/* ������� �������� ��������������� ������� */
BOOL OK2_GeomLoad( ok2GEOM *G, CHAR *FileName );


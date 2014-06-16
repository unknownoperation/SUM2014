/* FILENAME: VEC.H
 * PROGRAMMER: OK2
 * PURPOSE: Matrix handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#include <windows.h>

#include "def.h"

/* Common float point types */
typedef float FLT;
typedef double DBL;

#define OK2_PI 3.14159265358979323846
#define DEGREE2RADIANL 0.01745329251994329576L
#define D2R(A) ((A) * (OK2_PI / 180.0))

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

/* тип цвета */
typedef struct tagCOLOR
{
  FLT R, G, B, A;
} COLOR;

/* Unit Matrix */
extern MATR UnitMatrix;

/* Функция вычисления скалярного произведения двух векторов */
__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */
/* Функция вычисления длины вектора */
__inline FLT VecLen( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
} /* End of 'VecLen' function */
/* Функция создания вектора */
__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */
/* функция сложения двух векторов */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X,
                V1.Y + V2.Y,
                V1.Z + V2.Z);
} /* End of 'VecAddVec' function */
/* функция разности двух векторов */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X,
                V1.Y - V2.Y,
                V1.Z - V2.Z);
} /* End of 'VecSubVec' function */
/* функция умножения вектора на число */
__inline VEC VecMulNum( VEC V, FLT N )
{
  return VecSet(V.X * N, V.Y * N, V.Z * N);
} /* End of 'VecMulNum' function */
/* функция деления вектора на число */
__inline VEC VecDivNum( VEC V, FLT N )
{
  return VecSet(V.X / N, V.Y / N, V.Z / N);
} /* End of 'VecDivNum' function */
/* функция вычисления обратного вектора */
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */
/* функция векторного произведения векторов */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{                                                  
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,       
                V2.X * V1.Z - V2.Z * V1.X,               
                V1.X * V2.Y - V2.X * V1.Y);
} /* End of 'VecCrossVec' function */
/* функция вычисления квадрата длины вектора */
__inline FLT VecLen2( VEC V )
{
  return VecDotVec(V, V);
} /* End of 'VecLen2' function */
/* функция нормализации вектора */
__inline VEC VecNormalize( VEC V )
{
  FLT a = VecLen2(V);
  if (a != 0 && a != 1)
    return VecDivNum(V, sqrt(a));
  return V;  
} /* End of 'VecNormalize' function */

/* Matrix functions */

/* return unit matrix function */
__inline MATR MatrIdenity( VOID )
{
  return UnitMatrix;
} /* End of 'MatrIdenity' function */
/* matrix translate function */
__inline MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
} /* End of 'MatrTranslate' function */
/* scale matrix function */
__inline MATR MatrScale( FLT Sx, FLT Sy, FLT Sz )
{
  MATR m = UnitMatrix;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
} /* End of 'MatrScale' function */
/* rotate matrix by Z function */
__inline MATR MatrRotateZ( DBL AngleInDegree )
{  
  MATR m = UnitMatrix;        
  /* Multiplyer degree to radian constant */
  long double MultiplierDegree2Radian = DEGREE2RADIANL;

  _asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fld  st(0)
    fstp qword ptr m.A /* cos -> a11 */
    fstp qword ptr (m.A + 40) /* cos -> a22 */

    fld  st(0) /* duplicate st(0) */
    fstp qword ptr (m.A + 8) /* sin -> a12 */
    fldz       /* push 0 (st(0) = 0) */
    fsubr      /* st(0) = 0 - sin() */
    fstp qword ptr (m.A + 32) /* -sin -> a21 */
  }

  return m;
} /* End of 'MatrRotateZ' function */          
/* rotate matrix by Y function */
__inline MATR MatrRotateY( DBL AngleInDegree )
{  
  MATR m = UnitMatrix;        
  /* Multiplyer degree to radian constant */
  long double MultiplierDegree2Radian = DEGREE2RADIANL;

  _asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fld  st(0)
    fstp qword ptr m.A /* cos -> a11 */
    fstp qword ptr (m.A + 80) /* cos -> a33 */

    fld  st(0) /* duplicate st(0) */
    fstp qword ptr (m.A + 64) /* sin -> a31 */
    fldz       /* push 0 (st(0) = 0) */
    fsubr      /* st(0) = 0 - sin() */
    fstp qword ptr (m.A + 16) /* -sin -> a13 */
  }

  return m;
} /* End of 'MatrRotateY' function */    
/* rotate matrix by X function */
__inline MATR MatrRotateX( FLT AngleInDegree )
{  
   DBL sine, cosine;
   MATR m = MatrIdenity();
   FLT a = DEGREE2RADIANL;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul a

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  m.A[1][1] = cosine;
  m.A[2][2] = cosine;
  m.A[1][2] = sine;
  m.A[2][1] = -sine;
  return m;

} /* End of 'MatrRotateX' function */

/* rotate matrix function */
__inline MATR MatrRotate( FLT AngleInDegree,
                 FLT X, FLT Y, FLT Z )
{
  FLT a, si, co, len;
  MATR m;

  a = D2R(AngleInDegree / -2.0);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 * co * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * co * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * co * Y;
  m.A[2][1] = 2 * co * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'MatrRotate' function */

/* Mul 2 matrix function */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  int i, j, k;
  MATR m;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.A[i][j] = 0, k = 0; k < 4; k++)
        m.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return m;
} /* End of 'MatrMulMatr' function */

/* Transpose matrix function */
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR m = M;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      m.A[i][j] = M.A[j][i];
  return m;
} /* End of 'MatrTranspose' function */
/* Determinate 3x3 count function */
__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */
/* Determinate 4x4 count function */
__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */
/* Inverse matrix function */
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm(M);
  INT i, j, perm[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return UnitMatrix;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        MatrDeterm3x3(
          M.A[perm[i][0]][perm[j][0]],
          M.A[perm[i][0]][perm[j][1]],
          M.A[perm[i][0]][perm[j][2]],
          M.A[perm[i][1]][perm[j][0]],
          M.A[perm[i][1]][perm[j][1]],
          M.A[perm[i][1]][perm[j][2]],
          M.A[perm[i][2]][perm[j][0]],
          M.A[perm[i][2]][perm[j][1]],
          M.A[perm[i][2]][perm[j][2]]) / det;
  return r;
} /* End of 'MatrInverse' function */
/* Transform point function */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(M.A[0][0] * V.X + M.A[1][0] * V.Y + M.A[2][0] * V.Z + M.A[3][0],
                M.A[0][1] * V.X + M.A[1][1] * V.Y + M.A[2][1] * V.Z + M.A[3][1],
                M.A[0][2] * V.X + M.A[1][2] * V.Y + M.A[2][2] * V.Z + M.A[3][2]);
} /* End of 'PointTransform' function */
/* Transform vector function */
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(M.A[0][0] * V.X + M.A[1][0] * V.Y + M.A[2][0] * V.Z,
                M.A[0][1] * V.X + M.A[1][1] * V.Y + M.A[2][1] * V.Z,
                M.A[0][2] * V.X + M.A[1][2] * V.Y + M.A[2][2] * V.Z);
} /* End of 'VectorTransform' function */

__inline MATR MatrLookAt( VEC Loc, VEC At, VEC UpApprox )
{
  VEC Right, Up, Dir;
  MATR r;
  		
  Dir = VecNormalize(VecSubVec(At, Loc));
  Right = VecNormalize(VecCrossVec(Dir, UpApprox));
  Up = VecCrossVec(Right, Dir);

  r.A[0][0] = Right.X; r.A[0][1] = Up.X; r.A[0][2] = -Dir.X; r.A[0][3] = 0;
  r.A[1][0] = Right.Y; r.A[1][1] = Up.Y; r.A[1][2] = -Dir.Y; r.A[1][3] = 0;
  r.A[2][0] = Right.Z; r.A[2][1] = Up.Z; r.A[2][2] = -Dir.Z; r.A[2][3] = 0;
  r.A[3][0] = -VecDotVec(Loc, Right);
  r.A[3][1] = -VecDotVec(Loc, Up);
  r.A[3][2] = VecDotVec(Loc, Dir);
  r.A[3][3] = 1;
  return r;
}

__inline MATR MatrProjection( FLT Left, FLT Right,
                              FLT Bottom, FLT Top,
                              FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return m;
} /* End of 'MatrProjection' function */

__inline COLOR ColorSet(FLT R, FLT G, FLT B)
{
  COLOR a;
  a.R = R;
  a.G = G;
  a.B = B;

  return a;
}

#endif /* __VEC_H_ */

/* END OF 'VEC.H' FILE */

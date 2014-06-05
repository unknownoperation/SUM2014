/* Korneev Oleg 10-2 04.06.2014 */

#include <stdio.h>

#define N 6

int Arr[N], Parity;
int Lenght;
double A[N][N], Det = 0;

void LoadMa( char *FileName )
{
  FILE *F;
  int i, j;
  double x = 15.0;

  Lenght = 0;
  F = fopen(FileName, "r");
  if (F == NULL)                                              
    return;
  fscanf(F, "%i", &Lenght);
  if (Lenght < 0)
    Lenght = 0;
  else
    if (Lenght > N)
      Lenght = N;
  
  for (i = 0; i < Lenght; i++)
    for (j = 0; j < Lenght; j++)
      fscanf(F, "%lf", &A[i][j]);
  x *= 2.0;
}

void Swap( int A, int B )
{
  int t;
  t = Arr[A];
  Arr[A] = Arr[B];
  Arr[B] = t;
}

void ParityCheck( void )
{
  int i, c = 0, j;
  for (i = 0; i < N; i++)
    for (j = i; j < N; j++)
      if (Arr[i] > Arr[j])
        c++;
  if (!(c % 2))
    Parity = 1;
  else 
    Parity = -1;
}

void PrintArr1( void )
{
  int i, j, c = 0;
  FILE *F;

  if ((F = fopen("permutation.txt", "at")) == NULL)
    return;
  for (i = 0; i < N; i++)
  {
    for (j = i; j < N; j++)
      if (Arr[i] > Arr[j])
        c++;
    fprintf(F, "%i, ", Arr[i]);
  }
  fprintf(F, "number of inversions is %i \n", c);
  fclose(F);
}

void Go( int Pos )
{
  int i;
  double prod = 1;

  if (Pos == Lenght)
  {
    ParityCheck();
    for (i = 0; i < Lenght; i++)
      prod *= A[i][Arr[i]];
    prod *= Parity;
    Det += prod;
    return;
  }
  
  Go(Pos + 1);
  for (i = Pos + 1; i < Lenght; i++)
  {
    Swap(i, Pos);
    Go(Pos + 1);
    Swap(i, Pos);
  }
}

void PrintMatrix( void )
{
  int i, j;
  
  for (i = 0; i < Lenght; i++)
  {
    for (j = 0; j < Lenght; j++)
      printf("%lf ", A[i][j]);
    printf("\n");
  }
}

void Gausse( void )
{
  int i, j, m, k;
  double a = A[0][0], ttt, ratio, deter = 1;

  for (j = 0; j < Lenght; j++)
  {
    for (k = j; k < Lenght - 1; k++)
      for (i = j; i < Lenght - 1; i++)
        if (A[i][j] < A[i + 1][j])
        {
          deter *=   -1;
          for (m = 0; m < Lenght; m++)
          {
            ttt = A[i][m];
            A[i][m] = A[i + 1][m];
            A[i + 1][m] = ttt;
            
          }
        }
  }
  printf("\n\n");
  PrintMatrix();

  for (j = 0; j < Lenght; j++) 
    for (k = j + 1; k < Lenght; k++)
      if (A[j][j] != 0)
      {       
        ratio = A[j][k] / A[j][j];
        for (m = j; m < Lenght; m++)
          A[m][k] -= ratio * A[m][j];
      }
      
  for (i = 0; i < Lenght; i++)
    deter *= A[i][i];
  printf("%lf \n", deter);
}

void main( void )
{
  int i, j;

  

  LoadMa("matrix.txt");
  
  for (j = 0; j < Lenght; j++)
      Arr[j] = j;
  
  /*
  for (i = 0; i < Lenght; i++)
  {
    for (j = 0; j < Lenght; j++)
      Arr[j] = A[i][j];
    Go(0);
  }  */
  
  Go(0);
  
  printf("%lf \n", Det);
  PrintMatrix();
  Gausse();
  PrintMatrix();
  getchar();
}

/* End of 'T04DETERM' file */

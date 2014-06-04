/* Korneev Oleg 10-2 04.06.2014 */

#include <stdio.h>

#define N 6

int Arr[N], Parity;
int Amount;
double A[N][N], Det = 0;

void LoadMa( char *FileName )
{
  FILE *F;
  int i, j;
  double x = 15.0;

  Amount = 0;
  F = fopen(FileName, "r");
  if (F == NULL)                                              
    return;
  fscanf(F, "%i", &Amount);
  if (Amount < 0)
    Amount = 0;
  else
    if (Amount > N)
      Amount = N;
  
  for (i = 0; i < Amount; i++)
    for (j = 0; j < Amount; j++)
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

  if (Pos == Amount)
  {
    ParityCheck();
    for (i = 0; i < Amount; i++)
      prod *= A[i][Arr[i]];
    prod *= Parity;
    Det += prod;
    return;
  }
  
  Go(Pos + 1);
  for (i = Pos + 1; i < Amount; i++)
  {
    Swap(i, Pos);
    Go(Pos + 1);
    Swap(i, Pos);
  }
}

void main( void )
{
  int i, j;

  

  LoadMa("matrix.txt");
  for (j = 0; j < Amount; j++)
      Arr[j] = j;
  
  /*
  for (i = 0; i < Amount; i++)
  {
    for (j = 0; j < Amount; j++)
      Arr[j] = A[i][j];
    Go(0);
  }  */
  Go(0);
  printf("%lf", Det);
  getchar();
}

/* End of 'T04DETERM' file */

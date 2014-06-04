/* Korneev Oleg 10-2 04.06.2014 */

#include <stdio.h>

#define N 3

int Arr[N];

void Swap( int A, int B )
{
  int t;
  t = Arr[A];
  Arr[A] = Arr[B];
  Arr[B] = t;
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

  if (Pos == N)
  {
    PrintArr1();  
    return;
  }
  
  Go(Pos + 1);
  for (i = Pos + 1; i < N; i++)
  {
    Swap(i, Pos);
    Go(Pos + 1);
    Swap(i, Pos);
  }
}

void main( void )
{
  int i, j;

  for (i = 0; i < N; i++)
    Arr[i] = i + 1;

  Go(0);
}

/* End of 'T03PERM' file */

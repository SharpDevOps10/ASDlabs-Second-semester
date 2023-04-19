#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
double ** createMatrix (int n) {
  double** matrix = (double**) malloc(sizeof(double*) * n);
  for (int i = 0; i < n; i++) {
    matrix[i] = (double*) malloc(sizeof(double) * n);
  }
  return matrix;
}
double** randm (int n) {
  srand(2220);
  double** matrix = createMatrix(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <n; j++) {
      matrix[i][j] = (double)(rand() *2.0)/ (double)RAND_MAX;
    }
  }
  return matrix;

}
double **mulmr (double coef, double** matrix, int n) {
  for (int i = 0; i <n; i++) {
    for (int j = 0; j <n; j++) {
      matrix[i][j] *= coef;
      matrix[i][j] = matrix[i][j] < 1 ? 0 : 1;
    }
  }
  return matrix;
}


double** symmetricMatrix(int n) {
  double** symmetric = createMatrix(n);
  for (int i = 0; i <n; i++) {
    for (int j = 0; j <n; j++) {
      if (symmetric[i][j] == 1) symmetric[j][i] = 1;
    }
  }
  return symmetric;
}
void freeMatrix(double** matrix, int n) {
  for (int i = 0; i <n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

int main ()
{
  printf("Hello");
}
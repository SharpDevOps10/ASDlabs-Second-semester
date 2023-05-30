#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#define vertices 12
#include "props.h"


void typeMatrix(double **matrix) {
  const int number = vertices;
  for (int i = 0; i < number; i++) {
    for (int j = 0; j < number; j++) {
      printf("%.0lf  ", matrix[i][j]);
    }
    printf("\n");
  }
}


int main() {

  double **T = randm(vertices);
  double coefficient = 1.0 - 0.01 - 0.005 - 0.05;
  T = mulmr(coefficient, T, vertices);
  double** A = symmetricMatrix(T);
  double** R = randm(vertices);
  multiplyByCoefficientMatrix(R, A);
  double** Wt = roundm(R);

  double** matrixB = createBMatrix(Wt);
  double** matrixC = createCMatrix(matrixB);
  double** matrixD = createDMatrix(matrixB);
  double** matrixTr = createTrMatrix();
  Wt = createWtMatrix(Wt,matrixC, matrixD, matrixTr);

  double** W = symmetricMatrix(Wt);
  printf("\n\tAdjacency matrix\n");
  typeMatrix(A);
  printf("\n\tWeights matrix\n");
  typeMatrix(W);

}
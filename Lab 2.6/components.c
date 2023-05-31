#define vertices 12
#include <stdlib.h>
#include <stdio.h>
#include <math.h>




double **randm(int n) {
  srand(2220);
  double **matrix = (double **) malloc(sizeof(double *) * n);
  for (int i = 0; i < n; i++) {
    matrix[i] = (double *) malloc(sizeof(double) * n);
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = (double) (rand() * 2.0) / (double) RAND_MAX;
    }
  }
  return matrix;

}

double **mulmr(double coef, double **matrix, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] *= coef;
      matrix[i][j] = matrix[i][j] < 1 ? 0 : 1;
    }
  }
  return matrix;
}


void freeMatrix(double **matrix, int n) {
  for (int i = 0; i < n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

double** createMatrix(int n) {
  double** matrix = (double**)malloc(n * sizeof(double*));
  for (int i = 0; i < n; i++)
  {
    matrix[i] = (double*)malloc(n * sizeof(double));
  }
  return matrix;
}

double** roundm(double** matrix) {
  const int number = vertices;
  double** Wt = createMatrix(number);
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      Wt[i][j] = round(matrix[i][j]);
    }
  }
  return Wt;
}

void multiplyByCoefficientMatrix(double** matrixT, double** matrixA) {
  for (int i = 0; i < vertices; i++)
  {
    for (int j = 0; j < vertices; j++)
    {
      double coefficient = 100 * matrixA[i][j];
      matrixT[i][j] *= coefficient;
    }
  }
}

double** createBMatrix(double** matrix) {
  const int number = vertices;
  double** matrixB = createMatrix(number);
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      if (matrix[i][j] == 0) matrixB[i][j] = 0;
      else if (matrix[i][j] > 0) matrixB[i][j] = 1;
    }
  }
  return matrixB;
}

double** createCMatrix(double** matrix) {
  const int number = vertices;
  double** matrixC = createMatrix(number);
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      if(matrix[i][j] != matrix[j][i]) matrixC[i][j] = 1;
      else matrixC[i][j] = 0;
    }
  }
  return matrixC;
}

double** createDMatrix(double** matrix) {
  const int number = vertices;
  double** matrixD = createMatrix(number);
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      if (matrix[i][j] == matrix[j][i] && matrix[i][j] == 1) matrixD[i][j] = 1;
      else matrixD[i][j] = 0;
    }
  }
  return matrixD;
}

double** createTrMatrix() {
  const int number = vertices;
  double** matrixTr = createMatrix(number);
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      if (j > i) matrixTr[i][j] = 1;
      else matrixTr[i][j] = 0;
    }
  }
  return matrixTr;
}

double** createWtMatrix(double** Wt, double** matrixC, double** matrixD, double** matrixTr) {
  const int number = vertices;
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      Wt[i][j] = Wt[i][j] * (matrixC[i][j] + (matrixD[i][j] * matrixTr[i][j]));
    }
  }
  return Wt;
}


double **symmetricMatrix(double **matrix) {
  const int number = vertices;
  double **symmetricMatrix = createMatrix(number);
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      if (matrix[i][j] != matrix[j][i]) {
        double symmetricValue = (matrix[i][j] == 0) ? matrix[j][i] : matrix[i][j];
        symmetricMatrix[i][j] = symmetricValue;
        symmetricMatrix[j][i] = symmetricValue;
      } else {
        symmetricMatrix[i][j] = matrix[i][j];
      }
    }

  }
  return symmetricMatrix;
}

int countAmountOfEdges(double** matrix) {
  const int number = vertices;
  int counter = 0;
  for (int i = 0; i < number; ++i) {
    for (int j = i; j < number; ++j) {
      if (matrix[i][j] == 1) counter++;
    }
  }
  return counter;
}

void getEdge(int index, int number, double** adjacencyMatrix,double** weightMatrix, int* source, int* terminus, double* weight) {
  int edgeCounter = -1;
  for (int i = 0; i < number; ++i) {
    for (int j = i; j < number; ++j) {
      if (edgeCounter != index) {
        if (adjacencyMatrix[i][j] == 1) {
          edgeCounter++;
          (*source) = i;
          (*terminus) = j;
          (*weight) = weightMatrix[i][j];
        }
      }

    }
  }

}

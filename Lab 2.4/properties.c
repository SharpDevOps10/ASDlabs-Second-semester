#define vertices 12
#include <stdlib.h>
#include <stdio.h>

int* graphDegrees(double** matrix) {
  const int number = vertices;
  int* vertexDegree;
  vertexDegree = malloc(number * sizeof(int));
  int vertexDegreeCounter;
  for (int i = 0; i < number; ++i) {
    vertexDegreeCounter = 0;
    for (int j = 0; j < number; ++j) {
      if(matrix[i][j] && i == j) vertexDegreeCounter +=2;
      else vertexDegreeCounter++;
    }
    vertexDegree[i] = vertexDegreeCounter;
  }
  return vertexDegree;
}

void freeMatrix(double **matrix, int n) {
  for (int i = 0; i < n; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

int* halfDegreeEntry(double** matrix) {
  const int number = vertices;
  int* vertexDegree;
  vertexDegree = malloc(number * sizeof(int));
  int vertexDegreeCounter;
  for (int j = 0; j < number; ++j) {
    vertexDegreeCounter = 0;
    for (int i = 0; i < number; ++i) {
      if(matrix[i][j] ) vertexDegreeCounter++;
    }
    vertexDegree[j] = vertexDegreeCounter;
  }
  return vertexDegree;
}

int* halfDegreeExit(double** matrix) {
  const int number = vertices;
  int* vertexDegree;
  vertexDegree = malloc(number * sizeof(int));
  int vertexDegreeCounter;
  for (int i = 0; i < number; ++i) {
    vertexDegreeCounter = 0;
    for (int j = 0; j < number; ++j) {
      if(matrix[i][j]) vertexDegreeCounter++;
    }
    vertexDegree[i] = vertexDegreeCounter;
  }
  return vertexDegree;
}

int* calculateTotalDegrees(const int* exit, const int* entry) {
  const int number = vertices;
  int* vertexDegree;
  vertexDegree = malloc(number * sizeof(int));
  for (int i = 0; i < number; ++i) {
    vertexDegree[i] = exit[i] + entry[i];
  }

  return vertexDegree;
}

int* findTerminalVertices(const int* degreesArray) {
  const int number = vertices;
  int* terminalVertices = calloc(number, sizeof(int));
  int position = 0;
  terminalVertices[position++] = 0;
  for (int i = 0; i < number; ++i) {
    if(degreesArray[i] == 1) terminalVertices[position++] = i + 1;
  }
  return terminalVertices;
}

int* findIsolatedVertices(const int* degreesArray) {
  const int number = vertices;
  int* isolatedVertices = calloc(number, sizeof(int));
  int position = 0;
  isolatedVertices[position++] = 0;

  for (int i = 0; i < number; ++i) {
    if(degreesArray[i] == 0) isolatedVertices[position++] = i + 1;
  }

  return isolatedVertices;
}

int checkHomogeneity(const int* degreesArray) {
  const int number = vertices;
  int firstDegree = degreesArray[0];
  for (int i = 1; i < number; ++i) {
    if(degreesArray[i] != firstDegree) return 0;
    firstDegree = degreesArray[i];
  }
  return 1;
}

double** summarizeMatrices(double** AMatrix, double** BMatrix) {
  const int number = vertices;
  double **summedMatrix = (double **) malloc(sizeof(double *) * number);
  for (int i = 0; i < number; i++) {
    summedMatrix[i] = (double *) malloc(sizeof(double) * number);
    for (int j = 0; j < number; ++j) {
      summedMatrix[i][j] = AMatrix[i][j] + BMatrix[i][j];
    }
  }
  return summedMatrix;
}

double** multiplyMatrices(double** AMatrix, double** BMatrix) {
  const int number = vertices;
  double **multipliedMatrix = (double **) malloc(sizeof(double *) * number);
  for (int i = 0; i < number; ++i) {
    multipliedMatrix[i] = (double *) malloc(sizeof(double) * number);
    for (int j = 0; j < number; ++j) {
      multipliedMatrix[i][j] = 0;
      for (int e = 0; e < number; ++e) {
        multipliedMatrix[i][j] += AMatrix[i][e] * BMatrix[e][j];
      }
    }
  }
  return multipliedMatrix;
}

double** copyMatrix(double** matrix) {
  const int number = vertices;
  double **copiedMatrix = (double **) malloc(sizeof(double *) * number);
  for (int i = 0; i < number; ++i) {
    copiedMatrix[i] = (double *) malloc(sizeof(double) * number);
    for (int j = 0; j < number; ++j) {
      copiedMatrix[i][j] = matrix[i][j];
    }
  }
  return copiedMatrix;
}


void booleanConversion(double** matrix) {
  const int number = vertices;
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < number; ++j) {
      if (matrix[i][j]) matrix[i][j] = 1;
    }
  }
}



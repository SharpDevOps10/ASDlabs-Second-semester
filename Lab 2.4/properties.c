#define vertices 12
#include <stdlib.h>
#include <stdio.h>

int* graphDegrees(double** matrix) {
  int number = vertices;
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

int* halfDegreeEntry(double** matrix) {
  int number = vertices;
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
  int number = vertices;
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

int* calculateTotalDegrees(int* exit, int* entry) {
  int number = vertices;
  int* vertexDegree;
  vertexDegree = malloc(number * sizeof(int));
  for (int i = 0; i < number; ++i) {
    vertexDegree[i] = exit[i] + entry[i];
  }

  return vertexDegree;
}

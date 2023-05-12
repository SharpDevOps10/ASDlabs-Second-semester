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
#include <stdio.h>
#include <stdlib.h>
#define vertices 12

void typeMatrix(double **matrix) {
  const int number = vertices;
  for (int i = 0; i < number; i++) {
    for (int j = 0; j < number; j++) {
      printf("%.0lf  ", matrix[i][j]);
    }
    printf("\n");
  }
}


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

double** createMatrix(int n) {
  double **matrix = (double **) malloc(n * sizeof(double *));
  for (int i = 0; i < n; i++) {
    matrix[i] = (double *) malloc(n * sizeof(double));
  }
  return matrix;
}

void chargeWithZero(double** matrix, int n)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      matrix[i][j] = 0.;
    }
  }
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


void depthFirstSearch(double** adjacencyMatrix, int currentVertex, int* visited, int* depthVertices, double** tree, int* numVisited) {
  const int number = vertices;
  visited[currentVertex] = 1;
  depthVertices[(*numVisited)] = currentVertex;
  (*numVisited)++;

  for (int neighborVertex = 0; neighborVertex < number; ++neighborVertex) {
    if (adjacencyMatrix[currentVertex][neighborVertex] == 1 && !visited[neighborVertex]) {

      tree[currentVertex][neighborVertex] = 1;
      depthFirstSearch(adjacencyMatrix, neighborVertex, visited, depthVertices, tree, numVisited);

    }
  }
}

void breadthFirstSearch(double** adjacencyMatrix, int startVertex, int* queue, double** tree) {
  const int number = vertices;
  int visited[number];
  for (int i = 0; i < number; i++)
  {
    visited[i] = 0;
  }
  int queueStart = 0;
  int queueFinish = -1;

  queue[++queueFinish] = startVertex;
  visited[startVertex] = 1;

  while (queueStart <= queueFinish) {
    int currentVertex = queue[queueStart++];
    for (int neighborVertex = 0; neighborVertex < number; neighborVertex++) {
      if (adjacencyMatrix[currentVertex][neighborVertex] == 1 && visited[neighborVertex] == 0)
      {
        tree[currentVertex][neighborVertex] = 1;
        queue[++queueFinish] = neighborVertex;
        visited[neighborVertex] = 1;
      }
    }
  }
}

int findFirstArch(double** matrix, int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      double forwardArch = matrix[i][j];
      double backwardArch = matrix[j][i];
      if (forwardArch == 1 && backwardArch == 1) {
        return i;
      }
    }
  }
  return -1;

}

void constructSearchMatrix(const double** graph, int sourceVertex, double** searchMatrix) {
  const int number = vertices;

  for (int i = 0; i < number; ++i) {
    if (graph[i][sourceVertex]) searchMatrix[i][sourceVertex] = 1;
  }
}

double** createTraversalMatrix(const int* arr) {


  double** traversalMatrix = createMatrix(vertices);
  chargeWithZero(traversalMatrix, vertices);


  for (int i = 0; i < vertices; i++)
  {
    traversalMatrix[arr[i]][i] = 1.0;
  }

  return traversalMatrix;
}

void runDfsForNotVisitedVertices(int currentVertex, double** adjacencyMatrix,
                                 int* visited, int amount, int* depthVertices, double** graph ) {

  const int number = vertices;

  for (int i = 0; i < number; ++i) {
    visited[i] = 0;
  }

  for (int i = 0; i < number; ++i) {
    if (visited[i] == 0) {
      depthFirstSearch(adjacencyMatrix, currentVertex, visited, depthVertices,graph, &amount);
    }

  }

}
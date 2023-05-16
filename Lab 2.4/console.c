#include <stdio.h>
#include <stdlib.h>
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

void printDegrees(int *degrees) {
  const int number = vertices;
  printf("{  ");
  for (int i = 0; i < number; i++) {
    printf("%d  ", degrees[i]);
  }
  printf("}\n");
}

void printVertices(int* verticesNumber) {
  printf("{ ");
  for (int i = 0; verticesNumber[i] != 0; ++i) {
    printf("%d ", verticesNumber[i]);
  }
  printf("}\n");
}

void printPathways(double** matrix) {
  const int numbers = vertices;
  for (int i = 0; i < numbers; i++) {
    for (int j = 0; j < numbers; j++) {
      if (*(*(matrix + i) + j) != 0) {
        printf("%d to %d;  ", i + 1, j + 1);
      }
    }
    printf("\n");
  }
}

void printComponents(double **matrix, int number) {
  number = vertices;
  int numComponents = 1;
  for (int i = 0; i < number; i++) {
    int isNewGroup = 1;
    printf("Component %d: { ", numComponents);
    for (int j = 0; j < number; j++) {
      if (matrix[i][j]) {
        if (!isNewGroup) {
          printf(", ");
        }
        printf("%d", j + 1);
        isNewGroup = 0;
      }
    }
    if (!isNewGroup) {
      printf(" }\n");
      numComponents++;
    }
  }
}


void aboutDirectedGraph() {

  double **T = randm(vertices);
  double coefficient = 1.0 - 0.01 - 0.01 - 0.3;
  double **A = mulmr(coefficient, T, vertices);

  int* entry = halfDegreeEntry(A);
  int* exit = halfDegreeExit(A);
  int* summedDegrees = summarizeHalfDegrees(exit, entry);

  int* isolated = findIsolatedVertices(summedDegrees);
  int* terminal = findTerminalVertices(summedDegrees);



  printf("\nThis is a Directed Graph \n");
  printf("\n\tInitial matrix\n");
  typeMatrix(A);


  printf("Exit degree : ");
  printDegrees(exit);

  printf("Entry degree : ");
  printDegrees(entry);

  printf("\nIs the graph homogeneous?\n");
  if(checkHomogeneity(summedDegrees)) {
    printf("%d\n", summedDegrees[0]);
  } else {
    printf("\tThe graph is not homogeneous ");
  }

  printf("\nFind isolated vertices\n");
  printf("Isolated vertices : ");
  if(isolated[0]) {
    printVertices(isolated);
  } else {
    printf("\tNo isolated vertices");
  }

  printf("\nFind terminal vertices\n");
  printf("Terminal vertices : ");
  if(terminal[0]) {
    printVertices(isolated);
  } else {
    printf("\tNo terminal vertices");
  }
  freeMatrix(A, vertices);

  free(entry);
  free(exit);

  free(summedDegrees);

  free(isolated);
  free(terminal);

}

void aboutUndirectedGraph() {
  double coefficient = 1.0 - 0.01 - 0.01 - 0.3;
  double **R = randm(vertices);
  double **C = symmetricMatrix(mulmr(coefficient, R, vertices), vertices);
  int* degree = graphDegrees(C);

  int* isolated = findIsolatedVertices(degree);
  int* terminal = findTerminalVertices(degree);

  printf("\nThis is a Undirected Graph \n");
  printf("\n\tMatrix for Undirected Graph\n");
  typeMatrix(C);

  printf("Undirected graph degrees : ");
  printDegrees(degree);

  printf("\nIs the graph homogeneous?\n");
  if(checkHomogeneity(degree)) {
    printf("%d\n", degree[0]);
  } else {
    printf("\tThe graph is not homogeneous ");
  }

  printf("\nFind isolated vertices\n");
  printf("Isolated vertices : ");
  if(isolated[0]) {
    printVertices(isolated);
  } else {
    printf("\tNo isolated vertices");
  }

  printf("\nFind terminal vertices\n");
  printf("Terminal vertices : ");
  if(terminal[0]) {
    printVertices(isolated);
  } else {
    printf("\tNo terminal vertices");
  }
  freeMatrix(C, vertices);

  free(degree);
  free(isolated);
  free(terminal);

}

void aboutModifiedGraph() {
  double** K = randm(vertices);
  double modifiedCoefficient = 1.0 - 0.005 - 0.005 - 0.27;
  double** D = mulmr(modifiedCoefficient, K, vertices);
  int* entry = halfDegreeEntry(D);
  int* exit = halfDegreeExit(D);
  int* summedDegrees = summarizeHalfDegrees(exit, entry);
  double** squaredMatrix = multiplyMatrices(D,D);
  double** cubedMatrix = multiplyMatrices(squaredMatrix, D);

  int* isolated = findIsolatedVertices(summedDegrees);
  int* terminal = findTerminalVertices(summedDegrees);

  double** reachabilityMatrix = calculateReachabilityMatrix(D);
  double** connectivityMatrix = strongConnectivityMatrix(reachabilityMatrix);

  double** strongComponents = findStrongComponents(connectivityMatrix);
  printf("\nThis is a Modified Graph \n");
  printf("\n\tMatrix for Modified Graph\n");
  typeMatrix(D);

  printf("Exit degree : ");
  printDegrees(exit);

  printf("Entry degree : ");
  printDegrees(entry);

  printf("\nIs the graph homogeneous?\n");
  if(checkHomogeneity(summedDegrees)) {
    printf("%d\n", summedDegrees[0]);
  } else {
    printf("\tThe graph is not homogeneous ");
  }

  printf("\nFind isolated vertices\n");
  printf("Isolated vertices : ");
  if(isolated[0]) {
    printVertices(isolated);
  } else {
    printf("\tNo isolated vertices");
  }

  printf("\nFind terminal vertices\n");
  printf("Terminal vertices : ");
  if(terminal[0]) {
    printVertices(isolated);
  } else {
    printf("\tNo terminal vertices");
  } //Paths of length 2

  printf("\nMatrix squared : 2\n");
  typeMatrix(squaredMatrix);

  printf("\nPathways with length : 2\n");
  printPathways(squaredMatrix);

  printf("\nMatrix cubed : 3\n");
  typeMatrix(cubedMatrix);

  printf("\nPathways with length : 3\n");
  printPathways(cubedMatrix);

  printf("\nReachability Matrix of Mod graph\n");
  typeMatrix(reachabilityMatrix);

  printf("\nConnected Matrix of Mod graph\n");
  typeMatrix(connectivityMatrix);

  printf("\nStrongly Connected Components of Mod Graph\n");
  printComponents(strongComponents,vertices);

  printf("\n**Condensation Graph Adjacency Matrix**\n");
  condensationMatrix(strongComponents);

  freeMatrix(D,vertices);
  freeMatrix(squaredMatrix, vertices);
  freeMatrix(cubedMatrix, vertices);
  freeMatrix(reachabilityMatrix, vertices);
  freeMatrix(connectivityMatrix, vertices);
  freeMatrix(strongComponents, vertices);


  free(isolated);
  free(terminal);
  free(summedDegrees);
  free(entry);
  free(exit);
}


int main() {
  aboutDirectedGraph();
  //aboutModifiedGraph();
  //aboutUndirectedGraph();
}








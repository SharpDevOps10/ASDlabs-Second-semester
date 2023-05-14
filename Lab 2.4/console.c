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
  printf("\nInitial matrix\n");
  typeMatrix(A);


  printf("Exit degree : ");
  printDegrees(exit);

  printf("Entry degree : ");
  printDegrees(entry);

  printf("\nIs the graph homogeneous?\n");
  if(checkHomogeneity(summedDegrees)) {
    printf("%d\n", summedDegrees[0]);
  } else {
    printf("The graph is not homogeneous ");
  }

  printf("\nFind isolated vertices\n");
  printf("Isolated vertices : ");
  if(isolated[0]) {
    printVertices(isolated);
  } else {
    printf("No isolated vertices");
  }

  printf("\nFind terminal vertices\n");
  printf("Terminal vertices : ");
  if(terminal[0]) {
    printVertices(isolated);
  } else {
    printf("No terminal vertices");
  }
  freeMatrix(A, vertices);

  free(entry);
  free(exit);

  free(summedDegrees);

  free(isolated);
  free(terminal);

}

int main() {
  aboutDirectedGraph();
}








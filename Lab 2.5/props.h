

#ifndef LAB_2_5_PROPS_H
#define LAB_2_5_PROPS_H
void typeMatrix(double **matrix);
double **randm(int n);
double **mulmr(double coef, double **matrix, int n);
void freeMatrix(double **matrix, int n);
void depthFirstSearch(double** adjacencyMatrix, int currentVertex, int* visited, int* depthVertices, double** tree, int* numVisited);
void breadthFirstSearch(double** adjacencyMatrix, int startVertex, int* queue, double** tree);
int findStartVertex(double** matrix, int n);
void constructSearchMatrix(double** graph, int sourceVertex, double** searchMatrix);
double** createTraversalMatrix(const int* arr);
void runDfsForNotVisitedVertices(int currentVertex, double** adjacencyMatrix,
                                 int* visited, int amount, int* depthVertices, double** graph );
double** createMatrix(int n);
void chargeWithZero(double** matrix, int n);
#endif

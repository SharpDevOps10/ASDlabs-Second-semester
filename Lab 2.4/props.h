

#ifndef LAB_2_4_PROPS_H
#define LAB_2_4_PROPS_H

double **randm(int n);
double **mulmr(double coef, double **matrix, int n);
int* halfDegreeEntry(double** matrix);
int* halfDegreeExit(double** matrix);
int* summarizeHalfDegrees(const int* exit, const int* entry);
int* findIsolatedVertices(const int* degreesArray);
int* findTerminalVertices(const int* degreesArray);
int checkHomogeneity(const int* degreesArray);
void freeMatrix(double **matrix, int n);
double **symmetricMatrix(double **matrix, int n);
int* graphDegrees(double** matrix);
#endif

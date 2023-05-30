

#ifndef LAB_2_6_PROPS_H
#define LAB_2_6_PROPS_H

double **randm(int n);
double **mulmr(double coef, double **matrix, int n);
void freeMatrix(double **matrix, int n);
double** createMatrix(int n);
double** roundm(double** matrix);
double** createBMatrix(double** matrix);
double** createCMatrix(double** matrix);
double** createDMatrix(double** matrix);
double** createTrMatrix();
double** createWtMatrix(double** Wt, double** matrixC, double** matrixD, double** matrixTr);
double **symmetricMatrix(double **matrix);
void multiplyByCoefficientMatrix(double** matrixT, double** matrixA);
#endif
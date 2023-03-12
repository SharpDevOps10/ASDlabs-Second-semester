#include <stdio.h>
double multiply (int i, double x) {
    return x*x*(2*i-1)/(2*i+1);
}
double recSummation (int n, double *sum,double x) {
    if (n == 1) {
        *sum += x;
        return x;
    }
    double item = recSummation(n-1,sum,x) * multiply(n-1,x);
    *sum += item;
    return item;
}
double summation (double parameter, int amount, double *sum) {
    *sum = 0;
    return recSummation(amount,sum,parameter);
}
int main() {
    double argument = -1;
    while (argument >=1 || argument<= -1) {
        printf("Please, write down the value of x , Hint : |x| < 1\n");
        scanf("%lf" , &argument);
    }
    unsigned int number;
    printf("Please, write down the value of n\n");
    scanf("%d" , &number);
    double sum;
    summation(argument,number,&sum);
    printf("The second result  is %.30lf", sum);
    return 0;
}

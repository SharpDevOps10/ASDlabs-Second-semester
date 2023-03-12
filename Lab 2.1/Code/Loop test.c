#include <stdio.h>
double multiplier (int i,double x) {
    return x*x*(2*i-1)/(2*i+1);
}
double recSummation (double x, int amount) {
    double sum = x;
    double item = x;
    for (int i = 2; i <= amount ; i++) {
        item *= multiplier(i-1,x);
        sum += item;
    }
    return sum;
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
    double sum = recSummation(argument,number);
    printf("The test result is %.30lf", sum);
    return 0;
}

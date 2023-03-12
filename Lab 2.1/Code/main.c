#include <stdio.h>
double multiplier (int i, double x) {
    return x*x*(2*i-1)/(2*i+1);
}
double recSummation (double previous,int present, double sum,double x,int n) {
    double item;
    if (present == 1) {
        item = x;
    } else {
        item = previous*multiplier(present-1,x);
    }
    sum += item;
    if (present == n) {
        return sum;
    } else {
        return recSummation (item,present+1,sum,x,n);
    }
}
double provideRecurtion (double parameter, int amount) {
    return recSummation(0,1,0,parameter,amount);
}
int main() {
    double argument = -1;
    while (argument >=1 || argument<= -1) {
        printf("Please, write down the value of x , Hint : |x| < 1\n");
        scanf("%lf" , &argument);
    }
    int number;
    printf("Please, write down the value of n\n");
    scanf("%d" , &number);
    double sum = provideRecurtion(argument,number);
    printf("The first result is %.30lf", sum);
    return 0;
}

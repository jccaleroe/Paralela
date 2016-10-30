#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
	double sum = 0;
	long i, n = 1e9;
	for(i = 0; i < n; i++)
		sum += pow(-1, i) * 4 / (i*2+1);
	printf("Pi = %0.9lf\n", sum);
}

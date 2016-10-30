#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//gcc piprocess.c -o piprocess

double piPartial[2];
long n = 1e9;

void calculatePi(int id, int total){
	long i;
	double x=0;
	for(i = id * (n/total); i < (id+1)*(n/total); i++)
		x += pow(-1,i)*(4.0/(2*i+1));
	piPartial[0] = x;
}

int main(){
	double piTotal;
	pid_t pid;
	int pipefd[2];
	int r = pipe(pipefd);
	pid = fork();
	if(pid == 0){
		calculatePi(1,2);
		close(pipefd[0]);
		write(pipefd[1], &piPartial[0], sizeof(double));
		open(pipefd[0]);
	}else{
		close(pipefd[1]);
		calculatePi(0,2);
		read(pipefd[0], &piPartial[1], sizeof(double));
		open(pipefd[1]);
	}
	piTotal = piPartial[0] + piPartial[1];
	close(pipefd);
	if(pid != 0)
		printf("Pi = %0.9lf\n",piTotal);
}

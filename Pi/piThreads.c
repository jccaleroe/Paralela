#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<math.h>

long n = 1e9;
double total = 0;
int threads_num = 6;
pthread_mutex_t a_lock = PTHREAD_MUTEX_INITIALIZER;


void error(char *msg){
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

void* sum(void *tNo){
 	long t_no = (long) tNo;
	long a = t_no * n / threads_num + 1;
	long b = (t_no+1) * n / threads_num;
	double tmp = 0;
	long i;
	for(i = a; i <= b; i++)
		tmp += pow(-1, i-1)*4 / ((i-1)*2+1);

	pthread_mutex_lock(&a_lock);
	total += tmp;
	pthread_mutex_unlock(&a_lock);
	return NULL;
}

int main(){
    pthread_t threads[threads_num];
    long i;
    for(i = 0; i < threads_num; i++)
	if(pthread_create(&threads[i], NULL, sum, (void*)i)  == -1)
		error("Can't create thread");
    
    void* result;
    for(i = 0; i < threads_num; i++)
	if(pthread_join(threads[i], &result) == -1)
		error("Can't join thread");
    
    printf("Pi = %0.9lf\n", total);
}

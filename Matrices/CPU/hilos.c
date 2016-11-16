#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

int N, threads_num;
float **A, **B, **C, max_num = (float)(RAND_MAX);

void error( char *msg ){
    fprintf( stderr, "%s: %s\n", msg, strerror(errno) );
    exit(1);
}

void* multiply(void *tNo){
 	long t_no = (long) tNo;
 	int i, j, k, pos;
 	float tmp;
 	int aux = N * N / threads_num;

 	long a = aux * t_no;

 	long b = aux * (t_no + 1);

 	if (t_no == threads_num - 1)
        b = N * N;

    for( pos = a; pos < b; pos++ ){
        i = pos / N;
        j = pos - i*N;
        tmp = 0;
        for(k = 0; k < N; k++)
            tmp += A[i][k] * B[k][j];
        C[i][j] = tmp;
    }
}

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Please enter matrix size N\n");
        scanf("%d", &N);
        printf("Please enter number of threads\n");
        scanf("%d", &threads_num);
    }
    else{
        N = atoi(argv[1]);
        threads_num = atoi(argv[2]);
    }

    float *m[N], *m2[N], *m3[N];
    int i, j;

    for( i = 0; i < N; i++){
        m[i] = (float *) malloc ( N * sizeof(float) );
        m2[i] = (float *) malloc ( N * sizeof(float) );
        m3[i] = (float *) malloc ( N * sizeof(float) );
    }

    A = m;
    B = m2;
    C = m3;

    srand(time(NULL));
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i][j] = max_num / (float) (rand());
            B[i][j] = max_num / (float) (rand());
        }
    }

//    test case
//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++){
//            A[i][j] = i*N+j;
//            B[i][j] = i*N+j;
//        }
//    }

    pthread_t threads[threads_num];

    long n;
    for( n = 0; n < threads_num; n++)
        if( pthread_create( &threads[n], NULL, multiply, (void*) n )  == -1 )
            error("Can't create thread");


    void* result;
    for( n = 0; n < threads_num; n++ )
        if(pthread_join( threads[n], &result ) == -1)
            error("Can't join thread");

//    print matrix
//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++)
//          printf("%0.1f ", C[i][j]);
//        printf("\n");
//    }

    for( i = 0; i < N; i++){
        free(m[i]);
        free(m2[i]);
        free(m3[i]);
    }
}


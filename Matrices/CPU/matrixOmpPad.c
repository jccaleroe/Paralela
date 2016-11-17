#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"
#define PAD 4

int N, threads_num;
float *A, *B, *C, max_num = (float)(RAND_MAX);

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

	double t1 = omp_get_wtime();

	int f = sizeof(float);
	int size = N * N * f, i, j, k;

	A = (float *) malloc ( size );
	B = (float *) malloc ( size );
	C = (float *) malloc ( size * PAD );

    srand(time(NULL));
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i*N + j] = max_num / (float) (rand());
            B[i*N + j] = max_num / (float) (rand());
        }
    }
	
	omp_set_num_threads( threads_num );
	#pragma omp parallel for collapse(3)
		for ( i = 0; i < N; i++ ){
			for ( j = 0; j < N; j++ ){
				for ( k = 0; k < N; k++ ){
					C[i*N+j+f*PAD] += A[i*N+k] * B[k*N+j];
				}
			}
		}

    free(A); free(B); free(C);

	printf("%lf\n",	omp_get_wtime() - t1);
}

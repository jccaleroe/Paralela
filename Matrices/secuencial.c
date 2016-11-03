#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int N;
float **A, **B, **C, aux = (float)(RAND_MAX);

void multiply(){

    int i, j, k;
    float tmp;

    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            tmp = 0;
            for(k = 0; k < N; k++)
                tmp += A[i][k] * B[k][j];
            C[i][j] = tmp;
        }
    }
}

int main(){
    printf("Please enter matrix size N\n");
    scanf("%d", &N);

    float *m[N], *m2[N], *m3[N];
    int i, j;

    for( i = 0; i < N; i++){
        m[i] = (float *) malloc ( N * sizeof(float) );
        m2[i] = (float *) malloc ( N * sizeof(float) );
        m3[i] = (float *) malloc ( N * sizeof(float) );
    }

    A = m; B = m2; C = m3;

    srand(time(NULL));
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i][j] = aux/(float)(rand());
            B[i][j] = aux/(float)(rand());
        }
    }

    multiply();

//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++)
//          printf("%0.1f ", C[i][j]);
//        printf("\n");
//    }
}

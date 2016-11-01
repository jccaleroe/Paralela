#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){

    int N;

    printf("Please enter matrix size N\n");
    scanf("%d", &N);

    float A[N][N], B[N][N], C[N][N], max_rand = (float)(RAND_MAX), tmp;
    int i, j, k;

    srand(time(NULL));
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i][j] = max_rand / (float)(rand());
            B[i][j] = max_rand / (float)(rand());
        }
    }

    clock_t begin, end;
    begin = clock();

    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            tmp = 0;
            for(k = 0; k < N; k++)
                tmp += A[i][k] * B[k][j];
            C[i][j] = tmp;
        }
    }


    end = clock();
    double time_spent = (double) (end-begin) / CLOCKS_PER_SEC;


//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++)
//          printf("%0.1f ", C[i][j]);
//        printf("\n");
//    }

    printf("Time spent: %0.4lf seconds\n", time_spent);
}

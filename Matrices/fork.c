#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

int main(){

    int N, processes_num, processes_id;
    int a, b, tam;

    printf("Please enter matrix size N\n");
    scanf("%d", &N);
    printf("Please enter number of processes\n");
    scanf("%d", &processes_num);

    float A[N][N], B[N][N], max_num = (float)(RAND_MAX);

    float *m[N], *m2[N];
    int i, j, pos;

    srand(time(NULL));
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i][j] = max_num / (float)(rand());
            B[i][j] = max_num / (float)(rand());
        }
    }

    //test case
//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++){
//            A[i][j] = i*N+j;
//            B[i][j] = i*N+j;
//        }
//    }

    clock_t begin, end;
    begin = clock();

    int pipes[processes_num - 1][2];
    int cells_per_process = N * N / processes_num;

    pid_t pid;

    for ( i = 0; i < processes_num - 1; i++ ){
        if( pipe( pipes[i] ) == -1){
            error("Can't create pipe");
            return -1;
        }

        a = cells_per_process * (i+1);
        b = cells_per_process * (i + 2);
        processes_id = i;

        if (processes_id == processes_num - 2)
            b = N * N;

        tam = b - a;
        pid = fork();

        if( pid == -1){
            error("Can't fork");
            return -1;
        }

        if ( !pid )
            break;
    }

    if( !pid && processes_num > 1){

        int k, pos;
        float numbers[tam], tmp;

        for(pos = a; pos < b; pos++ ){
            i = pos / N;
            j = pos - i * N;
            tmp = 0;
            for(k = 0; k < N; k++)
                tmp += A[i][k] * B[k][j];
            numbers[pos-a] = tmp;
        }

        close(pipes[processes_id][0]);

		write(pipes[processes_id][1], &a, sizeof(int));
		write(pipes[processes_id][1], &b, sizeof(int));

		write(pipes[processes_id][1], numbers, tam * sizeof(float));

    }
    else{

        printf("\n");
        float C[N][N];
        int i2;

        for ( i = 0; i < processes_num - 1; i++ ){

            close(pipes[i][1]);

            read(pipes[i][0], &a, sizeof(int));
            read(pipes[i][0], &b, sizeof(int));

            tam = b - a;

            float tmp[tam];
            read(pipes[i][0], tmp, tam * sizeof(float));

            for( pos = a; pos < b; pos++ ){
                i2 = pos / N;
                j = pos - i2*N;
                C[i2][j] = tmp[pos-a];
            }
        }

        a = 0;
        b = cells_per_process;
        int k, tmp;

        for( pos = a; pos < b; pos++ ){
            i = pos / N;
            j = pos - i*N;
            tmp = 0;
            for(k = 0; k < N; k++)
                tmp += A[i][k] * B[k][j];
            C[i][j] = tmp;
        }

        end = clock();
        double time_spent = (double) (end-begin) / CLOCKS_PER_SEC;

//        for( i = 0; i < N; i++){
//            for( j = 0; j < N; j++)
//                printf("%0.1f ", C[i][j]);
//            printf("\n");
//        }

        printf("Time spent: %0.4lf seconds\n", time_spent);
    }
}


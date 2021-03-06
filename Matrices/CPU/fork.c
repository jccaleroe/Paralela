#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int N, processes_num, processes_id, a, b, tam;
float **A, **B, max_num = (float)(RAND_MAX);

void error( char *msg ){
    fprintf( stderr, "%s: %s\n", msg, strerror(errno) );
    exit(1);
}

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Please enter matrix size N\n");
        scanf("%d", &N);
        printf("Please enter number of processes\n");
        scanf("%d", &processes_num);
    }
    else{
        N = atoi(argv[1]);
        processes_num = atoi(argv[2]);
    }

    float *m[N], *m2[N];
    int i, j, pos;

    for( i = 0; i < N; i++){
        m[i] = (float *) malloc ( N * sizeof(float) );
        m2[i] = (float *) malloc ( N * sizeof(float) );
    }

    A = m; B = m2;

    srand(time(NULL));
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i][j] = max_num / (float)(rand());
            B[i][j] = max_num / (float)(rand());
        }
    }

//    test case
//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++){
//            A[i][j] = i*N+j;
//            B[i][j] = i*N+j;
//        }
//    }

    int pipes[processes_num - 1][2];
    int cells_per_process = N * N / processes_num;

    pid_t pid;

    for ( i = 0; i < processes_num - 1; i++ ){
        if( pipe( pipes[i] ) == -1)
            error("Can't create pipe");

        a = cells_per_process * (i+1);
        b = cells_per_process * (i + 2);
        processes_id = i;

        if (processes_id == processes_num - 2)
            b = N * N;

        tam = b - a;
        pid = fork();

        if( pid == -1)
            error("Can't fork");

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

//        for( i = 0; i < N; i++){
//            for( j = 0; j < N; j++)
//                printf("%0.1f ", C[i][j]);
//            printf("\n");
//        }
        for( i = 0; i < N; i++){
            free(m[i]);
            free(m2[i]);
        }
    }
}


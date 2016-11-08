#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <time.h>

int N, *D_N, threads_per_block, blocks_num, threads_num, *d_threads_num;
float *A, *B, *C, *D_A, *D_B, *D_C, max_num = (float)(RAND_MAX);

#define GPUerrchk(ans) { GPUassert((ans), __FILE__, __LINE__); }

__global__ void multiply( float *A, float *B, float *C, int N, int threads_num ){

 	float tmp;
	int index = (blockDim.x * blockIdx.x) + threadIdx.x;
 	int i, j, k, pos, cell_per_thread = N * N / threads_num;

	int a = cell_per_thread * index, b = cell_per_thread * (index + 1);

	if (index == threads_num - 1)
        b = N * N;

    for( pos = a; pos < b; pos++ ){
        i = pos / N;
        j = pos - i*N;
        tmp = 0;
        for(k = 0; k < N; k++)
            tmp += A[i*N + k] * B[k*N + j];
        C[i*N + j] = tmp;
    }
}

int main(int argc, char *agcv){
	if(argc != 4){
        printf("Please enter matrix size N\n");
        scanf("%d", &N);
        printf("Please enter number of blocks\n");
        scanf("%d", &blocks_num);
		printf("Please enter number of threads per block\n");
        scanf("%d", &threads_num);
    }
    else{
        N = atoi(argv[1]);
        blocks_num = atoi(argv[2]);
		threads_num = atoi(argv[3]);
    }
	
	int size = N * N * sizeof(float), i, j;

	A = (float *) malloc ( size );
	B = (float *) malloc ( size );
	C = (float *) malloc ( size );

//    srand(time(NULL));
//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++){
//            A[i*N + j] = max_num / (float) (rand());
//            B[i*N + j] = max_num / (float) (rand());
//        }
//    }

//    test case
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i*N + j] = i*N+j;
            B[i*N + j] = i*N+j;
        }
    }

	threads_per_block = threads_num / blocks_num;

    GPUerrchk( cudaMalloc ( (void **) &D_A, size ) );
	GPUerrchk( cudaMemcpy ( D_A, A, size, cudaMemcpyHostToDevice );

    GPUerrchk( cudaMalloc ( (void **) &D_B, size ) );
	GPUerrchk( cudaMemcpy ( D_B, B, size, cudaMemcpyHostToDevice );

    GPUerrchk( cudaMalloc ( (void **) &D_C, size ) );
	GPUerrchk( cudaMemcpy ( D_C, C, size, cudaMemcpyHostToDevice );

    GPUerrchk( cudaMalloc ( (void **) &D_N, sizeof(int) ) );
	GPUerrchk( cudaMemcpy ( D_N, &N, sizeof(int), cudaMemcpyHostToDevice );

    GPUerrchk( cudaMalloc ( (void **) &D_threads_num, sizeof(int) ) );
	GPUerrchk( cudaMemcpy ( D_threads_num, &threads_num, sizeof(int), cudaMemcpyHostToDevice );

    multiply<<<blocks_num, threads_per_block>>>(D_A, D_B, D_C, D_N, D_threads_num);
	
	GPUerrchk( cudaPeeKAtLastError() );
	
	GPUerrchk( cudaMemcpy(C, D_C, size, cudaMemcpyDeviceToHost) );

//  print matrix
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++)
          printf("%0.1f ", C[i*N +j]);
        printf("\n");
    }

    free(A); free(B); free(C);
	GPUerrchk ( cudaDeviceReset() );
}

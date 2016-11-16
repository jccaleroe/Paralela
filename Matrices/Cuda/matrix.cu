#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cuda_runtime.h>

#define GPUERRCHK(ans) { gpuAssert((ans), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t code, char *file, int line, bool abort=true)
{
   if (code != cudaSuccess)
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

int N, threads_per_block, blocks_num, threads_num;
float *A, *B, *C, *D_A, *D_B, *D_C, max_num = (float)(RAND_MAX);

__global__ void multiply( float *A2, float *B2, float *C, int N, int threads_num ){
	__shared__ float *A;
	__shared__ float *B;
	A = A2;	B = B2;

 	float tmp;
 	int k, pos;

	int a = N * N * (blockDim.x * blockIdx.x + threadIdx.x) / threads_num, b;

	if ( blockDim.x * blockIdx.x + threadIdx.x == threads_num - 1)
        b = N * N;
	else
		b = N * N * ( blockDim.x * blockIdx.x + threadIdx.x + 1) / threads_num;

    for( pos = a; pos < b; pos++ ){
        tmp = 0;
        for( k = 0; k < N; k++ )
            tmp += A[ N * (pos / N ) + k ] * B[ k * N + pos - ( pos / N) * N];
        C[ pos ] = tmp;
    }
}

int main(int argc, char *argv[]){
	if(argc != 4){
        printf("Please enter matrix size N\n");
        scanf("%d", &N);
        printf("Please enter number of blocks\n");
        scanf("%d", &blocks_num);
		printf("Please enter number of threads\n");
        scanf("%d", &threads_per_block);
    }
    else{
        N = atoi(argv[1]);
        blocks_num = atoi(argv[2]);
		threads_per_block = atoi(argv[3]);
    }
	
	cudaEvent_t start, stop;
	float elapsedTime;
	cudaEventCreate(&start);
	cudaEventRecord(start, 0);

	int size = N * N * sizeof(float), i, j;

	A = (float *) malloc ( size );
	B = (float *) malloc ( size );
	C = (float *) malloc ( size );

    srand(time(NULL));
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            A[i*N + j] = max_num / (float) (rand());
            B[i*N + j] = max_num / (float) (rand());
        }
    }

//   test case
//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++){
//            A[i*N + j] = i*N+j;
//            B[i*N + j] = i*N+j;
//        }
//    }

	threads_num	= threads_per_block * blocks_num;

    GPUERRCHK( cudaMalloc ( (void **) &D_A, size ) );
	GPUERRCHK( cudaMemcpy ( D_A, A, size, cudaMemcpyHostToDevice ) );

    GPUERRCHK( cudaMalloc ( (void **) &D_B, size ) );
	GPUERRCHK( cudaMemcpy ( D_B, B, size, cudaMemcpyHostToDevice ) );

    GPUERRCHK( cudaMalloc ( (void **) &D_C, size ) );

    multiply<<<blocks_num, threads_per_block>>>(D_A, D_B, D_C, N, threads_num);
	
	GPUERRCHK( cudaPeekAtLastError() );
	
	GPUERRCHK( cudaMemcpy(C, D_C, size, cudaMemcpyDeviceToHost) );

//    print matrix
//    for( i = 0; i < N; i++){
//        for( j = 0; j < N; j++)
//          printf("%0.1f ", C[i*N +j]);
//        printf("\n");
//    }

    free(A); free(B); free(C);

	cudaEventCreate(&stop);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);
	printf("Elapsed time: %f ms\n", elapsedTime);

	GPUERRCHK ( cudaDeviceReset() );
}

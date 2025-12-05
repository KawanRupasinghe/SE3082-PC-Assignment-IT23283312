
/*
------------------------------------------------------------
 File: midpoint_cuda.cu
 Purpose: Numerical Integration using the Midpoint Rule (CUDA)
 Domain: Numerical Computation & Scientific Computing
 Author: P W K W RUPASINGHE | Student ID: IT23283312
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to integrate
__device__ double f(double x)
{
    return sin(x) * exp(-x / 10.0);
}

// Kernel: each block computes a partial sum in shared memory
__global__ void midpoint_kernel(double a, double dx, long long n, double *block_sums)
{
    extern __shared__ double sdata[];

    int tid = threadIdx.x;
    long long idx = (long long)blockIdx.x * blockDim.x + tid;

    double value = 0.0;

    // Compute midpoint contribution for this thread
    if (idx < n)
        value = f(a + (idx + 0.5) * dx);

    sdata[tid] = value;
    __syncthreads();

    // Reduce values within block
    for (int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
            sdata[tid] += sdata[tid + s];

        __syncthreads();
    }

    // First thread writes block sum
    if (tid == 0)
        block_sums[blockIdx.x] = sdata[0];
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Usage: %s <a> <b> <n> <threads_per_block>\n", argv[0]);
        return 1;
    }

    double a = atof(argv[1]);     // start
    double b = atof(argv[2]);     // end
    long long n = atoll(argv[3]); // number of intervals
    int threadsPerBlock = atoi(argv[4]);

    double dx = (b - a) / (double)n;

    int blocks = (n + threadsPerBlock - 1) / threadsPerBlock;

    // Allocate GPU memory for partial sums
    double *d_block_sums;
    cudaMalloc(&d_block_sums, blocks * sizeof(double));

    double *h_block_sums = (double *)malloc(blocks * sizeof(double));

    // Timing kernel execution
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    midpoint_kernel<<<blocks, threadsPerBlock, threadsPerBlock * sizeof(double)>>>(a, dx, n, d_block_sums);
    cudaDeviceSynchronize();

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms;
    cudaEventElapsedTime(&ms, start, stop);

    // Copy results back to CPU
    cudaMemcpy(h_block_sums, d_block_sums, blocks * sizeof(double), cudaMemcpyDeviceToHost);

    // Final reduction on CPU
    double total_sum = 0.0;
    for (int i = 0; i < blocks; i++)
        total_sum += h_block_sums[i];

    double integral = total_sum * dx;

    // Output for performance evaluation
    printf("Approx integral = %.12f\n", integral);
    printf("time=%.9f threadsPerBlock=%d\n", ms / 1000.0, threadsPerBlock);

    cudaFree(d_block_sums);
    free(h_block_sums);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}

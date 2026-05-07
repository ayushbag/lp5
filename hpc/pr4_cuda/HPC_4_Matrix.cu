// Save this file as: matrix_mul.cu
#include <iostream>
#include <cuda.h>

using namespace std;

// --- The CUDA Kernel ---
__global__ void matrixMul(int *A, int *B, int *C, int width) {
    // Calculate 2D thread indices (row and column)
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < width && col < width) {
        int sum = 0;
        // Dot product of the row of A and column of B
        for (int k = 0; k < width; k++) {
            sum += A[row * width + k] * B[k * width + col];
        }
        C[row * width + col] = sum;
    }
}

int main() {
    int width = 1000; // 1000x1000 matrices
    int size = width * width * sizeof(int);

    // Allocate Host Memory
    int *h_A = (int *)malloc(size);
    int *h_B = (int *)malloc(size);
    int *h_C = (int *)malloc(size);

    // Initialize matrices
    for (int i = 0; i < width * width; i++) {
        h_A[i] = 1; // Fill with 1s
        h_B[i] = 1;
    }

    // Allocate Device Memory
    int *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);

    // Copy Host to Device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Define 2D Block and Grid sizes
    dim3 threadsPerBlock(16, 16); // 16x16 = 256 threads per block
    dim3 blocksPerGrid((width + threadsPerBlock.x - 1) / threadsPerBlock.x, 
                       (width + threadsPerBlock.y - 1) / threadsPerBlock.y);

    cout << "Launching Matrix Mul Kernel for " << width << "x" << width << " matrices." << endl;
    
    // Launch Kernel
    matrixMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, width);

    // Copy Result Device to Host
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Verify Result (1 * 1 added 1000 times should be 1000)
    cout << "Verification: C[0][0] = " << h_C[0] << " (Expected: " << width << ")" << endl;

    // Free Memory
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(h_A); free(h_B); free(h_C);

    return 0;
}

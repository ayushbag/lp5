// Save this file as: vector_add.cu
#include <iostream>
#include <cuda.h>

using namespace std;

// --- The CUDA Kernel (Runs on the GPU) ---
// __global__ tells the compiler this runs on the GPU but is called from the CPU
__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    // Calculate the unique global thread ID
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    
    // Ensure we don't go out of bounds if threads > array size
    if (i < n) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    int n = 100000; // Size of the vectors
    int size = n * sizeof(int);

    // 1. Allocate Host (CPU) Memory
    int *h_A = (int *)malloc(size);
    int *h_B = (int *)malloc(size);
    int *h_C = (int *)malloc(size);

    // Initialize the vectors on the CPU
    for (int i = 0; i < n; i++) {
        h_A[i] = 1; // Arbitrary values
        h_B[i] = 2;
    }

    // 2. Allocate Device (GPU) Memory
    int *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);

    // 3. Copy Data from Host to Device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // 4. Launch the Kernel
    // Define how many threads per block, and how many blocks we need
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    
    cout << "Launching Vector Add Kernel with " << blocksPerGrid << " blocks of " << threadsPerBlock << " threads." << endl;
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);

    // 5. Copy Result back from Device to Host
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Verify a random element to check if it worked (1 + 2 should be 3)
    cout << "Verification: C[5000] = " << h_C[5000] << " (Expected: 3)" << endl;

    // 6. Free Memory
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(h_A); free(h_B); free(h_C);

    return 0;
}

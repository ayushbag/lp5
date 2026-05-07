#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// --- UTILITY FUNCTIONS ---
void generateRandomArray(vector<int>& arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

// Function to merge two halves for Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; } 
        else { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

// --- BUBBLE SORT ---

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int> arr) { // Pass by value to keep original unsorted
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort (Odd-Even Sort)
void parallelBubbleSort(vector<int> arr) {
    int n = arr.size();
    bool isSorted = false;
    
    while (!isSorted) {
        isSorted = true;
        
        // Even phase
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }

        // Odd phase
        #pragma omp parallel for
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }
    }
}

// --- MERGE SORT ---

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Use parallel tasks for large arrays, fallback to sequential for small chunks to avoid overhead
        if ((right - left) > 1000) { 
            #pragma omp task shared(arr)
            parallelMergeSort(arr, left, mid);
            
            #pragma omp task shared(arr)
            parallelMergeSort(arr, mid + 1, right);
            
            #pragma omp taskwait // Wait for both halves to finish before merging
        } else {
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }
        
        merge(arr, left, mid, right);
    }
}

int main() {
    int n = 20000; // Size of array (make it large to see the time difference)
    vector<int> arr(n);
    srand(time(0));
    generateRandomArray(arr, n);

    cout << "Array Size: " << n << " elements\n" << endl;
    double start_time, end_time;

    // --- Benchmark Bubble Sort ---
    start_time = omp_get_wtime();
    sequentialBubbleSort(arr);
    end_time = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end_time - start_time << " seconds" << endl;

    start_time = omp_get_wtime();
    parallelBubbleSort(arr);
    end_time = omp_get_wtime();
    cout << "Parallel Bubble Sort Time:   " << end_time - start_time << " seconds\n" << endl;

    // --- Benchmark Merge Sort ---
    vector<int> arrMergeSeq = arr; // Copy array for merge sort
    start_time = omp_get_wtime();
    sequentialMergeSort(arrMergeSeq, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Sequential Merge Sort Time:  " << end_time - start_time << " seconds" << endl;

    vector<int> arrMergePar = arr; // Copy array for merge sort
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single // Only one thread spawns the initial tasks
        parallelMergeSort(arrMergePar, 0, n - 1);
    }
    end_time = omp_get_wtime();
    cout << "Parallel Merge Sort Time:    " << end_time - start_time << " seconds" << endl;

    return 0;
}
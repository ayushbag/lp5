#include <iostream>
#include <algorithm>
#include <omp.h>
using namespace std;

void parallelBubbleSort(int arr[], int n) {
    for(int i=0; i<n; i++) {

        // Even phase
        #pragma omp parallel for
        for(int j=0; j<n-1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j+1]);
            }
        }

        // Odd phase
        #pragma omp parallel for
        for(int j=1; j<n-1; j+=2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int arr[] = {5, 1, 4, 2, 8};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original Array: ";
    printArray(arr, n);

    parallelBubbleSort(arr, n);

    cout << "Bubble Sorted: ";
    printArray(arr, n);

    return  0;
}
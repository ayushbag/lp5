// Merge Sort is a Divide and Conquer sorting algorithm.
//  Instead of swapping adjacent elements like bubble sort, it works by:
// - Dividing the array into smaller parts
// - Sorting those smaller parts
// - Merging them back together in sorted order

#include <iostream>
using namespace std;

// Merge two sorted halves
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    // Copy data into temp arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Merge back in sorted order
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements
    while (i < n1) {
        arr[k] = L[i];
        i++; k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++; k++;
    }
}

// Recursive merge sort
void parallelMergeSort(int arr[], int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    
    // Parallel region for independent tasks
    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(arr, left, mid);       // left half

        #pragma omp section
        parallelMergeSort(arr, mid + 1, right); // right half
    }

    merge(arr, left, mid, right);    // merge
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int arr[] = {5, 1, 4, 2, 8};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original array: ";
    printArray(arr, n);

    parallelMergeSort(arr, 0, n - 1);

    cout << "Sorted array: ";
    printArray(arr, n);

    return 0;
}

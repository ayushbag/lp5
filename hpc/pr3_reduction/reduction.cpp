#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Utility function to generate random array
void generateRandomArray(vector<int>& arr, int n) {
    for (int i=0; i<n; i++) {
        arr[i] = rand() % 1000; // Random numbers between 0 and 999
    }
}

int main() {
    int n = 1000000; // Use a large array (1 million elements) to see the benefit
    vector<int> arr(n);
    srand(time(0));
    generateRandomArray(arr, n);

    cout << "Array size" << n << "elements\n" << endl;

    // Initialize variables with appropriate starting values
    int min_val = arr[0];
    int max_val = arr[0];
    long long sum_val = 0; // Use long long to prevent overflow

    double start_time, end_time;
    start_time = omp_get_wtime();

    // --- PARALLEL REDUCTION ---
    // The compiler automatically handles giving each thread a private copy
    // of min_val, max_val, and sum_val, and then combines them at the end.

    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum_val)
    for (int i=0; i<n; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
        sum_val += arr[i];
    }

    end_time = omp_get_wtime();

    // Calculate Average (sum divided by number of elements)
    double avg_val = (double)sum_val / n;

    // Output Results
    cout << "Parallel Reduction Results:" << endl;
    cout << "Minimum Value : " << min_val << endl;
    cout << "Maximum Value : " << max_val << endl;
    cout << "Sum           : " << sum_val << endl;
    cout << "Average       : " << avg_val << endl;
    
    cout << "\nExecution Time: " << end_time - start_time << " seconds" << endl;

    return 0;
}
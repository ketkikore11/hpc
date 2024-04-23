#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <omp.h>

using namespace std;

// Function to merge two subarrays
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    vector<int> L(n1), R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
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

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Function to perform merge sort
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Parallelize this section
        #pragma omp task shared(arr)
        mergeSort(arr, left, mid);
        #pragma omp task shared(arr)
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;

    // Generate random input array
    vector<int> arr(n);
    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    // Make a copy for parallel sorting
    vector<int> arr_parallel = arr;

    double start_time, end_time;

    // Display original elements
    cout << "Original elements:\n";
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;

    // Sequential merge sort
    start_time = omp_get_wtime();
    mergeSort(arr, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Sorted elements (sequential):\n";
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
    cout << "Time for sequential algorithm: " << end_time - start_time << " seconds" << endl;

    // Parallel merge sort
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        mergeSort(arr_parallel, 0, n - 1);
    }
    end_time = omp_get_wtime();
    cout << "Sorted elements (parallel):\n";
    for (int i = 0; i < n; ++i)
        cout << arr_parallel[i] << " ";
    cout << endl;
    cout << "Time for parallel algorithm: " << end_time - start_time << " seconds" << endl;

    // Verify if both sorting methods produce the same result
    if (arr == arr_parallel)
        cout << "Both sequential and parallel merge sort produced the same result." << endl;
    else
        cout << "Sequential and parallel merge sort produced different results!" << endl;

    return 0;
}

/*
g++ -o mergesort -fopenmp mergesort.cpp
./mergesort

Enter the number of elements in the array: 9
Enter 9 integers:
22 11 55 33 44 77 99 88 66
Original elements:
22 11 55 33 44 77 99 88 66
Sorted elemnts(sequential):
11 22 33 44 55 66 77 88 99
Time for sequential algorithm: 2.1896 sec
Sorted elements(parallel):
11 22 33 44 55 77 88 66 99
Time for parallel algorithm: 0.00458446sec
Sequential and parallel merge sort produced different results!
*/

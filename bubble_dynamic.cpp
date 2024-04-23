#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

void bubbleSort(vector<int> &arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped, then the array is sorted
        if (!swapped)
            break;
    }
}

void parallelBubbleSort(vector<int> &arr) {
    int n = arr.size();
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            bool swapped = true;
            while (swapped) {
                swapped = false;
                #pragma omp parallel for shared(arr, swapped)
                for (int i = 0; i < n - 1; i++) {
                    if (arr[i] > arr[i + 1]) {
                        swap(arr[i], arr[i + 1]);
                        swapped = true;
                    }
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Sequential Bubble Sort
    auto startSeq = high_resolution_clock::now();
    bubbleSort(arr);
    auto stopSeq = high_resolution_clock::now();
    auto durationSeq = duration_cast<milliseconds>(stopSeq - startSeq);
    cout << "Sorted array (sequential): ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    cout << "Time taken for sequential execution: " << durationSeq.count() << " milliseconds" << endl;

    // Parallel Bubble Sort
    auto startPar = high_resolution_clock::now();
    parallelBubbleSort(arr);
    auto stopPar = high_resolution_clock::now();
    auto durationPar = duration_cast<milliseconds>(stopPar - startPar);
    cout << "Sorted array (parallel): ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    cout << "Time taken for parallel execution: " << durationPar.count() << " milliseconds" << endl;

    return 0;
}
/*
Enter the number of elements: 4
Enter 4 integers:
5 3 7 1
Sorted array (sequential): 1 3 5 7 
Time taken for sequential execution: 0 milliseconds
Sorted array (parallel): 1 3 5 7
Time taken for parallel execution: 0 milliseconds
*/

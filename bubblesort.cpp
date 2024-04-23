#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void bubbleSort(vector<int> &arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        #pragma omp parallel for shared(arr, swapped)
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

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    bubbleSort(arr);

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}

/*

g++ -o bubble -fopenmp bubble.cpp
./bubble

Enter the number of elements: 5
Enter 5 integers:
4 3 6 7 8
Sorted array: 3 4 6 7 8  
*/

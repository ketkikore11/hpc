#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>

using namespace std;

void parallel_reduction(const vector<int> &arr, int &min_val, int &max_val, int &sum, double &avg) {
    int n = arr.size();
    int local_min = numeric_limits<int>::max();
    int local_max = numeric_limits<int>::min();
    int local_sum = 0;

    #pragma omp parallel for reduction(min:local_min) reduction(max:local_max) reduction(+:local_sum)
    for (int i = 0; i < n; ++i) {
        local_min = min(local_min, arr[i]);
        local_max = max(local_max, arr[i]);
        local_sum += arr[i];
    }

    min_val = local_min;
    max_val = local_max;
    sum = local_sum;
    avg = static_cast<double>(sum) / n;
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

    int min_val, max_val, sum;
    double avg;
    parallel_reduction(arr, min_val, max_val, sum, avg);

    cout << "Minimum value: " << min_val << endl;
    cout << "Maximum value: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;

    return 0;
}

/*
g++ -o parallelReduction_minmax -fopenmp parallelReduction_minmax.cpp
./parallelReduction_minmax

Enter the number of elements: 6
Enter 6 integers:
2
3
4
5
6
7
Minimum value: 2
Maximum value: 7
Sum: 27
Average: 4.5
*/

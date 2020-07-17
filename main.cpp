#include <iostream>
#include <ctime>

using namespace std;

void swap(int &a, int &b);
void quickSort(int start, int end, int arr[]);
void display(int arr[]);

int main()
{
    int arr[10000], start = 0, end = 9999;
    srand(time(0));
    for(int i = 0; i < 10000; ++i)
        arr[i] = rand() % 10000 + 1;
    std::cout << "Unsorted: " << std::endl;
    display(arr);
    std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    #pragma omp parallel shared(arr) firstprivate(start, end)
        #pragma omp single
            quickSort(start, end, arr);
    std::cout << "Sorted: " << std::endl;
    display(arr);
    return 0;
}

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void quickSort(int start, int end, int arr[])
{
    if(start >= end)
        return;
    int pivot = arr[(end + start) / 2];
    swap(arr[end], arr[(end + start) / 2]);
    int indexToSwap = start;
    for(int i = start; i < end; ++i)
    {
        if(arr[i] < pivot)
        {
            swap(arr[i], arr[indexToSwap]);
            indexToSwap++;
        }
    }
    swap(arr[end], arr[indexToSwap]); // indexToSwap == pivot
    #pragma omp task firstprivate(start, indexToSwap)
    quickSort(start, indexToSwap-1, arr); //quicksort left side, pivot is already in the right place, dont need to sort it
    #pragma omp task firstprivate(end, indexToSwap)
    quickSort(indexToSwap+1, end, arr); //quicksort right side
}

void display(int arr[])
{
    std::cout << arr[0] << " ";
    for(int i = 1; i < 10000; ++i)
    {
         std::cout << arr[i] << " ";
         if(i%30 == 0)
             std::cout << std::endl;
    }
    std::cout << std::endl;
}

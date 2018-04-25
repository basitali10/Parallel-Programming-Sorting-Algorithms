
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NO_OF_ITEMS 1000
static int *integer_array;

void swap(int*, int, int); //function to swap two integer values
void print();              //function to print our global array.
void initializeIntegerArray(); //function to initialize array dynamically with random numbers.
unsigned partition(int *, unsigned, unsigned, unsigned); //function for partition
void quick_sort(int *, unsigned , unsigned ); //recursive quicksort function

//MAIN FUNTION
int main(){
    int i = 0;clock_t t;
    initializeIntegerArray();
    printf("\n\n------Before Sorting------\n\n");
    print();
    t = clock();
    quick_sort(integer_array, 0, NO_OF_ITEMS-1);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\n\n------After  Sorting------\n\n");
    print();
    printf("Quick Sort Sequential took %f seconds to sort the integer array. \n", time_taken);
    free(integer_array);
}

void swap(int* v, int a, int b){
    unsigned temp;
    temp=v[a];
    v[a]=v[b];
    v[b]=temp;
}
void print() {
    int i;
    printf("\n----------------OUR INTEGER ARRAY----------------\n");
    for (i = 0; i < NO_OF_ITEMS; i++) {
        printf("%d\n", integer_array[i]);
    }
    printf("\n\n----------------INTEGER ARRAY PRINTED----------------\n");
}
void initializeIntegerArray(){
    int i;time_t t;
    integer_array = (int *) malloc(NO_OF_ITEMS*sizeof(int)); //DMA
    srand((unsigned) time(&t)); //randomizing everytime
    for (i = 0; i < NO_OF_ITEMS; i++) {
        integer_array[i] = rand() % (1000 + 1 - 0) + 0; //Generating random numbers between 0-1000
    }
}
unsigned partition(int *integer_array, unsigned first, unsigned last, unsigned pivot_index) {
    if (pivot_index != first){
        swap(integer_array, first, pivot_index);
    }
    pivot_index = first;
    first++;
    while (first <= last) {
        if (integer_array[first] <= integer_array[pivot_index]){
            first++;
        }
        else{
            if (integer_array[last] > integer_array[pivot_index]){
                last--;
            }
            else{
                swap(integer_array, first, last);
            }
        }

    }
    if (last != pivot_index){
        swap(integer_array, pivot_index, last);
    }
    return last;
}

void quick_sort(int *integer_array, unsigned first, unsigned last) {
    unsigned pivot;
    if (first >= last){
        return;
    }
    pivot = (first+last)/2; //middle element as pivot.
    pivot = partition(integer_array, first, last, pivot);
    #pragma omp parallel sections num_threads(8) //creating 8 threads
    {
        #pragma omp section
        {
            if (first < pivot) {
                quick_sort(integer_array, first, pivot-1);
            }
        }
        #pragma omp section
            {
            if (pivot < last) {
                quick_sort(integer_array, pivot+1, last);
            }
        }
    }
}



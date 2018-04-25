
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NO_OF_ITEMS 1000

static int *integer_array; //our global integer array


void swap(int*, int, int); //function to swap two integer values
void print();              //function to print our global array.
void initializeIntegerArray(); //function to initialize array dynamically with random numbers.
void merge_sort(int,int);
void merge(int, int, int);


//MAIN FUNCTION
int main(){
    int j=0,i=0;
    initializeIntegerArray();
    printf("\n\n------Before Sorting------\n\n");
    print();
    clock_t t;
    t = clock();
    merge_sort(0,NO_OF_ITEMS-1);
    t = clock() - t;

    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("\n\n------After  Sorting------\n\n");
    print();
    printf("Merge Sort Sequential took %f seconds to execute \n", time_taken);
    free(integer_array);
}


void swap(int* v, int a, int b) {
    unsigned temp;
    temp=v[a];
    v[a]=v[b];
    v[b]=temp;
}

void print() {
    int i;
    printf("\n----------------OUR INTEGER ARRAY----------------\n\n");
    for (i = 0; i < NO_OF_ITEMS; i++) {
        printf("%d\t", integer_array[i]);
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


// merge function for merging two parts
void merge(int low, int mid, int high){
    int* left = (int *) malloc((mid - low + 1)*sizeof(int));
    int* right = (int *) malloc((high - mid)*sizeof(int));
    // n1 is size of left part and n2 is size of right part
    int n1 = mid - low + 1, n2 = high - mid, i, j;

    // storing values in left part
    for (i = 0; i < n1; i++){
        left[i] = integer_array[i + low];
    }

    // storing values in right part
    for (i = 0; i < n2; i++){
        right[i] = integer_array[i + mid + 1];
    }

    int k = low;
    i = j = 0;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]){
            integer_array[k++] = left[i++];
        }
        else{
            integer_array[k++] = right[j++];
        }
    }

    while (i < n1) {
        integer_array[k++] = left[i++];
    }

    while (j < n2) {
        integer_array[k++] = right[j++];
    }
    free(left);
    free(right);
}

// merge sort function
void merge_sort(int low, int high){
    // calculating mid point of array
    int mid = low + (high - low) / 2;
    if (low < high) {

        merge_sort(low, mid);

        merge_sort(mid + 1, high);

        merge(low, mid, high);
    }
}

/* Bubble sort code */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define NO_OF_ITEMS 1000
static int *integer_array;

void swap(int*, int, int); //function to swap two integer values
void print();              //function to print our global array.
void initializeIntegerArray(); //function to initialize array dynamically with random numbers.
void bubbleSort(int*);

int main(){
    clock_t t;
    initializeIntegerArray();
    printf("\n\n------Before Sorting------\n\n");
    print();
    t = clock();
    bubbleSort(integer_array);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("\n\n------After  Sorting------\n\n");
    print();
    printf("Bubble Sort Sequential took %f seconds to sort the integer array. \n", time_taken);
    free(integer_array);
    return 0;
}

void swap(int* v, int a, int b){
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

void bubbleSort(int* integer_array){
    int i,j;
    for (i = 0 ; i < ( NO_OF_ITEMS - 1 ); i++){
        for (j = 0 ; j < NO_OF_ITEMS - i - 1; j++){
            if (integer_array[j] > integer_array[j+1]){
                swap(integer_array,j,j+1);
            }
        }
    }
}

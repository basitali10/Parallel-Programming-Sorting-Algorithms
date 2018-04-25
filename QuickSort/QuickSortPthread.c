#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define NO_OF_ITEMS 1000
#define MAX_THREADS 8
#define SIZE_OF_CHUNK (NO_OF_ITEMS/MAX_THREADS)

static int *integer_array; //our global integer array
pthread_t tid[MAX_THREADS]; //maximum numbers of thread defined

typedef struct {
    int end;
    int start;
}Bounds;
typedef struct  {
    unsigned left_bound;
    unsigned right_bound;
    unsigned size_of_chunk;
}MergeBoundaries;

void swap(int*, int, int); //function to swap two integer values
void print();              //function to print our global array.
void initializeIntegerArray(); //function to initialize array dynamically with random numbers.
unsigned partition(int *, unsigned, unsigned, unsigned); //function for partition
void quick_sort(int *, unsigned , unsigned ); //recursive quicksort function
void * threadFunctionToSort(void *); //thread function to just sort a chunk defined by chunk's boundary.
void * threadFunctionToMerge(void *); //thread function to merge two chunks.

//MAIN FUNCTION
int main(){
    int j=0,i=0,purani=0;
    initializeIntegerArray();
    printf("\n\n------Before Sorting------\n\n");
    print();
    clock_t t;
    t = clock();
    //Sorting chunks seperately
    for(i=0; i < MAX_THREADS; i++) {
        Bounds * obj_of_Bounds = malloc(sizeof(* obj_of_Bounds));
        if(i==7){
            obj_of_Bounds->start = purani;
            obj_of_Bounds->end = NO_OF_ITEMS;

        }
        else{
            obj_of_Bounds->start = SIZE_OF_CHUNK * (i+1) - SIZE_OF_CHUNK;
            obj_of_Bounds->end = SIZE_OF_CHUNK * (i+1);
            purani=obj_of_Bounds->end;
        }
        printf("\nThread no %d Bounds : %d --- %d \n",(i+1),obj_of_Bounds->start,obj_of_Bounds->end-1);
        pthread_create(&tid[i], 0, threadFunctionToSort, obj_of_Bounds);

    }
    for(i=0; i < MAX_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    // Merging chunks two by two

    int var1=0,var2=1;
    for(i=0; i < MAX_THREADS/2; i++) {
            MergeBoundaries * obj_of_MergeBoundaries = malloc(sizeof(* obj_of_MergeBoundaries));
            obj_of_MergeBoundaries->left_bound = SIZE_OF_CHUNK * var1; var1+=2;
            obj_of_MergeBoundaries->right_bound = SIZE_OF_CHUNK* var2; var2+=2;
            obj_of_MergeBoundaries->size_of_chunk = SIZE_OF_CHUNK;
            pthread_create(&tid[i], 0, threadFunctionToMerge, obj_of_MergeBoundaries);

    }
    var1=0;var2=2;
    for(i=0; i < MAX_THREADS/2; i++) {
        pthread_join(tid[i], NULL);
    }

    for(i=0; i < MAX_THREADS/4; i++) {
            MergeBoundaries * obj_of_MergeBoundaries = malloc(sizeof(* obj_of_MergeBoundaries));
            obj_of_MergeBoundaries->left_bound = SIZE_OF_CHUNK * var1; var1+=4;
            obj_of_MergeBoundaries->right_bound = SIZE_OF_CHUNK * var2;var2+=4;
            obj_of_MergeBoundaries->size_of_chunk = SIZE_OF_CHUNK * 2;
            pthread_create(&tid[i], 0, threadFunctionToMerge, obj_of_MergeBoundaries);

    }

    for(i=0; i < MAX_THREADS/4; i++) {
        pthread_join(tid[i], NULL);
    }
    MergeBoundaries * obj_of_MergeBoundaries = malloc(sizeof(* obj_of_MergeBoundaries));
    obj_of_MergeBoundaries->left_bound = 0;
    obj_of_MergeBoundaries->right_bound = SIZE_OF_CHUNK * 4;
    obj_of_MergeBoundaries->size_of_chunk = SIZE_OF_CHUNK * 4;
    pthread_create(&tid[0], 0, threadFunctionToMerge, obj_of_MergeBoundaries);
    pthread_join(tid[0], NULL);

    t = clock() - t;

    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("\n\n------After  Sorting------\n\n");
    print();
    printf("Quick Sort Pthread took %f seconds to execute \n", time_taken);
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

void quick_sort(int *integer_array, unsigned first, unsigned last){
    unsigned pivot;
    if (first >= last){
        return;
    }
    pivot = (first+last)/2; //middle element as pivot.
    pivot = partition(integer_array, first, last, pivot);
    if (first < pivot){
        quick_sort(integer_array, first, pivot-1);
    }
    if (pivot < last){
        quick_sort(integer_array, pivot+1, last);
    }
}

void * threadFunctionToSort(void * obj) {
    Bounds obj_of_Bounds = *((Bounds *) obj);

    quick_sort(integer_array, obj_of_Bounds.start, obj_of_Bounds.end-1);
    free(obj);
    pthread_exit(0);
}
void * threadFunctionToMerge(void * obj) {

    MergeBoundaries obj_of_MergeBoundaries = *((MergeBoundaries *) obj);

    int * left = integer_array + obj_of_MergeBoundaries.left_bound;
    int * right = integer_array + obj_of_MergeBoundaries.right_bound;
    int * temp = malloc((obj_of_MergeBoundaries.size_of_chunk * 2)*sizeof(int));

    int size = obj_of_MergeBoundaries.size_of_chunk;

    int i,j,k;
    i = j = k = 0;
    while(i < size && j < size) {
        if(left[i] <= right[j]) {
            temp[k++] = left[i++];
        }
        else {
            temp[k++] = right[j++];
        }
    }
    while(i < size){
        temp[k++] = left[i++];
    }
    while(j < size){
        temp[k++] = right[j++];
    }
    int s = obj_of_MergeBoundaries.left_bound;
    for(i=0; i < obj_of_MergeBoundaries.size_of_chunk * 2; i++) {
        integer_array[s++] = temp[i];
    }
    free(obj);
    free(temp);
    pthread_exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Structure to pass data to threads
typedef struct {
    int *array;
    int start;
    int end;
} SortParams;

// Thread function to sort a portion of the array
void* sort_thread(void *arg) {
    SortParams *params = (SortParams *)arg;
    int *array = params->array;
    int start = params->start;
    int end = params->end;

    // Simple bubble sort for demonstration (replace with better algorithm if needed)
    for (int i = start; i < end; i++) {
        for (int j = start; j < end - 1; j++) {
            if (array[j] > array[j+1]) {
                // Swap elements
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }
    return NULL;
}

// Function to merge two sorted halves of the array
void merge(int *array, int start, int mid, int end) {
    int left_size = mid - start;
    int right_size = end - mid;
    
    // Create temporary arrays
    int *left = malloc(left_size * sizeof(int));
    int *right = malloc(right_size * sizeof(int));
    
    // Copy data to temp arrays
    for (int i = 0; i < left_size; i++)
        left[i] = array[start + i];
    for (int j = 0; j < right_size; j++)
        right[j] = array[mid + j];
    
    // Merge the temp arrays back into array[start..end]
    int i = 0, j = 0, k = start;
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            array[k] = left[i];
            i++;
        } else {
            array[k] = right[j];
            j++;
        }
        k++;
    }
    
    // Copy remaining elements of left[]
    while (i < left_size) {
        array[k] = left[i];
        i++;
        k++;
    }
    
    // Copy remaining elements of right[]
    while (j < right_size) {
        array[k] = right[j];
        j++;
        k++;
    }
    
    free(left);
    free(right);
}

// Function to print array
void print_array(int *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
    const int ARRAY_SIZE = 20;
    int array[ARRAY_SIZE];
    
    // Initialize array with random values
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100;
    }
    
    printf("Original array:\n");
    print_array(array, ARRAY_SIZE);
    
    pthread_t thread1, thread2;
    SortParams params1, params2;
    
    // First thread sorts first half
    params1.array = array;
    params1.start = 0;
    params1.end = ARRAY_SIZE / 2;
    
    // Second thread sorts second half
    params2.array = array;
    params2.start = ARRAY_SIZE / 2;
    params2.end = ARRAY_SIZE;
    
    // Create threads
    pthread_create(&thread1, NULL, sort_thread, &params1);
    pthread_create(&thread2, NULL, sort_thread, &params2);
    
    // Wait for threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("\nAfter sorting halves:\n");
    print_array(array, ARRAY_SIZE);
    
    // Merge the two sorted halves
    merge(array, 0, ARRAY_SIZE / 2, ARRAY_SIZE);
    
    printf("\nFinal sorted array:\n");
    print_array(array, ARRAY_SIZE);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Bubble Sort - Used by Parent
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    printf("\nParent: Starting Bubble Sort...\n");
    
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    
    printf("Parent: Sorted array (Bubble Sort): ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Insertion Sort - Used by Child
void insertionSort(int arr[], int n) {
    int i, key, j;
    printf("\nChild: Starting Insertion Sort...\n");
    
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    
    printf("Child: Sorted array (Insertion Sort): ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n, i;
    int arr[100];
    pid_t pid;
    int status;
    
    printf("=== NORMAL EXECUTION: Fork + Wait ===\n");
    printf("Enter the number of integers: ");
    scanf("%d", &n);
    
    printf("Enter %d integers:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("\nOriginal array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n========================================\n");
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // CHILD PROCESS
        printf("\n[CHILD PROCESS]\n");
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        insertionSort(arr, n);
        
        printf("Child: Exiting...\n");
        exit(0);
    }
    else {
        // PARENT PROCESS
        printf("\n[PARENT PROCESS]\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
        
        bubbleSort(arr, n);
        
        printf("\nParent: Waiting for child to finish...\n");
        wait(&status);  // Parent waits for child
        
        printf("Parent: Child completed with status %d\n", WEXITSTATUS(status));
        printf("Parent: Exiting...\n");
    }
    
    return 0;
}

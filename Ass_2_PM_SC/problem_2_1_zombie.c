#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main() {
    int n, i;
    int arr[100];
    pid_t pid;
    int status;
    
    printf("=== ZOMBIE PROCESS DEMONSTRATION ===\n");
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
        // CHILD PROCESS - Exits quickly
        printf("\n[CHILD PROCESS]\n");
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        printf("Child: Performing Insertion Sort...\n");
        insertionSort(arr, n);
        
        printf("Child: Sorted array: ");
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        
        printf("Child: Exiting immediately (will become ZOMBIE)...\n");
        exit(0);  // Child exits immediately
    }
    else {
        // PARENT PROCESS - Delays before calling wait()
        printf("\n[PARENT PROCESS]\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
        
        printf("\nParent: Performing Bubble Sort...\n");
        bubbleSort(arr, n);
        
        printf("Parent: Sorted array: ");
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        
        // ZOMBIE DEMONSTRATION
        printf("\n*** ZOMBIE STATE ***\n");
        printf("Parent: Sleeping for 10 seconds WITHOUT calling wait()...\n");
        printf("Child has exited but parent hasn't called wait() yet.\n");
        printf("Child is now a ZOMBIE process!\n");
        printf("\nTo verify: Open another terminal and run: ps aux | grep Z\n");
        printf("Or run: ps -l (look for 'Z' in STAT column)\n\n");
        
        sleep(10);  // Parent sleeps - child becomes zombie during this time
        
        printf("\nParent: Now calling wait() to reap zombie child...\n");
        wait(&status);  // Reap the zombie
        
        printf("Parent: Child has been reaped. Zombie removed from process table.\n");
        printf("Parent: Exiting...\n");
    }
    
    return 0;
}

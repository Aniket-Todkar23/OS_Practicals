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
    
    printf("=== ORPHAN PROCESS DEMONSTRATION ===\n");
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
        // CHILD PROCESS - Takes longer to complete
        printf("\n[CHILD PROCESS]\n");
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        printf("Child: Sleeping for 5 seconds before sorting...\n");
        sleep(5);  // Child takes longer - parent will exit first
        
        printf("\nChild: Woke up! Checking parent status...\n");
        printf("Child: My current Parent PID: %d\n", getppid());
        printf("(If Parent PID = 1 or init/systemd PID, I'm an ORPHAN!)\n");
        
        printf("\nChild: Performing Insertion Sort...\n");
        insertionSort(arr, n);
        
        printf("Child: Sorted array: ");
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        
        printf("Child: Exiting...\n");
        exit(0);
    }
    else {
        // PARENT PROCESS - Exits quickly WITHOUT waiting
        printf("\n[PARENT PROCESS]\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
        
        printf("\nParent: Performing Bubble Sort...\n");
        bubbleSort(arr, n);
        
        printf("Parent: Sorted array: ");
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        
        // ORPHAN DEMONSTRATION
        printf("\n*** ORPHAN STATE ***\n");
        printf("Parent: Exiting WITHOUT waiting for child...\n");
        printf("Child will become an ORPHAN (adopted by init/systemd)!\n");
        printf("Parent: Exiting now...\n\n");
        
        exit(0);  // Parent exits - child becomes orphan
        // NO wait() call - this creates orphan
    }
    
    return 0;
}

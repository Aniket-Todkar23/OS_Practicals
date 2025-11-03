#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Bubble Sort - Used by Parent
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

// Insertion Sort - Used by Child
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

// Function to display array
void displayArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Normal Execution with wait()
void normalExecution(int arr[], int n) {
    pid_t pid;
    int status;
    
    printf("\n========== NORMAL EXECUTION ==========\n");
    printf("Parent and Child both sort, Parent waits for Child\n");
    printf("======================================\n");
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    else if (pid == 0) {
        // CHILD PROCESS
        printf("\n[CHILD PROCESS]\n");
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        printf("Child: Performing Insertion Sort...\n");
        insertionSort(arr, n);
        
        printf("Child: Sorted array: ");
        displayArray(arr, n);
        
        printf("Child: Exiting...\n");
        exit(0);
    }
    else {
        // PARENT PROCESS
        printf("\n[PARENT PROCESS]\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
        
        printf("Parent: Performing Bubble Sort...\n");
        bubbleSort(arr, n);
        
        printf("Parent: Sorted array: ");
        displayArray(arr, n);
        
        printf("\nParent: Waiting for child to finish...\n");
        wait(&status);
        
        printf("Parent: Child exited with status %d\n", WEXITSTATUS(status));
        printf("Parent: Done!\n");
    }
}

// Zombie Process Demonstration
void zombieDemo(int arr[], int n) {
    pid_t pid;
    int status;
    
    printf("\n========== ZOMBIE PROCESS DEMO ==========\n");
    printf("Child exits quickly, Parent delays wait()\n");
    printf("=========================================\n");
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    else if (pid == 0) {
        // CHILD PROCESS - Exits quickly
        printf("\n[CHILD PROCESS]\n");
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        printf("Child: Performing Insertion Sort...\n");
        insertionSort(arr, n);
        
        printf("Child: Sorted array: ");
        displayArray(arr, n);
        
        printf("Child: Exiting immediately (will become ZOMBIE)...\n");
        exit(0);
    }
    else {
        // PARENT PROCESS - Delays before calling wait()
        printf("\n[PARENT PROCESS]\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
        
        printf("Parent: Performing Bubble Sort...\n");
        bubbleSort(arr, n);
        
        printf("Parent: Sorted array: ");
        displayArray(arr, n);
        
        printf("\n*** ZOMBIE STATE ***\n");
        printf("Parent: Sleeping for 10 seconds WITHOUT calling wait()...\n");
        printf("Child has exited but not reaped yet.\n");
        printf("Child is now a ZOMBIE process!\n");
        printf("\nTo verify: Open another terminal and run:\n");
        printf("  ps aux | grep Z   OR   ps -l\n");
        printf("Look for process with status 'Z' or '<defunct>'\n\n");
        
        sleep(10);  // Child is zombie during this time
        
        printf("\nParent: Now calling wait() to reap zombie...\n");
        wait(&status);
        
        printf("Parent: Zombie reaped successfully!\n");
        printf("Parent: Done!\n");
    }
}

// Orphan Process Demonstration
void orphanDemo(int arr[], int n) {
    pid_t pid;
    
    printf("\n========== ORPHAN PROCESS DEMO ==========\n");
    printf("Parent exits early, Child becomes orphan\n");
    printf("=========================================\n");
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    else if (pid == 0) {
        // CHILD PROCESS - Takes longer
        printf("\n[CHILD PROCESS]\n");
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        printf("Child: Sleeping for 5 seconds...\n");
        sleep(5);
        
        printf("\nChild: Woke up! Checking parent status...\n");
        printf("Child: Current Parent PID: %d\n", getppid());
        
        if (getppid() == 1) {
            printf("Child: I am an ORPHAN! (Adopted by init/systemd)\n");
        } else {
            printf("Child: My parent changed (likely adopted by init/systemd)\n");
        }
        
        printf("\nChild: Performing Insertion Sort...\n");
        insertionSort(arr, n);
        
        printf("Child: Sorted array: ");
        displayArray(arr, n);
        
        printf("Child: Exiting...\n");
        exit(0);
    }
    else {
        // PARENT PROCESS - Exits quickly
        printf("\n[PARENT PROCESS]\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
        
        printf("Parent: Performing Bubble Sort...\n");
        bubbleSort(arr, n);
        
        printf("Parent: Sorted array: ");
        displayArray(arr, n);
        
        printf("\n*** ORPHAN STATE ***\n");
        printf("Parent: Exiting WITHOUT waiting for child...\n");
        printf("Child will become an ORPHAN (adopted by init)!\n");
        printf("Parent: Exiting now...\n\n");
        
        exit(0);  // Parent exits - child becomes orphan
    }
}

int main() {
    int choice;
    int n, i;
    int arr[100];
    
    while (1) {
        printf("\n");
        printf("╔════════════════════════════════════════════╗\n");
        printf("║   PROBLEM 2.1: Fork, Wait, Sort Demo      ║\n");
        printf("╚════════════════════════════════════════════╝\n");
        printf("\n");
        printf("1. Normal Execution (Fork + Wait)\n");
        printf("2. Zombie Process Demonstration\n");
        printf("3. Orphan Process Demonstration\n");
        printf("4. Exit\n");
        printf("\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);
        
        if (choice == 4) {
            printf("\nExiting program. Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > 4) {
            printf("\nInvalid choice! Please enter 1-4.\n");
            continue;
        }
        
        // Input array for options 1-3
        printf("\nEnter the number of integers (max 100): ");
        scanf("%d", &n);
        
        if (n <= 0 || n > 100) {
            printf("Invalid array size!\n");
            continue;
        }
        
        printf("Enter %d integers:\n", n);
        for (i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
        
        printf("\nOriginal array: ");
        displayArray(arr, n);
        
        switch (choice) {
            case 1:
                normalExecution(arr, n);
                break;
            case 2:
                zombieDemo(arr, n);
                break;
            case 3:
                orphanDemo(arr, n);
                break;
        }
        
        printf("\nPress Enter to continue...");
        getchar(); // Clear input buffer
        getchar(); // Wait for user
    }
    
    return 0;
}

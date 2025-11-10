#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void displayArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void normalExecution(int arr[], int n) {
    printf("\n=== NORMAL EXECUTION ===\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    
    if (pid == 0) {
        printf("Child PID: %d\n", getpid());
        insertionSort(arr, n);
        printf("Child sorted: ");
        displayArray(arr, n);
        exit(0);
    } else {
        printf("Parent PID: %d\n", getpid());
        bubbleSort(arr, n);
        printf("Parent sorted: ");
        displayArray(arr, n);
        wait(NULL);
        printf("Child completed\n");
    }
}

void zombieDemo(int arr[], int n) {
    printf("\n=== ZOMBIE DEMO ===\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    
    if (pid == 0) {
        printf("Child PID: %d\n", getpid());
        insertionSort(arr, n);
        printf("Child sorted: ");
        displayArray(arr, n);
        printf("Child exiting (will become zombie)\n");
        exit(0);
    } else {
        printf("Parent PID: %d\n", getpid());
        bubbleSort(arr, n);
        printf("Parent sorted: ");
        displayArray(arr, n);
        printf("\nParent sleeping 10s (child is zombie now)\n");
        printf("Check with: ps aux | grep Z\n");
        sleep(10);
        wait(NULL);
        printf("Zombie reaped\n");
    }
}

void orphanDemo(int arr[], int n) {
    printf("\n=== ORPHAN DEMO ===\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return;
    }
    
    if (pid == 0) {
        printf("Child PID: %d, Parent: %d\n", getpid(), getppid());
        sleep(5);
        printf("Child awake, Parent now: %d (adopted by init)\n", getppid());
        insertionSort(arr, n);
        printf("Child sorted: ");
        displayArray(arr, n);
        exit(0);
    } else {
        printf("Parent PID: %d\n", getpid());
        bubbleSort(arr, n);
        printf("Parent sorted: ");
        displayArray(arr, n);
        printf("Parent exiting (child becomes orphan)\n");
        exit(0);
    }
}

int main() {
    int choice, n, arr[100];
    
    while (1) {
        printf("\n=== PROBLEM 2.1: Fork Demo ===\n");
        printf("1. Normal (Fork + Wait)\n");
        printf("2. Zombie Process\n");
        printf("3. Orphan Process\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        if (choice == 4) break;
        if (choice < 1 || choice > 3) continue;
        
        printf("Array size (max 100): ");
        scanf("%d", &n);
        if (n <= 0 || n > 100) continue;
        
        printf("Enter %d integers: ", n);
        for (int i = 0; i < n; i++) scanf("%d", &arr[i]);
        
        printf("Original: ");
        displayArray(arr, n);
        
        if (choice == 1) normalExecution(arr, n);
        else if (choice == 2) zombieDemo(arr, n);
        else orphanDemo(arr, n);
        
        getchar();
        getchar();
    }
    
    return 0;
}

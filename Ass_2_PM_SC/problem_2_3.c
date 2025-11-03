#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to check if a number is prime
int isPrime(int n) {
    if (n <= 1) {
        return 0;  // Not prime
    }
    if (n == 2) {
        return 1;  // 2 is prime
    }
    if (n % 2 == 0) {
        return 0;  // Even numbers (except 2) are not prime
    }
    
    // Check odd divisors up to sqrt(n)
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return 0;  // Found a divisor, not prime
        }
    }
    return 1;  // Prime
}

// Function to generate first n prime numbers
void generatePrimes(int count) {
    int found = 0;  // Number of primes found
    int num = 2;    // Start checking from 2
    
    printf("\nChild Process: Generating first %d prime numbers:\n", count);
    printf("Primes: ");
    
    while (found < count) {
        if (isPrime(num)) {
            printf("%d ", num);
            found++;
        }
        num++;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int n;
    int status;
    
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║  Problem 2.3: Prime Number Generation        ║\n");
    printf("║  Using fork() and wait()                      ║\n");
    printf("╚═══════════════════════════════════════════════╝\n\n");
    
    // Check if command line argument is provided
    if (argc == 2) {
        n = atoi(argv[1]);  // Convert string to integer
    } else {
        // Ask user for input
        printf("Enter the number of prime numbers to generate: ");
        scanf("%d", &n);
    }
    
    // Validate input
    if (n <= 0) {
        fprintf(stderr, "Error: Please provide a positive integer!\n");
        fprintf(stderr, "Usage: %s <positive_number>\n", argv[0]);
        return 1;
    }
    
    printf("\n========================================\n");
    printf("Generating first %d prime numbers\n", n);
    printf("========================================\n");
    
    // Create child process
    pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // CHILD PROCESS
        printf("\n[CHILD PROCESS STARTED]\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
        
        // Generate prime numbers in child
        generatePrimes(n);
        
        printf("\nChild Process: Completed prime generation.\n");
        printf("Child Process: Exiting...\n");
        exit(0);  // Child exits successfully
    }
    else {
        // PARENT PROCESS
        printf("\n[PARENT PROCESS]\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        printf("Parent: Waiting for child to complete...\n");
        
        // Wait for child to complete
        wait(&status);
        
        // Check exit status
        if (WIFEXITED(status)) {
            printf("\nParent: Child completed successfully.\n");
            printf("Parent: Child exit status: %d\n", WEXITSTATUS(status));
        } else {
            printf("\nParent: Child did not exit normally.\n");
        }
        
        printf("Parent: Exiting program.\n");
    }
    
    return 0;
}

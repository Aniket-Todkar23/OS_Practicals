# Problem 2.3 - Prime Number Generation
## Fork + Wait + Command Line Arguments

---

## 📋 Problem Statement

Write a C program using the `fork()` system call that generates prime numbers in the **child process**. The number of primes will be provided from the **command line**. The parent must invoke `wait()` to wait for the child to complete.

**Example:** If 10 is passed as parameter, output: `2, 3, 5, 7, 11, 13, 17, 19, 23, 29`

---

## 🎯 Key Concepts

### **1. Why Child Must Output the Sequence?**

```c
fork() creates TWO processes with SEPARATE memory spaces

Parent's data ≠ Child's data

If child generates primes, parent CANNOT see them!
(Unless using IPC - Inter-Process Communication)
```

**Solution:** Child must print the primes itself.

---

## 🔹 Program Structure

```
┌──────────────────────────────────────┐
│  Main Program                        │
├──────────────────────────────────────┤
│  1. Read n (command line or input)  │
│  2. Validate n > 0                   │
│  3. fork()                           │
├──────────────────────────────────────┤
│  CHILD (pid == 0)                    │
│    - generatePrimes(n)               │
│    - Print primes                    │
│    - exit(0)                         │
├──────────────────────────────────────┤
│  PARENT (pid > 0)                    │
│    - wait(&status)                   │
│    - Check exit status               │
│    - exit                            │
└──────────────────────────────────────┘
```

---

## 🔹 Prime Number Algorithm

### **isPrime() Function:**

```c
int isPrime(int n) {
    if (n <= 1) return 0;        // 0, 1 not prime
    if (n == 2) return 1;        // 2 is prime
    if (n % 2 == 0) return 0;    // Even numbers not prime
    
    // Check odd divisors up to √n
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;  // Prime!
}
```

**Why check only up to √n?**
- If n has a divisor > √n, it must also have a divisor < √n
- Example: 36 = 6 × 6. If checking 6, we find it's divisible.

### **generatePrimes() Function:**

```c
void generatePrimes(int count) {
    int found = 0;
    int num = 2;  // Start from first prime
    
    while (found < count) {
        if (isPrime(num)) {
            printf("%d ", num);
            found++;
        }
        num++;
    }
}
```

**Logic:**
1. Start from 2 (first prime)
2. Check each number if it's prime
3. If prime, print it and increment counter
4. Continue until we find 'count' primes

---

## 🔹 Command Line Arguments

### **What is argc and argv?**

```c
int main(int argc, char *argv[])
```

- **argc** = Argument Count (number of arguments)
- **argv** = Argument Vector (array of strings)

### **Example:**

```bash
./problem_2_3 10
```

```
argc = 2
argv[0] = "./problem_2_3"  (program name)
argv[1] = "10"             (first argument)
```

### **Reading Command Line Argument:**

```c
if (argc == 2) {
    n = atoi(argv[1]);  // Convert string "10" to int 10
}
```

---

## 🔹 Error Checking

### **1. Positive Integer Check:**

```c
if (n <= 0) {
    fprintf(stderr, "Error: Please provide a positive integer!\n");
    return 1;
}
```

### **2. Fork Failure:**

```c
if (pid < 0) {
    perror("Fork failed");
    exit(1);
}
```

---

## 🔹 Process Flow

### **Timeline Diagram:**

```
Time 0s:  Parent reads n = 10
Time 0s:  fork() → Parent + Child created

          ┌─────────────────┐         ┌─────────────────┐
          │  PARENT          │         │  CHILD          │
          │  PID: 1234       │         │  PID: 1235      │
          ├─────────────────┤         ├─────────────────┤
Time 0s:  │ wait(&status)   │         │ generatePrimes  │
          │ (BLOCKED)       │         │ Check 2: Prime  │
          │      ⏳         │         │ Print: 2        │
          │                 │         │ Check 3: Prime  │
          │                 │         │ Print: 3        │
          │                 │         │ ... (8 more)    │
          │                 │         │ Print: 29       │
Time 2s:  │                 │◄────────│ exit(0)         │
          │ UNBLOCKED       │         └─────────────────┘
          │ Check status    │
          │ exit            │
          └─────────────────┘
```

---

## 💡 VIVA Questions & Answers

### **Q1: Why does child generate primes, not parent?**
**A:** Because parent and child have separate memory spaces after fork(). If parent generates primes, child won't have access to them. The problem requires child to output the sequence.

### **Q2: What does wait() do?**
**A:** 
1. Blocks parent until child exits
2. Retrieves child's exit status
3. Reaps the child (removes from process table)
4. Prevents zombie process

### **Q3: What is atoi()?**
**A:** Converts ASCII string to integer. 
- `atoi("10")` returns `10`
- `atoi("abc")` returns `0`

### **Q4: Why check if n is positive?**
**A:** Negative or zero prime count makes no sense. We validate input to prevent errors and provide meaningful error messages.

### **Q5: What happens if we don't call wait()?**
**A:** 
- Child becomes zombie after exit
- Parent may exit before child completes
- No guarantee of proper synchronization

### **Q6: Can we use waitpid() instead of wait()?**
**A:** Yes! `waitpid(pid, &status, 0)` waits for specific child PID.

### **Q7: What is WIFEXITED() and WEXITSTATUS()?**
**A:**
- **WIFEXITED(status)**: Checks if child exited normally (returns true/false)
- **WEXITSTATUS(status)**: Gets the exit code (0 = success, non-zero = error)

### **Q8: How is the prime algorithm efficient?**
**A:** 
- Checks divisibility only up to √n (reduces checks)
- Skips even numbers after 2
- Time complexity: O(√n) per number

---

## 📝 Sample Execution

### **Method 1: Command Line Argument**

```bash
$ gcc problem_2_3.c -o problem_2_3
$ ./problem_2_3 10

╔═══════════════════════════════════════════════╗
║  Problem 2.3: Prime Number Generation        ║
║  Using fork() and wait()                      ║
╚═══════════════════════════════════════════════╝

========================================
Generating first 10 prime numbers
========================================

[PARENT PROCESS]
Parent PID: 1234
Child PID: 1235
Parent: Waiting for child to complete...

[CHILD PROCESS STARTED]
Child PID: 1235
Parent PID: 1234

Child Process: Generating first 10 prime numbers:
Primes: 2 3 5 7 11 13 17 19 23 29 

Child Process: Completed prime generation.
Child Process: Exiting...

Parent: Child completed successfully.
Parent: Child exit status: 0
Parent: Exiting program.
```

### **Method 2: Interactive Input**

```bash
$ ./problem_2_3

Enter the number of prime numbers to generate: 5

Primes: 2 3 5 7 11
```

### **Error Case:**

```bash
$ ./problem_2_3 -5

Error: Please provide a positive integer!
Usage: ./problem_2_3 <positive_number>
```

---

## 🔑 Key Takeaways

1. **Child outputs data** because parent can't access child's memory
2. **wait()** ensures parent waits for child to complete
3. **Command line args** provide flexible input method
4. **Error checking** validates input before processing
5. **Prime algorithm** is optimized (check up to √n)

---

## 🎓 One-Line Answers for Viva

- **fork():** "Creates child process, returns 0 to child, child PID to parent"
- **wait():** "Blocks parent until child exits, retrieves exit status"
- **argc/argv:** "Command line argument count and array of strings"
- **atoi():** "Converts string to integer"
- **Prime check:** "Number divisible only by 1 and itself"
- **Why child outputs:** "Separate memory spaces after fork"

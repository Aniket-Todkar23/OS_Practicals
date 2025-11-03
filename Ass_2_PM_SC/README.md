# Assignment 2: Process Management & System Calls

## 📚 Problems Covered

- **Problem 2.1**: Fork, Wait, Bubble Sort, Insertion Sort (Zombie & Orphan)
- **Problem 2.2**: Fork + Execve + Binary Search *(Not yet implemented)*
- **Problem 2.3**: Prime Number Generation using Fork & Wait

---

## 📁 Files Structure

```
Ass_2_PM_SC/
├── problem_2_1_normal.c          # Normal fork + wait
├── problem_2_1_zombie.c          # Zombie process demo
├── problem_2_1_orphan.c          # Orphan process demo
├── problem_2_1_menu.c            # Menu-driven (all 3 scenarios)
├── problem_2_3.c                 # Prime number generation
├── PROBLEM_2_1_EXPLANATION.md    # Detailed explanation
├── PROBLEM_2_3_EXPLANATION.md    # Detailed explanation
└── README.md                     # This file
```

---

## 🔧 Compilation & Execution

### **Problem 2.1 - Individual Programs**

```bash
# Normal execution
gcc problem_2_1_normal.c -o normal
./normal

# Zombie demonstration
gcc problem_2_1_zombie.c -o zombie
./zombie

# Orphan demonstration
gcc problem_2_1_orphan.c -o orphan
./orphan
```

### **Problem 2.1 - Menu Driven (Recommended)**

```bash
gcc problem_2_1_menu.c -o menu
./menu
```

**Menu Options:**
1. Normal Execution (Fork + Wait)
2. Zombie Process Demonstration
3. Orphan Process Demonstration
4. Exit

### **Problem 2.3 - Prime Numbers**

```bash
gcc problem_2_3.c -o primes

# Method 1: Command line argument
./primes 10

# Method 2: Interactive input
./primes
```

---

## 📖 Quick Concepts

### **Problem 2.1 Summary**

| Scenario | Child Behavior | Parent Behavior | Result |
|----------|---------------|----------------|--------|
| **Normal** | Exits normally | Calls wait() immediately | Proper cleanup |
| **Zombie** | Exits quickly | Delays wait() by 10 sec | Child becomes zombie |
| **Orphan** | Sleeps 5 sec | Exits without wait() | Child adopted by init |

### **Problem 2.3 Summary**

- Child generates first N prime numbers
- Parent waits for child to complete
- Accepts input via command line or interactive

---

## 🎯 Viva Quick Reference

### **fork() Return Values**
- `< 0` → Fork failed
- `= 0` → You are in **CHILD**
- `> 0` → You are in **PARENT** (value = child's PID)

### **System Calls Used**
- `fork()` - Create child process
- `wait(&status)` - Wait for child to exit
- `getpid()` - Get current process ID
- `getppid()` - Get parent process ID
- `exit(code)` - Terminate process

### **Process States**
- **Zombie**: Child terminated, parent hasn't called wait()
- **Orphan**: Parent terminated, child still running
- **Running**: Actively executing
- **Waiting**: Blocked on wait()

---

## 🔍 How to Verify Zombie/Orphan

### **Verify Zombie:**
```bash
# In first terminal
./zombie

# In second terminal (during 10-sec sleep)
ps aux | grep Z
# OR
ps -l  # Look for 'Z' in STAT column
```

### **Verify Orphan:**
```bash
# Run orphan program
./orphan

# Child will print its PPID
# Before parent exits: PPID = parent's PID
# After parent exits: PPID = 1 (or systemd PID)
```

---

## 📊 Sample Output

### **Problem 2.1 - Normal**
```
Enter the number of integers: 5
Enter 5 integers:
5 2 8 1 9

Original array: 5 2 8 1 9

[PARENT PROCESS]
Parent PID: 1234, Child PID: 1235
Parent: Starting Bubble Sort...
Parent: Sorted array: 1 2 5 8 9

[CHILD PROCESS]
Child PID: 1235, Parent PID: 1234
Child: Starting Insertion Sort...
Child: Sorted array: 1 2 5 8 9
Child: Exiting...

Parent: Child completed with status 0
```

### **Problem 2.3 - Prime Numbers**
```
$ ./primes 10

Generating first 10 prime numbers

[CHILD PROCESS STARTED]
Child PID: 1235
Primes: 2 3 5 7 11 13 17 19 23 29

Parent: Child completed successfully.
```

---

## ⚠️ Important Notes

1. **These programs require POSIX environment:**
   - Linux, macOS, WSL (Windows Subsystem for Linux)
   - Won't compile on native Windows without modifications

2. **Zombie demonstration:**
   - Open second terminal quickly during 10-second sleep
   - Use `ps` commands to see zombie state

3. **Orphan demonstration:**
   - Parent exits first, returns to shell prompt
   - Child continues running in background
   - Child output appears after prompt

4. **Menu-driven program:**
   - Most convenient for demonstration
   - All three scenarios in one program

---

## 🎓 Learning Objectives

After completing these programs, you should understand:

✅ How `fork()` creates processes  
✅ How `wait()` synchronizes parent-child  
✅ Difference between zombie and orphan  
✅ How to pass data via command line arguments  
✅ Process states and transitions  
✅ Sorting algorithms (bubble sort, insertion sort)  
✅ Prime number generation algorithm  

---

## 📞 For Viva - Key Points

1. **Always explain fork() return values**
2. **Explain why wait() is necessary**
3. **Know how to create zombie vs orphan**
4. **Understand separate memory spaces after fork()**
5. **Be ready to trace process execution timeline**

---

## 🚀 Next Steps

- [ ] Complete Problem 2.2 (Fork + Execve + Binary Search)
- [ ] Practice explaining each program
- [ ] Draw process diagrams on paper
- [ ] Test all programs thoroughly
- [ ] Prepare for viva questions

---

**Good Luck! 🎉**

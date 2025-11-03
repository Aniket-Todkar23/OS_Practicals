# Problem 2.1 - Complete Explanation
## Fork, Wait, Zombie & Orphan States

---

## 📁 Three Programs Created:

1. **problem_2_1_normal.c** - Normal execution with wait()
2. **problem_2_1_zombie.c** - Demonstrates ZOMBIE process
3. **problem_2_1_orphan.c** - Demonstrates ORPHAN process

---

## 🎯 VIVA Questions & Answers

### **Q1: What is the difference between Zombie and Orphan?**

| Aspect | Zombie Process | Orphan Process |
|--------|----------------|----------------|
| **Definition** | Child exits but parent hasn't called wait() | Parent exits before child completes |
| **State** | Terminated but not reaped | Still running |
| **Parent Status** | Parent alive but sleeping/busy | Parent is dead |
| **PPID** | Original parent PID | Changes to 1 (init/systemd) |
| **Memory** | Holds minimal resources (PCB entry) | Full process, continues execution |
| **How to detect** | `ps` shows status 'Z' | Check PPID = 1 |
| **Problem** | Wastes process table space | No problem - adopted by init |

---

## 📋 Scenario 1: NORMAL EXECUTION

### **Code Flow:**
```
Parent creates child → Both sort → Parent waits → Child exits → Parent exits
```

### **Timeline:**
```
Time 0s:  fork() → Parent + Child exist
Time 0s:  Parent: Bubble Sort
Time 0s:  Child: Insertion Sort
Time 1s:  Child exits
Time 1s:  Parent: wait() ✓ (reaps child immediately)
Time 1s:  Parent exits
```

### **Key Point:**
- Parent calls `wait()` → Child is reaped properly
- **NO zombie, NO orphan**

---

## 📋 Scenario 2: ZOMBIE PROCESS

### **How Zombie is Created:**
```c
// Child exits QUICKLY
exit(0);

// Parent DELAYS calling wait()
sleep(10);  // Child is zombie during this time!
wait(&status);  // Finally reaps zombie
```

### **Timeline:**
```
Time 0s:  fork() → Parent + Child exist
Time 1s:  Child: Sorts quickly and exits ⚰️
Time 1s:  Parent: Still sorting...
Time 1-10s: *** CHILD IS ZOMBIE *** (can verify with ps -l)
Time 10s: Parent: wait() → Reaps zombie ✓
```

### **Zombie Characteristics:**
- Child has **finished execution** (`exit(0)`)
- Parent is **alive but hasn't called wait()**
- Process entry still in process table
- Shows as `<defunct>` or status `Z` in `ps`

### **How to Verify:**
```bash
# In one terminal:
./problem_2_1_zombie

# In another terminal (during 10-second sleep):
ps aux | grep Z
# or
ps -l  # Look for 'Z' in STAT column
```

### **VIVA Answer:**
> "A zombie process is a terminated child whose exit status hasn't been collected by parent using wait(). It holds a process table entry but no longer executes. In my code, child exits immediately but parent sleeps for 10 seconds before calling wait(), creating a zombie during this period."

---

## 📋 Scenario 3: ORPHAN PROCESS

### **How Orphan is Created:**
```c
// Parent exits WITHOUT calling wait()
exit(0);  // Parent dies

// Child is still running
sleep(5);  // Child continues alone
```

### **Timeline:**
```
Time 0s:  fork() → Parent + Child exist
Time 0s:  Child: sleep(5) 😴
Time 1s:  Parent: Sorts and exits ☠️ (NO wait() call!)
Time 1-5s: *** CHILD IS ORPHAN *** (PPID changes to 1)
Time 5s:  Child: Wakes up, checks PPID (now = 1 or systemd)
Time 6s:  Child: Completes sorting and exits
```

### **Orphan Characteristics:**
- Parent has **died/exited** before child finished
- Child is **still running** (not terminated)
- **Init (PID 1) or systemd adopts the child**
- Child's PPID changes from original parent to 1

### **How to Verify:**
```c
printf("Child: My current Parent PID: %d\n", getppid());
// Before parent exits: Shows parent's PID (e.g., 1234)
// After parent exits: Shows 1 or systemd PID (e.g., 1 or 1000)
```

### **VIVA Answer:**
> "An orphan process is a child whose parent has terminated before the child completed. The init process (PID 1) automatically adopts orphans. In my code, parent exits quickly without wait(), while child sleeps for 5 seconds, making it an orphan. I verify this by checking PPID before and after parent exits."

---

## 🔑 Key Differences Summary

### **Creating Zombie:**
```c
if (pid == 0) {
    // Child: finish QUICKLY
    exit(0);
} else {
    // Parent: DELAY before wait()
    sleep(10);
    wait(&status);
}
```

### **Creating Orphan:**
```c
if (pid == 0) {
    // Child: Take LONG time
    sleep(10);
    // work...
    exit(0);
} else {
    // Parent: exit QUICKLY (NO wait!)
    exit(0);
}
```

---

## 💡 VIVA Questions You Should Know

### **Q: Why are zombies bad?**
A: They waste process table entries. If many zombies accumulate, system may run out of PIDs.

### **Q: How to remove zombie?**
A: Parent must call `wait()` or `waitpid()`. If parent refuses, killing parent removes zombies.

### **Q: Are orphans bad?**
A: No! Init properly manages them. It's a normal mechanism.

### **Q: What does wait() do?**
A: 
1. Blocks parent until child exits
2. Retrieves child's exit status
3. Removes child from process table (reaps)

### **Q: Can you have zombie orphan?**
A: Technically yes - if parent dies and child becomes zombie before init reaps it. But init calls wait() immediately, so rare.

### **Q: What is init process?**
A: PID 1, first process started by kernel. Adopts all orphans and reaps them properly.

---

## 📝 Compilation & Execution

```bash
# Compile all three
gcc problem_2_1_normal.c -o normal
gcc problem_2_1_zombie.c -o zombie
gcc problem_2_1_orphan.c -o orphan

# Run
./normal   # Normal execution
./zombie   # Shows zombie for 10 seconds
./orphan   # Child becomes orphan
```

---

## 🎓 For Viva - One-Line Answers

**Zombie:** "Child dead, parent alive but didn't call wait()"

**Orphan:** "Parent dead, child still running, adopted by init"

**wait():** "Blocks parent, reaps child, gets exit status"

**fork():** "Creates child process, returns 0 to child, child PID to parent"

**getpid():** "Returns my own process ID"

**getppid():** "Returns my parent's process ID"

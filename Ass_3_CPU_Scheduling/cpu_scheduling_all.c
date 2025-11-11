#include <stdio.h>

struct Process {
    int pid;
    int at;
    int bt;
    int rem;
    int ct;
    int tat;
    int wt;
};

// ========== 1. FCFS ==========
void fcfs(struct Process p[], int n) {
    int time = 0;
    float total_wt = 0, total_tat = 0;
    
    printf("\n========== FCFS SCHEDULING ==========\n");
    
    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    
    // Execute each process
    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;
        
        time += p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }
    
    // Display
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < n; i++)
        printf(" P%d |", p[i].pid);
    printf("\n0");
    for (int i = 0; i < n; i++)
        printf("   %d", p[i].ct);
    printf("\n");
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

// ========== 2. SJF Non-Preemptive ==========
void sjf_non_preemptive(struct Process p[], int n) {
    int time = 0;
    int completed = 0;
    int done[10] = {0};
    float total_wt = 0, total_tat = 0;
    struct Process order[10];
    
    printf("\n========== SJF (Non-Preemptive) SCHEDULING ==========\n");
    
    while (completed < n) {
        int shortest = -1;
        int min_bt = 9999;
        
        // Find shortest job that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && done[i] == 0) {
                if (p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    shortest = i;
                }
            }
        }
        
        if (shortest == -1) {
            time++;
        } else {
            time += p[shortest].bt;
            p[shortest].ct = time;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
            
            total_wt += p[shortest].wt;
            total_tat += p[shortest].tat;
            
            order[completed] = p[shortest];
            done[shortest] = 1;
            completed++;
        }
    }
    
    // Display
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < n; i++)
        printf(" P%d |", order[i].pid);
    printf("\n0");
    for (int i = 0; i < n; i++)
        printf("   %d", order[i].ct);
    printf("\n");
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

// ========== 3. SJF Preemptive (SRTF) ==========
void sjf_preemptive(struct Process p[], int n) {
    int time = 0;
    int completed = 0;
    float total_wt = 0, total_tat = 0;
    
    printf("\n========== SJF (Preemptive / SRTF) SCHEDULING ==========\n");
    
    // Initialize remaining time
    for (int i = 0; i < n; i++)
        p[i].rem = p[i].bt;
    
    // Find max time
    int max_time = 0;
    for (int i = 0; i < n; i++)
        if (p[i].at + p[i].bt > max_time)
            max_time = p[i].at + p[i].bt;
    max_time += 100;
    
    // Execute
    while (completed < n && time < max_time) {
        int shortest = -1;
        int min_rem = 9999;
        
        // Find process with shortest remaining time
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rem > 0) {
                if (p[i].rem < min_rem) {
                    min_rem = p[i].rem;
                    shortest = i;
                }
            }
        }
        
        if (shortest == -1) {
            time++;
        } else {
            p[shortest].rem--;
            time++;
            
            // Check if completed
            if (p[shortest].rem == 0) {
                p[shortest].ct = time;
                p[shortest].tat = p[shortest].ct - p[shortest].at;
                p[shortest].wt = p[shortest].tat - p[shortest].bt;
                
                total_wt += p[shortest].wt;
                total_tat += p[shortest].tat;
                completed++;
            }
        }
    }
    
    // Display
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

// ========== 4. Round Robin ==========
void round_robin(struct Process p[], int n) {
    int time = 0;
    int completed = 0;
    int quantum = 2;
    float total_wt = 0, total_tat = 0;
    
    printf("\n========== ROUND ROBIN (Quantum = 2) SCHEDULING ==========\n");
    
    // Initialize remaining time
    for (int i = 0; i < n; i++)
        p[i].rem = p[i].bt;
    
    // Execute
    while (completed < n) {
        int flag = 0;
        
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rem > 0) {
                flag = 1;
                
                if (p[i].rem > quantum) {
                    time += quantum;
                    p[i].rem -= quantum;
                } else {
                    time += p[i].rem;
                    p[i].rem = 0;
                    
                    p[i].ct = time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    
                    total_wt += p[i].wt;
                    total_tat += p[i].tat;
                    completed++;
                }
            }
        }
        
        if (flag == 0)
            time++;
    }
    
    // Display
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

// ========== MAIN ==========
int main() {
    struct Process p[10], copy[10];
    int n, choice;
    
    printf("=====================================================\n");
    printf("       CPU SCHEDULING ALGORITHMS SIMULATOR\n");
    printf("=====================================================\n");
    
    // Input
    printf("\nEnter number of processes: ");
    scanf("%d", &n);
    
    printf("\nEnter Process Details:\n");
    printf("(Format: Arrival_Time Burst_Time)\n\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d - Arrival Time: ", i + 1);
        scanf("%d", &p[i].at);
        printf("P%d - Burst Time: ", i + 1);
        scanf("%d", &p[i].bt);
    }
    
    // Menu
    while (1) {
        printf("\n=====================================================\n");
        printf("                     MENU\n");
        printf("=====================================================\n");
        printf("1. FCFS (First Come First Serve)\n");
        printf("2. SJF (Non-Preemptive)\n");
        printf("3. SJF (Preemptive / SRTF)\n");
        printf("4. Round Robin (Quantum = 2)\n");
        printf("5. Exit\n");
        printf("=====================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        // Copy original data
        for (int i = 0; i < n; i++)
            copy[i] = p[i];
        
        switch (choice) {
            case 1:
                fcfs(copy, n);
                break;
            case 2:
                sjf_non_preemptive(copy, n);
                break;
            case 3:
                sjf_preemptive(copy, n);
                break;
            case 4:
                round_robin(copy, n);
                break;
            case 5:
                printf("\n✓ Thank you for using the simulator!\n\n");
                return 0;
            default:
                printf("\n✗ Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}

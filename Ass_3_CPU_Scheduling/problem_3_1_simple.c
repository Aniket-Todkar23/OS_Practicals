#include <stdio.h>

// Simple structure for process
struct Process {
    int pid;        // Process ID (P1, P2, etc)
    int at;         // Arrival Time
    int bt;         // Burst Time
    int ct;         // Completion Time
    int tat;        // Turnaround Time = CT - AT
    int wt;         // Waiting Time = TAT - BT
};

// Function 1: FCFS Scheduling
void fcfs(struct Process p[], int n) {
    int time = 0;  // Current time
    float total_wt = 0, total_tat = 0;
    
    printf("\n===== FCFS SCHEDULING =====\n");
    
    // Sort by arrival time (simple bubble sort)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    
    // Calculate times for each process
    for (int i = 0; i < n; i++) {
        // If CPU idle, jump to next arrival
        if (time < p[i].at) {
            time = p[i].at;
        }
        
        // Process executes
        time = time + p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }
    
    // Display results
    printf("\nP_ID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    
    // Gantt Chart
    printf("\nGantt Chart:\n");
    printf("|");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", p[i].pid);
    }
    printf("\n0");
    for (int i = 0; i < n; i++) {
        printf("    %d", p[i].ct);
    }
    printf("\n");
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

// Function 2: SJF Non-Preemptive
void sjf(struct Process p[], int n) {
    int time = 0;
    int completed = 0;
    int done[10] = {0};  // Mark which process is done
    float total_wt = 0, total_tat = 0;
    struct Process order[10];  // Store execution order
    
    printf("\n===== SJF (Non-Preemptive) SCHEDULING =====\n");
    
    while (completed < n) {
        int shortest = -1;
        int min_bt = 9999;
        
        // Find process with shortest burst time that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && done[i] == 0) {
                if (p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    shortest = i;
                }
            }
        }
        
        if (shortest == -1) {
            // No process available, CPU idle
            time++;
        } else {
            // Execute this process
            time = time + p[shortest].bt;
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
    
    // Display results
    printf("\nP_ID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    
    // Gantt Chart
    printf("\nGantt Chart:\n");
    printf("|");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", order[i].pid);
    }
    printf("\n0");
    for (int i = 0; i < n; i++) {
        printf("    %d", order[i].ct);
    }
    printf("\n");
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

int main() {
    struct Process p[10], copy[10];
    int n, choice;
    
    printf("========================================\n");
    printf("  CPU Scheduling - Problem 3.1\n");
    printf("========================================\n");
    
    // Input number of processes
    printf("\nEnter number of processes: ");
    scanf("%d", &n);
    
    // Input process details
    printf("\nEnter Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d - Arrival Time: ", p[i].pid);
        scanf("%d", &p[i].at);
        printf("P%d - Burst Time: ", p[i].pid);
        scanf("%d", &p[i].bt);
    }
    
    // Menu
    while (1) {
        printf("\n========================================\n");
        printf("           MENU\n");
        printf("========================================\n");
        printf("1. FCFS Scheduling\n");
        printf("2. SJF (Non-Preemptive) Scheduling\n");
        printf("3. Exit\n");
        printf("========================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        // Make copy of original data
        for (int i = 0; i < n; i++) {
            copy[i] = p[i];
        }
        
        switch (choice) {
            case 1:
                fcfs(copy, n);
                break;
            case 2:
                sjf(copy, n);
                break;
            case 3:
                printf("\nThank you!\n");
                return 0;
            default:
                printf("\nInvalid choice!\n");
        }
    }
    
    return 0;
}

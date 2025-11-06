#include <stdio.h>

// Simple structure for process
struct Process {
    int pid;        // Process ID (P1, P2, etc)
    int at;         // Arrival Time
    int bt;         // Burst Time (original)
    int rem_bt;     // Remaining Burst Time (for preemptive)
    int ct;         // Completion Time
    int tat;        // Turnaround Time = CT - AT
    int wt;         // Waiting Time = TAT - BT
};

// Function 1: FCFS Scheduling (Same as 3.1)
void fcfs(struct Process p[], int n) {
    int time = 0;
    float total_wt = 0, total_tat = 0;
    
    printf("\n===== FCFS SCHEDULING =====\n");
    
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
    
    // Calculate times
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) {
            time = p[i].at;
        }
        
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

// Function 2: SJF Preemptive (SRTF - Shortest Remaining Time First)
void sjf_preemptive(struct Process p[], int n) {
    int time = 0;
    int completed = 0;
    int prev_process = -1;
    float total_wt = 0, total_tat = 0;
    
    // Store Gantt chart
    int gantt[100];      // Which process runs
    int gantt_time[100]; // Time points
    int gantt_count = 0;
    
    printf("\n===== SJF (Preemptive/SRTF) SCHEDULING =====\n");
    
    // Initialize remaining burst time
    for (int i = 0; i < n; i++) {
        p[i].rem_bt = p[i].bt;
    }
    
    // Find maximum completion time to loop until
    int max_time = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].at + p[i].bt > max_time) {
            max_time = p[i].at + p[i].bt;
        }
    }
    max_time += 100; // Safety margin
    
    // Simulate each time unit
    while (completed < n && time < max_time) {
        int shortest = -1;
        int min_rem_bt = 9999;
        
        // Find process with shortest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rem_bt > 0) {
                if (p[i].rem_bt < min_rem_bt) {
                    min_rem_bt = p[i].rem_bt;
                    shortest = i;
                }
                // If same remaining time, choose the one that arrived first
                else if (p[i].rem_bt == min_rem_bt && shortest != -1) {
                    if (p[i].at < p[shortest].at) {
                        shortest = i;
                    }
                }
            }
        }
        
        if (shortest == -1) {
            // CPU idle
            time++;
        } else {
            // Execute this process for 1 time unit
            
            // Record for Gantt chart
            if (prev_process != shortest) {
                gantt[gantt_count] = shortest;
                gantt_time[gantt_count] = time;
                gantt_count++;
                prev_process = shortest;
            }
            
            p[shortest].rem_bt--;
            time++;
            
            // Check if this process is completed
            if (p[shortest].rem_bt == 0) {
                p[shortest].ct = time;
                p[shortest].tat = p[shortest].ct - p[shortest].at;
                p[shortest].wt = p[shortest].tat - p[shortest].bt;
                
                total_wt += p[shortest].wt;
                total_tat += p[shortest].tat;
                completed++;
            }
        }
    }
    
    // Add final time to Gantt
    gantt_time[gantt_count] = time;
    
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
    for (int i = 0; i < gantt_count; i++) {
        printf(" P%d |", p[gantt[i]].pid);
    }
    printf("\n");
    for (int i = 0; i <= gantt_count; i++) {
        printf("%d    ", gantt_time[i]);
    }
    printf("\n");
    
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}

int main() {
    struct Process p[10], copy[10];
    int n, choice;
    
    printf("========================================\n");
    printf("  CPU Scheduling - Problem 3.2\n");
    printf("  FCFS & SJF (Preemptive)\n");
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
        printf("2. SJF (Preemptive) Scheduling\n");
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
                sjf_preemptive(copy, n);
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

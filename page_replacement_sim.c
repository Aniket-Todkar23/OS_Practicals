#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Display current frames
void display_frames(int frame[], int frames) {
    printf("| ");
    for (int i = 0; i < frames; i++) {
        if (frame[i] == -1)
            printf("- ");
        else
            printf("%d ", frame[i]);
    }
    printf("|");
}

// FIFO Page Replacement
void fifo(int pages[], int n, int frames) {
    printf("\n=== FIFO Algorithm ===\n");
    
    int frame[frames];
    int front = 0;
    int page_faults = 0;
    
    for (int i = 0; i < frames; i++)
        frame[i] = -1;
    
    printf("Ref | Frames | Fault?\n");
    printf("----|--------|-------\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        
        for (int j = 0; j < frames; j++) {
            if (frame[j] == page) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            frame[front] = page;
            front = (front + 1) % frames;
            page_faults++;
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf(" YES\n");
        } else {
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf(" NO\n");
        }
    }
    
    printf("\nTotal Page Faults: %d\n", page_faults);
}

// LRU Page Replacement
void lru(int pages[], int n, int frames) {
    printf("\n=== LRU Algorithm ===\n");
    
    int frame[frames];
    int last_used[frames];
    int page_faults = 0;
    
    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        last_used[i] = -1;
    }
    
    printf("Ref | Frames | Fault?\n");
    printf("----|--------|-------\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        int empty_frame = -1;
        
        for (int j = 0; j < frames; j++) {
            if (frame[j] == page) {
                found = 1;
                last_used[j] = i;
                break;
            }
            if (frame[j] == -1 && empty_frame == -1)
                empty_frame = j;
        }
        
        if (!found) {
            page_faults++;
            
            if (empty_frame != -1) {
                frame[empty_frame] = page;
                last_used[empty_frame] = i;
            } else {
                int lru_index = 0;
                for (int j = 1; j < frames; j++) {
                    if (last_used[j] < last_used[lru_index])
                        lru_index = j;
                }
                frame[lru_index] = page;
                last_used[lru_index] = i;
            }
            
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf(" YES\n");
        } else {
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf(" NO\n");
        }
    }
    
    printf("\nTotal Page Faults: %d\n", page_faults);
}

// Optimal Page Replacement
void optimal(int pages[], int n, int frames) {
    printf("\n=== Optimal Algorithm ===\n");
    
    int frame[frames];
    int page_faults = 0;
    
    for (int i = 0; i < frames; i++)
        frame[i] = -1;
    
    printf("Ref | Frames | Fault?\n");
    printf("----|--------|-------\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        int empty_frame = -1;
        
        for (int j = 0; j < frames; j++) {
            if (frame[j] == page) {
                found = 1;
                break;
            }
            if (frame[j] == -1 && empty_frame == -1)
                empty_frame = j;
        }
        
        if (!found) {
            page_faults++;
            
            if (empty_frame != -1) {
                frame[empty_frame] = page;
            } else {
                int replace_index = 0;
                int farthest = -1;
                
                for (int j = 0; j < frames; j++) {
                    int next_use = -1;
                    for (int k = i + 1; k < n; k++) {
                        if (pages[k] == frame[j]) {
                            next_use = k;
                            break;
                        }
                    }
                    
                    if (next_use == -1) {
                        replace_index = j;
                        break;
                    }
                    
                    if (next_use > farthest) {
                        farthest = next_use;
                        replace_index = j;
                    }
                }
                
                frame[replace_index] = page;
            }
            
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf(" YES\n");
        } else {
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf(" NO\n");
        }
    }
    
    printf("\nTotal Page Faults: %d\n", page_faults);
}

int main() {
    while (1) {
        printf("\n======================================\n");
        printf("PAGE REPLACEMENT SIMULATOR\n");
        printf("======================================\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        
        int choice;
        scanf("%d", &choice);
        
        if (choice == 4) {
            printf("\nThank you!\n");
            break;
        }
        
        if (choice < 1 || choice > 3) {
            printf("Invalid choice!\n");
            continue;
        }
        
        // Get page reference string
        int n;
        printf("\nEnter number of page references: ");
        scanf("%d", &n);
        
        int pages[n];
        printf("Enter page reference string (space-separated): ");
        for (int i = 0; i < n; i++)
            scanf("%d", &pages[i]);
        
        // Get frame size
        int frames;
        printf("Enter number of frames: ");
        scanf("%d", &frames);
        
        if (frames < 1) frames = 1;
        
        // Run chosen algorithm
        if (choice == 1) fifo(pages, n, frames);
        else if (choice == 2) lru(pages, n, frames);
        else if (choice == 3) optimal(pages, n, frames);
    }
    
    return 0;
}

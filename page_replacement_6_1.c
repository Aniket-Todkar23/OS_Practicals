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
    printf("Page Reference String: ");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);
    printf("\n\n");
    
    int frame[frames];
    int front = 0;
    int page_faults = 0;
    
    // Initialize frames as empty (-1)
    for (int i = 0; i < frames; i++)
        frame[i] = -1;
    
    printf("Ref | Frames After | Fault?\n");
    printf("----|--------------|-------\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        
        // Check if page is already in frame
        for (int j = 0; j < frames; j++) {
            if (frame[j] == page) {
                found = 1;
                break;
            }
        }
        
        // If page not found, it's a page fault
        if (!found) {
            frame[front] = page;  // Replace page at front
            front = (front + 1) % frames;  // Move to next position (circular)
            page_faults++;
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf("   YES\n");
        } else {
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf("   NO\n");
        }
    }
    
    printf("\nTotal Page Faults: %d\n", page_faults);
}

// LRU Page Replacement
void lru(int pages[], int n, int frames) {
    printf("\n=== LRU Algorithm ===\n");
    printf("Page Reference String: ");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);
    printf("\n\n");
    
    int frame[frames];
    int last_used[frames];  // Track when each frame was last used
    int page_faults = 0;
    
    // Initialize
    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        last_used[i] = -1;
    }
    
    printf("Ref | Frames After | Fault?\n");
    printf("----|--------------|-------\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        int empty_frame = -1;
        
        // Check if page is already in frame
        for (int j = 0; j < frames; j++) {
            if (frame[j] == page) {
                found = 1;
                last_used[j] = i;  // Update last used time
                break;
            }
            // Track first empty frame
            if (frame[j] == -1 && empty_frame == -1)
                empty_frame = j;
        }
        
        // If page not found, it's a page fault
        if (!found) {
            page_faults++;
            
            // If there's an empty frame, use it
            if (empty_frame != -1) {
                frame[empty_frame] = page;
                last_used[empty_frame] = i;
            } else {
                // Find least recently used frame
                int lru_index = 0;
                for (int j = 1; j < frames; j++) {
                    if (last_used[j] < last_used[lru_index])
                        lru_index = j;
                }
                
                // Replace LRU page
                frame[lru_index] = page;
                last_used[lru_index] = i;
            }
            
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf("   YES\n");
        } else {
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf("   NO\n");
        }
    }
    
    printf("\nTotal Page Faults: %d\n", page_faults);
}

// Optimal Page Replacement
void optimal(int pages[], int n, int frames) {
    printf("\n=== Optimal Algorithm ===\n");
    printf("Page Reference String: ");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);
    printf("\n\n");
    
    int frame[frames];
    int page_faults = 0;
    
    // Initialize frames as empty (-1)
    for (int i = 0; i < frames; i++)
        frame[i] = -1;
    
    printf("Ref | Frames After | Fault?\n");
    printf("----|--------------|-------\n");
    
    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        int empty_frame = -1;
        
        // Check if page is already in frame
        for (int j = 0; j < frames; j++) {
            if (frame[j] == page) {
                found = 1;
                break;
            }
            // Track first empty frame
            if (frame[j] == -1 && empty_frame == -1)
                empty_frame = j;
        }
        
        // If page not found, it's a page fault
        if (!found) {
            page_faults++;
            
            // If there's an empty frame, use it
            if (empty_frame != -1) {
                frame[empty_frame] = page;
            } else {
                // Find the page that will be used farthest in future
                int replace_index = 0;
                int farthest = -1;
                
                for (int j = 0; j < frames; j++) {
                    int next_use = -1;
                    
                    // Find when this page will be used next
                    for (int k = i + 1; k < n; k++) {
                        if (pages[k] == frame[j]) {
                            next_use = k;
                            break;
                        }
                    }
                    
                    // If page is never used again, replace it immediately
                    if (next_use == -1) {
                        replace_index = j;
                        break;
                    }
                    
                    // Keep track of page used farthest in future
                    if (next_use > farthest) {
                        farthest = next_use;
                        replace_index = j;
                    }
                }
                
                // Replace the optimal page
                frame[replace_index] = page;
            }
            
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf("   YES\n");
        } else {
            printf(" %d  ", page);
            display_frames(frame, frames);
            printf("   NO\n");
        }
    }
    
    printf("\nTotal Page Faults: %d\n", page_faults);
}

int main() {
    int choice;
    printf("==========================================\n");
    printf("PAGE REPLACEMENT SIMULATION\n");
    printf("==========================================\n");
    printf("Choose option:\n");
    printf("1. Assignment 6.1 (Random string + FIFO & LRU)\n");
    printf("2. Assignment 6.2 (Given string + Optimal & LRU)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        // Assignment 6.1 - Random page reference string
        srand(time(0));
        
        int n = 15;
        int pages[n];
        
        printf("\nGenerated Random Page Reference String:\n");
        for (int i = 0; i < n; i++) {
            pages[i] = rand() % 10;
            printf("%d ", pages[i]);
        }
        printf("\n");
        
        int frames;
        printf("\nEnter number of frames (1-7): ");
        scanf("%d", &frames);
        
        if (frames < 1 || frames > 7) {
            printf("Invalid frame count. Using 3 frames.\n");
            frames = 3;
        }
        
        printf("\n=========================================\n");
        printf("Simulating with %d frames\n", frames);
        printf("=========================================\n");
        
        fifo(pages, n, frames);
        lru(pages, n, frames);
        
    } else if (choice == 2) {
        // Assignment 6.2 - Given page reference string
        int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};
        int n = 13;
        
        printf("\n=== Assignment 6.2 ===\n");
        printf("Given Page Reference String: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", pages[i]);
        }
        printf("\n\nTesting with frame size 3 and 4:\n");
        
        // Test with frame size 3
        printf("\n*** FRAME SIZE = 3 ***");
        optimal(pages, n, 3);
        lru(pages, n, 3);
        
        // Test with frame size 4
        printf("\n*** FRAME SIZE = 4 ***");
        optimal(pages, n, 4);
        lru(pages, n, 4);
        
    } else {
        printf("Invalid choice!\n");
        return 1;
    }
    
    printf("\n=========================================\n");
    printf("Simulation Complete!\n");
    printf("=========================================\n");
    
    return 0;
}

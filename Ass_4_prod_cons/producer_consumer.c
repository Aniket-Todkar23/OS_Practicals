#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Pizza counter capacity

// Shared buffer (Pizza Counter)
int buffer[BUFFER_SIZE];
int in = 0;   // Index where Simpson adds pizza
int out = 0;  // Index where Joey takes pizza

// Synchronization tools
pthread_mutex_t mutex;
sem_t empty;  // Counts empty spaces
sem_t full;   // Counts pizzas available

// Function to display buffer state
void display_buffer() {
    printf("   Counter: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == 0) {
            printf(" _ ");
        } else {
            printf("%2d ", buffer[i]);
        }
    }
    printf("]\n");
}

// Producer: Mr. Simpson (makes pizza)
void* simpson(void* arg) {
    int pizza_num;
    
    for (int i = 1; i <= 10; i++) {
        pizza_num = i;  // Pizza number
        
        // Wait if counter is full
        sem_wait(&empty);
        
        // Lock the counter
        pthread_mutex_lock(&mutex);
        
        // Add pizza to counter
        buffer[in] = pizza_num;
        printf("\nSimpson: Baked Pizza #%d (placed at position %d)\n", pizza_num, in);
        display_buffer();
        in = (in + 1) % BUFFER_SIZE;
        
        // Unlock the counter
        pthread_mutex_unlock(&mutex);
        
        // Signal: one more pizza available
        sem_post(&full);
        
        sleep(1);  // Time to bake next pizza
    }
    
    printf("\nSimpson: Done baking all pizzas!\n");
    return NULL;
}

// Consumer: Joey (eats pizza)
void* joey(void* arg) {
    int pizza_num;
    
    for (int i = 1; i <= 10; i++) {
        // Wait if counter is empty
        sem_wait(&full);
        
        // Lock the counter
        pthread_mutex_lock(&mutex);
        
        // Take pizza from counter
        pizza_num = buffer[out];
        buffer[out] = 0;  // Mark as empty
        printf("\nJoey: Ate Pizza #%d (taken from position %d)\n", pizza_num, out);
        display_buffer();
        out = (out + 1) % BUFFER_SIZE;
        
        // Unlock the counter
        pthread_mutex_unlock(&mutex);
        
        // Signal: one space now empty
        sem_post(&empty);
        
        sleep(2);  // Time to eat pizza
    }
    
    printf("\nJoey: Done eating all pizzas!\n");
    return NULL;
}

int main() {
    pthread_t simpson_thread, joey_thread;
    
    // Initialize buffer to 0
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all spaces empty
    sem_init(&full, 0, 0);              // Initially no pizzas
    
    printf("=== Pizza Shop Problem ===\n");
    printf("Counter capacity: %d pizzas\n", BUFFER_SIZE);
    printf("Initial Counter: [ _  _  _  _  _ ]\n");
    printf("==============================\n");
    
    // Create threads
    pthread_create(&simpson_thread, NULL, simpson, NULL);
    pthread_create(&joey_thread, NULL, joey, NULL);
    
    // Wait for threads to finish
    pthread_join(simpson_thread, NULL);
    pthread_join(joey_thread, NULL);
    
    // Cleanup
    // pthread_mutex_destroy(&mutex);
    // sem_destroy(&empty);
    // sem_destroy(&full);
    
    printf("\n=== Program Complete ===\n");
    
    return 0;
}

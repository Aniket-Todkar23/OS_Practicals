#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// Synchronization tools
pthread_mutex_t mutex;
sem_t value_ready;     // Producer signals: value is ready
sem_t value_consumed;  // Consumer signals: value consumed

#define FILENAME "shared_data.txt"
int NUM_ITEMS = 10;  // Number of items to produce/consume

// Producer: Generates random numbers and writes to file
void* producer(void* arg) {
    FILE *file;
    int value;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Generate random number (0-9)
        value = rand() % 10;
        
        // Wait until previous value is consumed
        sem_wait(&value_consumed);
        
        // Lock file access
        pthread_mutex_lock(&mutex);
        
        // Write to file
        file = fopen(FILENAME, "w");
        fprintf(file, "%d\n", value);
        fclose(file);
        
        printf("\nProducer: Generated and wrote %d to file\n", value);
        
        // Unlock file
        pthread_mutex_unlock(&mutex);
        
        // Signal: value is ready
        sem_post(&value_ready);
        
        // Sleep random time (0-100 milliseconds)
        usleep((rand() % 101) * 1000);
    }
    
    printf("\nProducer: Done generating!\n");
    return NULL;
}

// Consumer: Reads numbers from file
void* consumer(void* arg) {
    FILE *file;
    int value;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Wait until value is ready
        sem_wait(&value_ready);
        
        // Lock file access
        pthread_mutex_lock(&mutex);
        
        // Read from file
        file = fopen(FILENAME, "r");
        fscanf(file, "%d", &value);
        fclose(file);
        
        printf("Consumer: Read %d from file\n", value);
        
        // Unlock file
        pthread_mutex_unlock(&mutex);
        
        // Signal: value consumed
        sem_post(&value_consumed);
    }
    
    printf("\nConsumer: Done consuming!\n");
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    // Seed random number generator
    srand(time(NULL));
    
    printf("=== Producer-Consumer with File ===\n");
    printf("File: %s\n", FILENAME);
    printf("Items to produce: %d\n", NUM_ITEMS);
    printf("===================================\n");
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize semaphores
    sem_init(&value_ready, 0, 0);     // Initially no value ready
    sem_init(&value_consumed, 0, 1);  // Initially can produce
    
    // Create threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&value_ready);
    sem_destroy(&value_consumed);
    
    printf("\n=== Program Complete ===\n");
    
    return 0;
}

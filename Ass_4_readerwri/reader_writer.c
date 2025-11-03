#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Shared database
int database = 0;

// Synchronization tools
int readcount = 0;           // Number of readers reading
pthread_mutex_t mutex;       // Protects readcount
sem_t wrt;                   // Writer lock

// Reader function
void* reader(void* arg) {
    int reader_id = *((int*)arg);
    
    while (1) {
        // Entry Section
        pthread_mutex_lock(&mutex);
        readcount++;
        if (readcount == 1) {
            sem_wait(&wrt);  // First reader locks writers
        }
        pthread_mutex_unlock(&mutex);
        
        // Critical Section - READING
        printf("\n[Reader %d] Reading database value: %d\n", reader_id, database);
        printf("   Active readers: %d\n", readcount);
        sleep(1);  // Simulate reading time
        
        // Exit Section
        pthread_mutex_lock(&mutex);
        readcount--;
        if (readcount == 0) {
            sem_post(&wrt);  // Last reader unlocks writers
        }
        pthread_mutex_unlock(&mutex);
        
        printf("[Reader %d] Finished reading\n", reader_id);
        sleep(2);  // Wait before reading again
    }
    
    return NULL;
}

// Writer function
void* writer(void* arg) {
    int writer_id = *((int*)arg);
    
    while (1) {
        // Entry Section
        sem_wait(&wrt);  // Request exclusive access
        
        // Critical Section - WRITING
        database++;
        printf("\n>>> [Writer %d] Writing to database: %d <<<\n", writer_id, database);
        sleep(2);  // Simulate writing time
        
        // Exit Section
        sem_post(&wrt);  // Release exclusive access
        
        printf(">>> [Writer %d] Finished writing <<<\n", writer_id);
        sleep(3);  // Wait before writing again
    }
    
    return NULL;
}

int main() {
    pthread_t readers[3], writers[2];
    int reader_ids[3] = {1, 2, 3};
    int writer_ids[2] = {1, 2};
    
    printf("=== Reader-Writer Problem (Readers Priority) ===\n");
    printf("Database initial value: %d\n", database);
    printf("===============================================\n");
    
    // Initialize synchronization tools
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);  // Binary semaphore (1 = available)
    
    // Create reader threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    
    // Create writer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    // Let it run for 20 seconds
    sleep(20);
    
    printf("\n\n=== Program Complete ===\n");
    
    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);
    
    return 0;
}

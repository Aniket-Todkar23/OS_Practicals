#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define MAX_THREADS 10
#define MAX_FLIGHTS 10

// Flight structure
typedef struct {
    char flight_number[20];
    char destination[50];
    int available_seats;
} Flight;

// Shared database
Flight flight_database[MAX_FLIGHTS];
int total_flights = 0;

// Synchronization tools
int readcount = 0;           // Number of readers currently reading
pthread_mutex_t mutex;       // Protects readcount variable
sem_t wrt;                   // Writer lock for exclusive access

// Reader function - checks flight information
void* reader(void* arg) {
    int reader_id = *((int*)arg);
    
    for (int iteration = 0; iteration < 3; iteration++) {
        // Entry Section
        pthread_mutex_lock(&mutex);
        readcount++;
        if (readcount == 1) {
            sem_wait(&wrt);  // First reader locks writers
        }
        pthread_mutex_unlock(&mutex);
        
        // Critical Section - READING flight information
        printf("\n[Reader %d] Checking flight availability:\n", reader_id);
        for (int i = 0; i < total_flights; i++) {
            printf("   %s to %s: %d seats available\n", 
                   flight_database[i].flight_number,
                   flight_database[i].destination,
                   flight_database[i].available_seats);
        }
        printf("   Active readers: %d\n", readcount);
        sleep(1);
        
        // Exit Section
        pthread_mutex_lock(&mutex);
        readcount--;
        if (readcount == 0) {
            sem_post(&wrt);  // Last reader unlocks writers
        }
        pthread_mutex_unlock(&mutex);
        
        printf("[Reader %d] Finished reading\n", reader_id);
        sleep(2);
    }
    
    return NULL;
}

// Writer function - makes flight reservations
void* writer(void* arg) {
    int writer_id = *((int*)arg);
    
    for (int iteration = 0; iteration < 3; iteration++) {
        // Entry Section
        sem_wait(&wrt);  // Request exclusive access
        
        // Critical Section - WRITING (booking flight)
        int flight_index = writer_id % total_flights;
        
        if (flight_database[flight_index].available_seats > 0) {
            flight_database[flight_index].available_seats--;
            printf("\n>>> [Writer %d] Booked seat on %s to %s\n", 
                   writer_id,
                   flight_database[flight_index].flight_number,
                   flight_database[flight_index].destination);
            printf("    Seats remaining: %d <<<\n", 
                   flight_database[flight_index].available_seats);
        } else {
            printf("\n>>> [Writer %d] Booking failed for %s - No seats available <<<\n", 
                   writer_id,
                   flight_database[flight_index].flight_number);
        }
        sleep(2);
        
        // Exit Section
        sem_post(&wrt);  // Release exclusive access
        
        printf(">>> [Writer %d] Finished booking <<<\n", writer_id);
        sleep(3);
    }
    
    return NULL;
}

int main() {
    int num_readers, num_writers;
    pthread_t readers[MAX_THREADS], writers[MAX_THREADS];
    int reader_ids[MAX_THREADS], writer_ids[MAX_THREADS];
    
    printf("==========================================================\n");
    printf("    Airline Reservation System - Reader-Writer Problem\n");
    printf("           (Readers Priority Implementation)\n");
    printf("==========================================================\n\n");
    
    // Initialize flight database (hardcoded from airbus.txt)
    strcpy(flight_database[0].flight_number, "AB101");
    strcpy(flight_database[0].destination, "New York");
    flight_database[0].available_seats = 50;
    
    strcpy(flight_database[1].flight_number, "AB102");
    strcpy(flight_database[1].destination, "London");
    flight_database[1].available_seats = 50;
    
    strcpy(flight_database[2].flight_number, "AB103");
    strcpy(flight_database[2].destination, "Tokyo");
    flight_database[2].available_seats = 50;
    
    strcpy(flight_database[3].flight_number, "AB104");
    strcpy(flight_database[3].destination, "Paris");
    flight_database[3].available_seats = 50;
    
   
    
    total_flights = 4;
    
    printf("Flight Database Initialized:\n");
    for (int i = 0; i < total_flights; i++) {
        printf("  %s to %s: %d seats\n", 
               flight_database[i].flight_number,
               flight_database[i].destination,
               flight_database[i].available_seats);
    }
    printf("\n");
    
    // Get user input
    printf("Enter number of readers (max %d): ", MAX_THREADS);
    scanf("%d", &num_readers);
    
    printf("Enter number of writers (max %d): ", MAX_THREADS);
    scanf("%d", &num_writers);
    
    if (num_readers <= 0 || num_readers > MAX_THREADS || 
        num_writers <= 0 || num_writers > MAX_THREADS) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("\n==========================================================\n");
    printf("Starting simulation with:\n");
    printf("  - %d readers (checking flights)\n", num_readers);
    printf("  - %d writers (booking flights)\n", num_writers);
    printf("  - %d flights in database\n", total_flights);
    printf("==========================================================\n");
    
    // Initialize synchronization tools
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);  // Binary semaphore (1 = available)
    
    // Create reader threads
    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
      

    
    // Create writer threads
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    // Wait for all threads to complete their iterations
    for (int i = 0; i < num_readers; i++) {
        pthread_join(readers[i], NULL);
    }
    
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writers[i], NULL);
    }
    
    printf("\n\n==========================================================\n");
    printf("[System] All threads completed - Program finished\n");
    printf("==========================================================\n");
    
    // Display final flight status
    printf("\nFinal Flight Status:\n");
    for (int i = 0; i < total_flights; i++) {
        printf("  %s to %s: %d seats remaining\n", 
               flight_database[i].flight_number,
               flight_database[i].destination,
               flight_database[i].available_seats);
    }
      sleep(30);
   
    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);
    
    return 0;
}

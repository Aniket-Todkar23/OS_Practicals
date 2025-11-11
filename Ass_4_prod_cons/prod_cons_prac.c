#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<unistd.h>
#define BUFFERSIZE 5
int buffer[BUFFERSIZE];
int in=0;
int out=0;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;


void *producer(void *arg){
    int pizza_num;
    for(int i=1;i<=10;i++){
        pizza_num=i;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in]=pizza_num;
        printf("\nProducer: Produced Pizza #%d (placed at position %d)\n",pizza_num,in);
        in=(in+1)%BUFFERSIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
    printf("\nProducer: Done producing all pizzas!\n");
    return NULL;
}
void *consumer(void *arg){
    int pizza_num;
    for(int i=1;i<=10;i++){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        pizza_num=buffer[out];
        buffer[out]=0;
        printf("\nConsumer: Consumed Pizza #%d (taken from position %d)\n",pizza_num,out);
        out=(out+1)%BUFFERSIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(2);
    }
    printf("\nConsumer: Done consuming all pizzas!\n");
    return NULL;
}
int main(){
    pthread_t joey,simpson;

    sem_init(&empty,0,BUFFERSIZE);
    sem_init(&full,0,0);

    pthread_create(&simpson,NULL,producer,NULL);
    pthread_create(&joey,NULL,consumer,NULL);

    pthread_join(simpson,NULL);
    pthread_join(joey,NULL);
}
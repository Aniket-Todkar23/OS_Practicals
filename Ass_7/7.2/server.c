#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHM_KEY 0x1234
int main(){
    int shmid=shmget(SHM_KEY,100,IPC_CREAT | 0666);
    if(shmid<0){
        perror("shmget");
        exit(1);
    }
    char *shmaddr;
    shmaddr=(char *)shmat(shmid,NULL,0);
    if(shmaddr==(char *)-1){
        perror("shmat");
        exit(1);
    }
    printf("Server : Write the message :");
    fgets(shmaddr,100,stdin);
    printf("Msg written to shared memory waiting for client to read\n");
    sleep(5);
    printf("server done\n");
    shmdt(shmaddr);
    return 0;
}
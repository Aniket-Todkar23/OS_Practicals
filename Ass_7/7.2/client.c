#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHM_KEY 0x1234
int main(){
    while(1){
        sleep(5);
    int shmid=shmget(SHM_KEY,100,0666);
    if(shmid<0){
        perror("shmget");
        exit(1);
    }
    char *shmaddr;
    shmaddr=(char*)shmat(shmid,NULL,0);
    if(shmaddr==(char *)-1){
        perror("shmat");
        exit(1);
    }
    printf("Client : Msg from server :%s\n",shmaddr);

    shmdt(shmaddr);
    sleep(5);
    }
    

}
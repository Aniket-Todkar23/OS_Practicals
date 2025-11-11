#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<string.h>
#include<sys/msg.h>
#include<ctype.h>

#define QUEUE_KEY 0x1234

struct msgbuf
{
    long mtype;
    char mtext[128];
};

int main(){
    int msgid=msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if(msgid==-1){
        perror("msgget");
        exit(1);
    }
    struct msgbuf msg;
    printf("Receiver is ready waiting for message...\n");
    while(1){
        if(msgrcv(msgid,&msg,sizeof(msg.mtext),0,0)==-1){
            perror("msgrcv");
            exit(1);
        }

        if(strcmp(msg.mtext,"exit")==0){
            printf("\nReceiver Exiting");
            break;
        }

        // for(int i=0;msg.mtext[i];i++){
        //     msg.mtext[i]=toupper(msg.mtext[i]);
        // }
        printf("Received message type %ld :%s\n",msg.mtype,msg.mtext);
    }

    printf("Cleanup\n");
    if(msgctl(msgid,IPC_RMID,NULL)==0){
        printf("Cleaned successfully\n");
    }
    else{
        perror("Error");
    }
    return 0;
}

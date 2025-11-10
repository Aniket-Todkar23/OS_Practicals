#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define QUEUE_KEY 0x1234

struct msgbuf{
    long mtype;
    char mtext[128];

};
int main(){
    int msgid=msgget(QUEUE_KEY,IPC_CREAT | 0666);
    if(msgid==-1){
        perror("msgget");
        exit(1);
    }
    struct msgbuf msg;
    while(1){
        printf("\nEnter the message type >0 or 0 to exit :");
        scanf("%ld",&msg.mtype);
        getchar();
        if(msg.mtype==0){
            printf("Sender Exiting ...\n");
            strcpy(msg.mtext,"exit");
            msg.mtype=1;
            if(msgsnd(msgid,&msg,sizeof(msg.mtext),0)==-1){
                perror("msgsnd");
                exit(1);

            }
            break;
        }
        printf("\nEnter the message to send :");
        fgets(msg.mtext,128,stdin);
        msg.mtext[strcspn(msg.mtext,"\n")]='\0';

        if(msgsnd(msgid,&msg,sizeof(msg.mtext),0)==-1){
            perror("msgsnd");
            exit(1);
        }
    }
    return 0;
}
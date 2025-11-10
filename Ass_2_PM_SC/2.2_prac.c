#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>

void bubblesort(int arr[],int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                int t=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=t;
            }
            
        }
    }
}
int main(){
    int n;
    printf("Enter the number of elements :");

    scanf("%d",&n);
    int arr[n];
    printf("Enter the Integers :");
    for(int i=0;i<n;i++){
        scanf("%d",&arr[i]);
    }
    bubblesort(arr,n);
    printf("Sorted Array");
    for(int i=0;i<n;i++){
        printf("%d",arr[i]);
    }
    pid_t pid =fork();
    if(pid<0){
        perror("Fork Failed");
        exit(1);
    }
    if(pid==0){
        printf("Child Process");
        char *args[n+3];
        args[0]="./child";
        //convert num to string
        for(int i=0;i<n;i++){
            char *num=malloc(10);
            sprintf(num,"%d",arr[i]);
            args[i+1]=num;
        }
        args[n+1]=NULL;
        execve("./child",args,NULL);
        perror("Ececve Failed");
        exit(1);

    }
    else{
        wait(NULL);
        printf("Parent done, Exiting..");
    }
    return 0;
}
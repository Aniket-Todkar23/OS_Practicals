#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

//parent process
void bubblesort(int arr[],int n){
    printf("Parent Process Started.....\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]<arr[j+1]){
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
    printf("Parent array sorted : Bubble Sort\n");
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
}
void isort(int arr[],int n){
    int key;
    printf("Child process started\n");
    for(int i=0;i<n;i++){
        key=arr[i];
        int j=i-1;
        while(j>=0 && arr[j]>key){
            arr[j+1]=arr[j];
            
            j--;
        }
        arr[j+1]=key;
    }
      printf("Child array sorted : Bubble Sort\n");
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
}
int main(){
    int n;
    pid_t pid;
    int status;
    printf("Enter the number of Integers :");
    scanf("%d",&n);
    int arr[n];
    printf("Enter the Integers \n");
    for(int i=0;i<n;i++){
        scanf("%d",&arr[i]);
    }
    pid=fork();
    if(pid<0){
        perror("Fork Failed\n");
        exit(1);
    }
    else if(pid==0){
        printf("CHILD PROCESS\n");
        printf("Child PID: %d\n",getpid());
        printf("Parent PID :%d\n",getppid());

        isort(arr,n);

        printf("Child Process Exiting...\n");
        exit(0);
    }
    else{
        printf("CHILD PROCESS\n");
        printf("Parent PID: %d\n",getpid());
        printf("Child PID :%d\n",pid);

        bubblesort(arr,n);

        printf("Waiting for the child process to finish...\n");
        wait(&status);
        if(WIFEXITED(status)){
            printf("Child process exited with status : %d\n",WEXITSTATUS(status));
        }
    }
            printf("Exiting Parent Process...\n");

}
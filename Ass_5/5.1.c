#include<stdio.h>
#include<stdlib.h>

#define Mx_process 10
#define Mx_res 10

int num_p;
int num_R;
int available[Mx_res];
int alloc[Mx_process][Mx_res];
int maximum[Mx_process][Mx_res];
int need[Mx_process][Mx_res];
int safe_seq[Mx_process];

int issafe(){
    int work[Mx_res];
    int finish[Mx_process]={0};
    int count=0;
    for(int i=0;i<num_R;i++){
        work[i]=available[i];
    }

    while(count<num_p){
        int found=0;
        for(int i=0;i<num_p;i++){
            if(!finish[i]){
                int can_alloc=1;
                for(int j=0;j<num_R;j++){
                    if(need[i][j]>work[j]){
                        can_alloc=0;
                        break;
                    }
                }
                 if(can_alloc){
                    for(int k=0;k<num_R;k++){
                        work[k]+=alloc[i][k];
                    }
                    safe_seq[count]=i;
                    finish[i]=1;
                    count++;
                    found=1;
                 }

            }
           
        }
        if(!found){
            return 0;
        }
    }

return 1;
}

void print_state(){
    printf("Current State of the system");
    printf("Available resources :");

    for(int i=0;i<num_R;i++){
      
        printf("%d",available[i]);
    }
    printf(" Allocation matrix");
    for(int i=0;i<num_p;i++){
        printf("For Process p%d",i);
        for(int j=0;j<num_R;j++){
         
            printf("%d",alloc[i][j]);
        }
    }
    printf("Maximum matrix");
    for(int i=0;i<num_p;i++){
        printf("For Process p%d",i);
        for(int j=0;j<num_R;j++){
            
            printf("%d",maximum[i][j]);
        }
    }
    printf("Need Matrix!");
    for(int i=0;i<num_p;i++){
        for(int j=0;j<num_R;j++){
            printf("%d",need[i][j]);
        }
    }

}
int main(){
    // get num process and res
    printf("Enter the Number of processes :");
    scanf("%d",&num_p);
    printf("Enter the Number of Resources :");
    scanf("%d",&num_R);

    printf("Enter the resources available :");
    for(int i=0;i<num_R;i++){
        printf("For resource R%d",i);
        scanf("%d",&available[i]);
    }
    printf("Enter Allocation matrix");
    for(int i=0;i<num_p;i++){
        printf("For Process p%d",i);
        for(int j=0;j<num_R;j++){
            printf("For resource R%d",i);
            scanf("%d",&alloc[i][j]);
        }
    }
    printf("Enter Maximum matrix");
    for(int i=0;i<num_p;i++){
        printf("For Process p%d",i);
        for(int j=0;j<num_R;j++){
            printf("For resource R%d",i);
            scanf("%d",&maximum[i][j]);
        }
    }
    printf("calculating the Need Matrix!");
    for(int i=0;i<num_p;i++){
        for(int j=0;j<num_R;j++){
            need[i][j]=maximum[i][j]-alloc[i][j];
        }
    }
    print_state();
    if(issafe()){
        printf("System is in safe state");
        for(int i=0;i<num_p;i++){
            printf("%d ",safe_seq[i]);
        }
    }
    else{
        printf("System is in unsafe state");
    }
    return 0;
}
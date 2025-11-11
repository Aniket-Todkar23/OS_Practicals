#include<stdio.h>
#include<stdlib.h>

#define Mx_cyll 4999
#define NUM_REQ 10
#define Mi_cyll 0

void sort(int arr[],int n){
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

void scan(int reqs[],int n,int head,int prev){
    int tot_mov=0;
    int sequence[20];
    int seq_idx=0;
    int curr=head;

    int sorted[NUM_REQ];
    for(int i=0;i<n;i++){
        sorted[i]=reqs[i];
    }
    sort(sorted,n);
    int srt_idx=0;
    while(srt_idx<n && sorted[srt_idx]<=head)srt_idx++;
    printf("Movement started with head ");
    sequence[seq_idx++]=head;

    for(int i=srt_idx;i<n;i++){
        printf("->%d",sorted[i]);
        sequence[seq_idx++]=sorted[i];
        tot_mov+=abs(sorted[i]-curr);
        curr=sorted[i];
    }

    printf("->%d",Mx_cyll);
    sequence[seq_idx++]=Mx_cyll;
    tot_mov+=abs(Mx_cyll-curr);
    curr=Mx_cyll;

    for(int i=srt_idx-1;i>=0;i--){
        printf("->%d",sorted[i]);
        sequence[seq_idx++]=sorted[i];
        tot_mov+=abs(sorted[i]-curr);
        curr=sorted[i];
    }
    printf("Totmovement calculated :%d",tot_mov);
}
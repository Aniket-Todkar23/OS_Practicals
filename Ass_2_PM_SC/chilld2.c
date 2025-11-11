#include<stdio.h>
#include<stdlib.h>

int binsearch(int arr[],int n,int key){
    int l=0;
    int r=n-1;
    while(l<r){
        int mid=(l+r)/2;
        if(arr[mid]==key)return mid;
        else if(arr[mid]<key)l=mid+1;
        else r=mid-1;
    }
    return -1;
}

int main(int argc,char *argv[]){
    if(argc<2){
        printf("No string recieved");
        exit(1);
    }
    int n=argc-1;
    int arr[n];
    for(int i=0;i<n;i++){
        arr[i]=atoi(argv[i+1]);
    }
    printf("Received array");
    for(int i=0;i<n;i++){
        printf("%d",arr[i]);
    }
    int key;
    printf("Enter the key to search");
    scanf("%d",&key);
    int result=binsearch(arr,n,key);
    if(result != -1){
        printf("Element found at position %d",result);
    }
    else{
        printf("Element not found");
    }
}
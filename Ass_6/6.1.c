#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void display(int frames,int frame[]){
    printf("|");
    for(int i=0;i<frames;i++){
        if(frame[i]==-1){
            printf("_ ");
        }
        else{
            printf("%d",frame[i]);
        }
    }
}
void fifo(int pages[],int n,int frames){
    int pagefault=0;
    int front=0;
    int frame[frames];
    for(int i=0;i<frames;i++){
        frame[i]=-1;
    }
    printf("Ref | frames | fault?\n");
    printf("----|--------|-------\n");
    for(int i=0;i<n;i++){
        int found=0;
        int page= pages[i];
        for(int j=0;j<frames;j++){
            if(frame[j]==page){
                found=1;
                break;
            }
        }
        if(!found){
            frame[front]=page;
            front=(front+1)%frames;
            pagefault++;
            printf("%d\t ",page);
            display(frames,frame);
            printf("| Yes\n");
        }else{
            printf("%d\t",page);
            display(frames,frame);
            printf("| No\n");
        }
    }
}

void lru(int pages[],int n,int frames){
    int pagefault=0;
    int frame[frames];
    int last_used[frames];

    for(int i=0;i<frames;i++){
        frame[i]=-1;
        last_used[i]=-1;
         
    }
    printf("Ref | frames | fault?\n");
    printf("----|--------|-------\n");
    for(int i=0;i<n;i++){
        int page = pages[i];
        int found=0;
            int emp_frame=-1;
            for(int j=0;j<frames;j++){
                if(frame[j]==page){
                    found=1;
                    last_used[j]=i;
                    break;
                }
                if(emp_frame==-1 && frame[j]==-1){
                    emp_frame=j;
                }
            }
            if(!found){
                if(emp_frame!=-1){
                    frame[emp_frame]=page;
                    last_used[emp_frame]=i;
                }
                else{
                    int lruidx=0;
                    for(int j=1;j<frames;j++){
                        if(last_used[j]<last_used[lruidx]){
                            lruidx=j;
                        }
                    }
                    frame[lruidx]=page;
                    last_used[lruidx]=i;
                }
                 printf("%d\t ",page);
                display(frames,frame);
                printf("| Yes\n");
            }else{
                printf("%d\t",page);
            display(frames,frame);
            printf("| No\n");
            }

    }


}

int main(){
   int n;
   printf("\nEnter the number of pages :");
   scanf("%d",&n);
   printf("\nEnter the reference string :");
   int pages[n];
   for(int i=0;i<n;i++){
    scanf("%d",&pages[i]);
   }
    printf("\nEnter the number of frames :");
    int frames;
   scanf("%d",&frames);
   while(1){
    printf("Menu\n");
    printf("1)FIFO\n");
    printf("2)LRU\n");
    printf("4)Exit\n");
    printf("Enter Choice :");
    int choice;
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        fifo(pages,n,frames);
        break;
    case 2:
        lru(pages,n,frames);
        break;
    case 4:
        printf("Exiting...");
        exit(0);
    
    default:
        break;
    }
   }
   return 0;
}
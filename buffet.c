#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[6];
int count=0;

void* producer(void* argv){
    while(1){
    //produce
    int x= rand() %3;
    sleep(1);

    //add to the buffer
    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutexBuffer);
        buffer[count]= x;
        count++;
    
    
    pthread_mutex_unlock(&mutexBuffer);
    sem_post(&semFull);

    }
   
}

void* consumer(void* args){
    while(1){
        int y;
    //remove from the buffer
    sem_wait(&semFull);
     pthread_mutex_lock(&mutexBuffer);
        y= buffer[count -1];
        count--;
     
    pthread_mutex_unlock(&mutexBuffer);
    sem_post(&semEmpty);
    //consume
    printf("van %d\n" , y);
    sleep(1);

    }
    

}

int main(int argc, char* argv[]){
    srand(time(NULL));
    int THREAD_NUM;
    printf("introduzca las personas que van a llegar");
    scanf("%d",&THREAD_NUM);
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer,NULL);
    sem_init(&semEmpty,0, 6);
    sem_init(&semFull,0,0);
    int i;
    for(i=0; i <THREAD_NUM; i++){
        if(i>0){
        if(pthread_create(&th[i], NULL, &producer, NULL)!=0){
            perror("failed to cread thread");
        }
        }else {
            if(pthread_create(&th[i], NULL, &consumer, NULL)!=0){
            perror("failed to cread thread");

            }
        }
    }
    for(i=0;i<THREAD_NUM;i++){
        if(pthread_join(th[i],NULL)!=0){
            perror("daled to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}
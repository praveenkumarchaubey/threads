/*Write a program where 5 threads are created and each thread prints the thread Id and message .thread x executing..*/
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define MAX 5

int status = 10;

void *func(void *arg)
{
    int n;
    n = *((int *)arg);
    printf("This is thread %d\n Thread id is: %lu\n", n, pthread_self());
    pthread_exit ((void *) &status);
}

int main()
{
    pthread_t tid[MAX]; //array of type pthread_t to store thread ids
    int i, ret, j;
    int *status; //to store the exit status
    int *arg; //argument to be passed

    for(i=0; i<MAX; i++)
    {
        arg = (int *)malloc(sizeof(int)); //allocating memory to the pointer
        *arg = i+1;
        ret = pthread_create(&tid[i], NULL, func, (void *)arg);
        if(0 != ret)
        {
            printf("Thread %d  Create Error.\n", i);
            return 0;
        }
    }

    //free the resources print the exit status
    for(j=0; j<MAX; j++)
    {
        ret = pthread_join(tid[j], (void**)&status); 
        if(0 != ret)
        {
            printf("Thread %d Join Failed.\n", i);
            return 0;
        }
    }
    return 0;
}


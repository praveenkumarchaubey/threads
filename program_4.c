/***************************************************************************
*       FILENAME:ThreadSema.c
*       DESCRIPTION:Contains Code for a program that demonstrates the
*       use of pthread calls
*       Invoke the Executable as a.out
*       Copyright 2007 Aricent
****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<semaphore.h>

/*Gloabl Counter*/
int counter;

/*Semaphore Variable available only in this Process*/
sem_t  posixsema;

void *doit1(void *);
void *doit2(void *);

int main()
{
        int ret;
        pthread_t th1,th2;

        /*Initialising Semaphore To 1, 2nd Arg PSHARED=0 which implies that
        Semaphore available locally to process*/
        /*So this semaphore is having Process Persistence, and don't initialise
        again*/
        ret = sem_init(&posixsema,0,1);

        if (-1 == ret)
        {
                printf("Failed to initialise Semaphore\n");
                exit(1); //Exit the Process
        }

        pthread_create(&th1, NULL, doit1, NULL);
        pthread_create(&th2, NULL, doit2, NULL);
        pthread_join(th1, NULL);
        pthread_join(th2, NULL);
	return 0;
}

void *doit2(void *ptr)
{
        int i,val;
        /*Test and Decrement atomically*/
        sem_wait(&posixsema);
        for(i=0;i<15000;i++)
	{
                val =counter;
                printf("%lu:%d\n",pthread_self()%10,val+1);
                counter=val+1;
        }
        /*Increment Atomically*/
        sem_post(&posixsema);
        return NULL;
}

void *doit1(void *ptr)
{
        int i,val;
        /*Test and Decrement atomically*/
        sem_wait(&posixsema);
        for(i=0;i<15000;i++)
	{
                val =counter;
                printf("%lu:%d\n",pthread_self()%10,val+1);
                counter=val+1;
        }
        /*Increment Atomically*/
        sem_post(&posixsema);
        return NULL;
}


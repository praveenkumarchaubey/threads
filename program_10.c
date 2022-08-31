/* Please do the following assignment:
 * Implement a message queue, which works as follows:
 * There is a function putqueue() which can be used to put any type of object (data) in the queue. putqueue should block the caller if the queue is full.
 * There is a function getqueue(), which can be used to get(remove) an object from the queue. getqueue should block if there is nothing in the queue
 * There is an initqueue() function which initializes the queue with the maximum number of elements it can hold.
 * The putqueue() and the getqueue() functions can be used by any number of threads in an MT(Multi-threaded) program and it should not lead to any conflict between these threads.
 * Create a library of this message queue.
 * There should be a header file msgqueue.h which has the prototypes and there should be a source code file msgqueue.c, which should have the functions and data types defined.
 * Write a multithreaded program which links with this message queue library and uses the message queue for communication between multiple threads.
 * In this program there are > 1 writer threads which write into the message queue and there are > 1 reader threads which read from the queue.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#define SIZE 100

int queue[SIZE];
int top = -1;
sem_t put;
sem_t get;
pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;
int val = 10;

void *init(void *arg)
{
    int i;
    for(i=0; i<SIZE; i++)
    {
        queue[++top] = SIZE + top;
    }
}

void *putqueue(void *arg)
{
    sem_wait(&put);
    if(top<SIZE-1)
    {
        queue[top++] = val;
        val++;
    }
    sem_post(&get);
}

void *getqueue(void *arg)
{
    sem_wait(&get);
    if(top > 0)
    {
        printf("Value = %d\n",queue[top--]);
    }
    sem_post(&put);
}

int main()
{
    pthread_t t1, t2, t3;
    int check, ret;
    sem_init(&put, 0, 0);
    sem_init(&get,0, 1);
    ret = pthread_create(&t1, NULL, init, NULL);
    if(ret != 0)
    {
        printf("Thread Create Error\n");
        return 0;
    }

    do
    {
        ret = pthread_create(&t2, NULL, putqueue, NULL);
        if(ret != 0)
        {
            printf("Thread Create Error\n");
            return 0;
        }
        ret = pthread_create(&t3, NULL, getqueue, NULL);
        if(ret != 0)
        {
            printf("Thread Create Error\n");
            return 0;
        }
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);
        printf("Continue transaction? (0/1)\n");
        scanf("%d",&check);
    }while(check);
    pthread_join(t1, NULL);
    return 0;

}


/* Write a program where 2 threads communicate using a single global variable "balance" and initialized to 1000.
 * Thread 1 deposits amount = 50 for 50 times and prints the balance amount and thread 2 withdrawals amount=20 for 20 times and prints the final balance.
 * Execution of thread 1 and thread 2 should not interleave.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int balance = 1000; //global variable

pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;

void *deposit(void *arg)
{
    int amt = 50, i;
    for(i=0; i<50; i++)
    {
        pthread_mutex_lock(&mutx); //lock the mutex
        balance = balance + amt;
        pthread_mutex_unlock(&mutx); //unlock the mutex
    }
    return NULL;
}

void *withdraw(void *arg)
{
    int amt = 20, i;
    for(i=0; i<20; i++)
    {
        pthread_mutex_lock(&mutx); //lock the mutex
        balance = balance - amt;
        pthread_mutex_unlock(&mutx); //unlock the mutex
    }
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int ret;
    ret = pthread_create(&t1, NULL, deposit, NULL);
    if(ret != 0)
    {
        printf("Thread Create Error.\n");
        return 0;
    }

    ret = pthread_create(&t2, NULL, withdraw, NULL);
    if(ret != 0)
    {
        printf("Thread Create Error.\n");
        return 0;
    }

    ret = pthread_join(t1, NULL);
    if(ret != 0)
    {
        printf("Thread Join Error.\n");
        return 0;
    }

    ret = pthread_join(t2, NULL);
    if(ret != 0)
    {
        printf("Thread Join Error.\n");
        return 0;
    }
    printf("Balance is : %d\n", balance);
    return 0;
}

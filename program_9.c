/* Write a program where 2 threads operate on a global variable "account" initialized to 1000.
 * There is a deposit function which deposits a given amount in this "account":  int deposit(int amount).
 * There is a withdrawal function which withdraws a given amount from the "account": int withdrawal(int amount)
 * However there is a condition: withdrawal function should block the calling thread when the amount in the "account" is less than 1000,
 * i.e. you can't withdraw if the "account" value is less than 1000.
 * Threads calling the deposit function should indicate to the withdrawing threads when the amount is greater than 1000.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>

//declaring two semaphore variables
sem_t dep; 
sem_t with;

//global variable on which the threads will opreate
int account = 1000;

//deposit function
void *deposit(void *amount)
{
    int amt;
    amt = (*(int *)amount);

    sem_wait(&dep); //lock the deposit semaphore
    account = account + amt;
    printf("New Balance: %d\n", account);
    sem_post(&with); //unlock the  withdrawl semaphore

    return NULL;
}

//withdrawl function
void *withdraw(void *amount)
{
    int amt;
    amt = (*(int *)amount);

    sem_wait(&with); //lock the withdrawl semaphore
    if(account < 1000)
    {
        printf("Sorry. Not allowed to withdraw.\n");
    }
    else
    {
        account = account - amt;
    }

    printf("New Balance: %d\n", account);
    sem_post(&dep); //unlock the deposit semaphore
    return NULL;
}


int main()
{
    pthread_t t1,t2;
    int ret, amount, check;

    sem_init(&dep, 0, 1); //initalize the semaphore variable
    sem_init(&with, 0, 0);

    do
    {
        printf("Enter Amount to be deposited: \n");
        scanf("%d", &amount);

        ret = pthread_create(&t1, NULL, (void *)deposit, (void*)&amount);
        if(ret != 0)
        {
            printf("Error Thread Create\n");
            return 0;
        }

        printf("Enter Amount to be withdrawn: \n");
        scanf("%d", &amount);
        ret = pthread_create(&t2, NULL, (void *)withdraw, (void*)&amount);
        if(ret != 0)
        {
            printf("Error Thread Create\n");
            return 0;
        }

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        printf("Do you want to continue the transaction? (0/1)\n");
        scanf("%d", &check);
    }while(check);
    return 0;
}


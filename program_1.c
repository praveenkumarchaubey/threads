/* creating n threads and printing their ids */
/***************************************************************************
 *       FILENAME: thread.c
 *       DESCRIPTION: Contains Code for a program that demonstrates the
 *       use of pthread calls
 ****************************************************************************/

#include<pthread.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define MAX 80

void *printids(void *s);

int main(int argc, char *argv[])
{
    while(2 != argc)
    {
        printf("Insufficient number of arguments\n");
        printf("Please provide the number of threads you want to create\n");
        exit(1);
    }

    pthread_t ntid[MAX]; /*Thread Identifier*/
    int status=0, ret, i, child;
    int *arg; //argument to be passed

    /*Creating a thread*/
    child = atoi(argv[argc - 1]);

    for (i=0; i<child; i++)
    {
        arg = (int *)malloc(sizeof(int)); //allocating memory to the pointer
        *arg = i;
        status = pthread_create(&ntid[i], NULL, printids, (void *)arg);
        if (status != 0)
        {
            printf("Error in Creating Thread\n");
            exit(1);
        }
    }

    for(i = 0; i< child; i++)
    {
        ret = pthread_join(ntid[i], NULL);
        if (ret != 0)
        {
            printf("Error joining\n");
            exit(1);
        }
    }
    exit(0);
}

/********************************************************************
 *       FUNCTION NAME: printids
 *       DESCRIPTION: Prints the information of a Thread
 *       NOTES: No Error Checking is done
 *       RETURNS: void
 *********************************************************************/
void *printids(void *arg)
{
    int s;
    s = *((int *)arg);
    pid_t pid;
    pthread_t tid;

    pid = getpid();/*Getting the Process Id*/
    tid = pthread_self();/*Getting the Thread Identifier*/
    printf("Thread %d has a process id %u and thread id %lu\n", s+1, pid, tid);
    pthread_exit(NULL);
}


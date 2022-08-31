/*Write a program where 2 threads are created and each thread prints information like (Name, Hall No., employee ID ,branch).*/

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Employee
{
    char name[30];
    int hallNo;
    int empID;
    char branch[30];
}employee;

int status = 10;
void *func(void * arg)
{
    employee *emp;
    emp = (employee *)arg;
    printf("\nEmployee Name: %s\n", emp->name);
    printf("Employee HallNo: %d\n", emp->hallNo);
    printf("Employee empID: %d\n", emp->empID);
    printf("Employee branch: %s\n", emp->branch);
    pthread_exit((void *) &status);
}

int main()
{
    pthread_t t[2];
    employee *emp[2];
    char name[30], branch[30];
    int i, ret;
    int *status;

    for(i=0; i<2; i++)
    {
        emp[i] = (employee *) malloc(sizeof(employee));

        printf("Enter Employee %d details: \n", i+1);

        printf("Enter name: \t");
        memset(name, 0,29);
        fgets(name, 29, stdin);
        strcpy(emp[i]->name, name);

        printf("Enter Hall no.: \t");
        scanf("%d", &(emp[i]->hallNo));

        printf("Enter empID: \t");
        scanf("%d", &(emp[i]->empID));

        while('\n'!=getchar());
        printf("Enter Branch: \t");
        memset(branch, 0, 29);
        fgets(branch, 29, stdin);
        strcpy(emp[i]->branch, branch);
    }

    for(i=0; i<2; i++)
    {
        ret = pthread_create(&t[i], NULL, func, (void *)emp[i]);
        if(ret != 0)
        {
            printf("Thread creation failed");
            return 0;
        }
    }

    for(i=0; i<2; i++)
    {
        ret = pthread_join(t[i], (void**)&status);
        if(0 != ret)
        {
            printf("Thread joining failed\n");
            return 0;
        }
    }
    return 0;
}


/*Write a threaded application where user will be providing “n” number of strings from the command line. The application will create “n” number of threads and will invoke  the function reverse_string with every thread and give one string from command line to each thread. 
Each thread will return the original string and the reversed string to the calling thread which the calling thread will print along with the threadid of the thread that has reversed that string.*/

#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100

typedef struct string
{
        char original[MAX];
        char reverse[MAX];
}structure;

void *reverse(void *arr);

int main(int argc, char *argv[])
{
        int i, creat, status;
        void *retval;
        pthread_t ntid[MAX];
        structure s[MAX];

        if (argc < 2)
        {
                printf("not enough argument\n");
                exit(1);
        }

        for (i = 0; i < argc-1; i++)
        {
                strcpy(s[i].original, argv[i+1]);
                memset(s[i].reverse, '\0', MAX*sizeof(char));
                creat = pthread_create(&ntid[i], NULL, reverse, &s[i]);
                if (creat)
                {
                        printf("error in creating child %d\n",i);
                        exit(1);
                }
        }

        for (i = 0; i < argc-1; i++)
        {
                status = pthread_join(ntid[i], &retval);
                if (status)
                {
                        printf("error in joining thread %d\n",i);
                        exit(1);
                }
        }
        printf("main exiting\n");
        exit(0);
}

void *reverse(void *arg)
{
        structure *arr;
        arr = (structure *)arg;
        int len, j=0,i;
        char b[MAX];
        len = strlen(arr->original);
        for(i = len-1; i >= 0; i--)
        {
                b[j] = arr->original[i];
                j++;
        }
                b[len] = '\0';
                strcpy(arr->reverse, b);
	printf("Thread ID: \t%lu\n", pthread_self());
	printf("original string: %s\t reversed string: %s\n\n",arr->original,arr->reverse);
        pthread_exit((void *)arr);
}


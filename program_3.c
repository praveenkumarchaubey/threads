/*Write a threaded application that accepts n number of file names from command line and creates n threads. Each thread is provided with the file name. Every thread will find the size of the file and inode number of the file and return back the filename, inode number and size of file. In case the file does not exist, the thread will return back the filename and 0,0 for file size and inode number. The main thread will print the threadid, file name, file size and inode number.*/

#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define MAX 100

typedef struct file_info
{
        char name[MAX];
        ino_t inodenum;
	mode_t mode;
        off_t size;
        pthread_t id;
}structure;

void *file_info(void *arr);

int main(int argc, char *argv[])
{
        int i, creat, status;
        void *retval;
        structure *ret;
        pthread_t ntid[MAX];
        structure s[MAX];

        if (argc < 2)
        {
                printf("not enough argument\n");
                exit(1);
        }

        for (i = 0; i < argc-1; i++)
        {
                strcpy(s[i].name, argv[i+1]);
                creat = pthread_create(&ntid[i], NULL, file_info, &s[i]);
                if (creat)
                {
                        printf("error in creating child %d\n",i);
                        exit(1);
                }
        }

        for (i = 0; i < argc-1; i++)
        {
                status = pthread_join(ntid[i],&retval);
                if (status)
                {
                        printf("error in joining thread %d\n",i);
                        exit(1);
                }

                ret = (structure *)retval;
		if((ret->mode & S_IFMT) == S_IFDIR)
		{
			printf("It is a directory\n");

		}

		if((ret->mode & S_IFMT) == S_IFREG)
		{
			printf("It is a regular file\n");

		}

                printf("file name: \t%s\n file size: \t%lu\n inode num: \t%lud\n thread id: \t%lu\n\n",ret->name,ret->size,ret->inodenum,ret->id);
        }
        printf("main thread exiting now...\n");
        exit(0);
}

void *file_info(void *arg)
{
        structure *arr;
        struct stat buf; //define a  buffer of type struct stat to store the specified file info

        arr = (structure *)arg;
        int fp, status;
        fp = open(arr->name, O_RDONLY);
        arr->id = pthread_self();

        if (fp < 0)
        {
                arr->size = 0;
                arr->inodenum = 0;
        }
        else
        {
                status = fstat(fp, &buf);
                if (status == 0)
                {
                        arr->size = buf.st_size;
                        arr->inodenum = buf.st_ino;
			arr->mode = buf.st_mode;
                }
        }
        pthread_exit((void *)arr);
}


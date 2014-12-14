/*Write an application to do the following:
The application will accept a filename from command line. The file has strings separated by comma, stored in it. The application will read the strings from the file and store it in a suitable data structure. If the file has “n” number of strings, the application will create “n” number of threads and provide the data structure to every thread.
Each thread will access the “ith” string in the data structure, were “I” is the index at which the thread was created. Each thread will find the number of vowels in the string and add the vowel count to the global variable “vowel_count” which has an initial value 0. 
The main thread will print the total number of vowels in the file.*/

/***************************************************************************
        file name : count_vowel.c
        description : count the no of vowels in a file use of pthread
                       and mutex to avoid race condition
        Invoke the executable as a.out
****************************************************************************/

/*****************************header files***********************************/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
#define MAX 1024
#define COL 30

void *th_f(void *arg);
int read_file(char name[], char *store_string[]);
int tokenize(char line[],char *store_string[]);
void free_arr(char *store_string[], int index);
int counter;

//pthread_mutex_t vowel_count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t vowel_count;

/*******************************MAIN*************************************/
int main(int argc, char *argv[])
{
        int child,i,creat,ret_join;
        pthread_t nid[COL];
        char *store_string[COL];
        if (argc < 2)                           // error handling for command line argument
        {
                printf("not enough argument\n");
                exit(1);
        }
        child = read_file(argv[1],store_string);
        printf("value of child %d\n",child);
        for(i=0; i<child; i++)
                printf("dhjcfg %s\n",store_string[i]);
        for ( i = 0; i < child; i++)
        {
                printf("thread creadted %d\n",i);
                creat = pthread_create(&nid[i],NULL, th_f, (void *)store_string[i]);
                if (creat)
                {
                        printf("error in creating child %d\n",i);
                        exit(1);
                }
        }
        for ( i = 0; i < child; i++)
        {
                ret_join = pthread_join(nid[i],NULL);
                if (ret_join)
                {
                        printf("error in joining %d\n",i);
                        exit(1);
                }
        }
        printf("In Main: count of vowel is %d\n",counter);
        free_arr(store_string, child);
        exit(0);                                //main exits
}

void free_arr(char *store_string[], int index)
{
        int i;
        for(i =0; i< index; i++)
        {
                free(store_string[i]);
        }
}

void *th_f(void *arg)
{
        char *str;
        str = (char *)arg;
        int len,index;
        len = strlen(str);
        printf("val rcv is %s\n",str);
        printf("len is %d\n",len);
        for (index = 0; index < len; index++)
        {  //   pthread_mutex_lock(&vowel_count);
                if (str[index] == 'a'||str[index] == 'e'||str[index] == 'i'||str[index] == 'o'||str[index] == 'u')
                {
                        counter++;
                }
        //      pthread_mutex_unlock(&vowel_count);
        }
        pthread_exit(NULL);
}

int read_file(char name[], char *store_string[])
{
        int fp,count,size,byte_read=0;
        char buf[MAX];
        memset(buf,'\0',MAX);
        printf("file name %s\n",name);
        fp = open(name , O_RDONLY);
        if (fp < 0)
        {
                printf("cannot open file\n");
                exit(1);
        }
        size = lseek(fp, 0, SEEK_END);
        printf("size of file %d\n",size);
        lseek(fp, 0, SEEK_SET);
        byte_read = read(fp, buf, size);
        printf("fp is %d\n",fp);
        printf("byte read %d\n",byte_read);
        printf("in read_file %s\n",buf);
        if (byte_read < 0)
        {
                printf("error in reading\n");
                exit(1);
        }
        count = tokenize(buf, store_string);
        printf("count is %d\n",count);
        return count;
}

int tokenize(char line[],char *store_string[])
{
/*      keep the copy of original line as strtok changes lines into token      */
        int word_count=0,i =0;
        char *token;
       // word_count counts the number of words in line
        printf("string is in tokenize%s\n",line);
//      printf("hello how are you\n");
        token = strtok(line,",");
        while(token!=NULL)
        {
                store_string[i] = (char *)calloc(sizeof(token)+1,sizeof(char));
                if(store_string[i] == NULL)
                {
                        printf("calloc fail\n");
                        exit(1);
                }
                word_count++;
                strcpy(store_string[i],token);
              printf("token is %s\n",token);
                //token = strtok(NULL," ");
           //     strcpy(arr[i], reverse(token,i));
        //      printf("after reverse %s\n",arr[i]);
                token = strtok(NULL,",");
                i++;
        }
        printf("word count is %d\n",i);
        //strcpy(line,copy);
        // copy original line back to its position
        return i;
}


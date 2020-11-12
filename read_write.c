#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_WRITER 3
#define N_READER 5
#define W_SLEEP  1
#define R_SLEEP  1

pthread_t wid[N_WRITER],rid[N_READER];
const int MAX_RAND = 1000;
pthread_mutex_t fileLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readerCntLock = PTHREAD_MUTEX_INITIALIZER;
int data = 0;
int readerCnt = 0;
void write()
{
	int rd = rand()%MAX_RAND;
	printf("write %d\n",rd);
	data = rd;
}
void read()
{
	printf("read %d\n",data);
}
void * writer(void * in)
{
	while(1)
	{
		pthread_mutex_lock(&fileLock);
		write();
		pthread_mutex_unlock(&fileLock);
		sleep(W_SLEEP);
	}
	pthread_exit((void *) 0);
}
 
void * reader (void * in)
{
	while(1)
	{
		pthread_mutex_lock(&readerCntLock);
		readerCnt++;
		if(readerCnt == 1){
			pthread_mutex_lock(&fileLock);
		}
		pthread_mutex_unlock(&readerCntLock);
		
		read();
		
		pthread_mutex_lock(&readerCntLock);
		readerCnt--;
		if(readerCnt == 0){
			pthread_mutex_unlock(&fileLock);
		}
		pthread_mutex_unlock(&readerCntLock);
		sleep(R_SLEEP);
	}
	pthread_exit((void *) 0);
}
 
int main()
{
	int i = 0;
	for(i = 0; i < N_WRITER; i++)
	{
		pthread_create(&wid[i],NULL,reader,NULL);
	}
	for(i = 0; i < N_READER; i++)
	{
		pthread_create(&rid[i],NULL,writer,NULL);
	}
	while(1){
		sleep(10);
	}
	return 0;
}

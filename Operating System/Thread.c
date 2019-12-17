#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 5

pthread_mutex_t mut;
int sum = 0 ;
void* run(void* args)
{
	for(int i = 0;i<10000000;i++)
	{
		pthread_mutex_lock(&mut);
		sum++;
		pthread_mutex_unlock(&mut);
		
	}
	return NULL;
}
int main()
{
	pthread_t thread_id[NUM_THREADS];
	pthread_mutex_init(&mut,NULL);
	for(int i = 0;i<NUM_THREADS;i++)
	{
		pthread_create(&thread_id[i],NULL,run,NULL);
	}
	for(int i = 0;i<NUM_THREADS;i++)
	{
		pthread_join(thread_id[i],NULL);
	}
	printf("Sum = %d\n",sum);
	pthread_mutex_destroy(&mut);
}
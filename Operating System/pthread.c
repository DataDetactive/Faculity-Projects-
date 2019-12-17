#include <stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <pthread.h>
#include <stdio.h>



#define NTHREADS 4
#define N 100
int array[N];
int sum=0;



void *runner(void *ThreadNumber);

int main(int argc,char *argv[])
{
	pthread_t thread_id[NTHREADS];
	int i ;
	for(i=0; i<N; i++)
		array[i]=i;
	for(i=0;i<NTHREADS;i++)
		pthread_create(&thread_id[i], NULL, runner, i);
	for (int j=0; j<NTHREADS; j++)
		pthread_join(thread_id[j], NULL);
	printf("sum  =  %d\n",sum);

}


void *runner(void *ThreadNumber)
{


	int tsum = 0;
	int tn = (int) ThreadNumber;
	int chunksize = N/ NTHREADS ;
	for (int i= tn*chunksize; i<(tn+1)*chunksize; i++)
		tsum += array[i];
	printf("thread %d  Sum = %d \n", tn, tsum);
	sum+= tsum;


}



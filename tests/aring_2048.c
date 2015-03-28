#include "../aring.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

struct atomic_ring * a;
struct timespec * sleepytime;

#define SIZE 2048
#define ITERS 100000000
#define NSLEEP 1000

void * producer(void * v);
void * consumer(void * v);

int main()
{
	pthread_t t;
	a = calloc(1, sizeof a);
	aring_init(a, SIZE);
	sleepytime = calloc(1, sizeof sleepytime);
	sleepytime->tv_nsec = NSLEEP;
	pthread_create(&t, NULL, &consumer, NULL);
	producer(NULL);
	pthread_join(t, NULL);
	printf("Complete\n");
}

void * producer(void * v)
{
	unsigned long long i = 0;
	int chk = 0;
	while(i < ITERS)
	{
		chk = aring_give(a, (void *) i);
		if(chk == -1)
		{
			/*printf("1P: sleep\n");*/
			nanosleep(sleepytime, NULL);
			continue;
		}
		++i;
		/*printf("1P: gave one\n");*/
	}
	printf("1P: Complete\n");
	return NULL;
}

void * consumer(void * v)
{
	unsigned long long i = 0;
	void * item;
	int chk;
	while(i < ITERS)
	{
		chk = aring_take(a, &item);
		if(chk == -1)
		{
			/*printf("1C: sleep\n");*/
			nanosleep(sleepytime, NULL);
			continue;
		}
		if((unsigned long long) item != i)
			printf("1C: it's all broken. item is %lld and should be %lld\n", (unsigned long long) item, i);
		/*printf("1C: took one\n");*/
		i++;
	}
	printf("1C: Complete\n");
	return NULL;
}

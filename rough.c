#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>

sem_t mutex;

void* somefunc(void* arg)
{
	sem_wait(&mutex);
		for(int i = 0; i < 100; i++)
		{
			printf("I entered\n");
		}
	sem_post(&mutex);
}
int main()
{
	sem_init(&mutex, 0, 1);
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, somefunc, NULL);
	pthread_create(&tid2, NULL, somefunc, NULL);

	for(int i = 0; i < 600; i++)
	{

		int val;
		sem_getvalue(&mutex, &val);
		printf("%d ", val);
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);


	sem_destroy(&mutex);


}

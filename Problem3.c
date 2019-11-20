#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>

sem_t wrt;
sem_t mutex;
sem_t rdr;			//Initialized to 5
sem_t mutex_temp1;
sem_t mutex_temp2;
sem_t mutex_temp3;
sem_t mutex_temp4;
sem_t mutex_temp5;



int blocked = 0;
int readcount = 0;
int no_overtake_readers = 0;
int reader_num = 0;
int writer_num = 0;
int flag = 0;
int hello = 0;
int hello1 = 0;

void* reader(void* arg)
{
	int rdr_value;
	int wrt_wait_value;

	sem_wait(&mutex_temp5);
		if(blocked > 0)
		{
			++no_overtake_readers;
			printf("Overtaking request, %d %d\n", no_overtake_readers, blocked);
			hello1 = 1;
			if(no_overtake_readers > 2)
			{
				no_overtake_readers = 0;
				hello = 1;
				sem_wait(&wrt);
				flag = 1;
			}
		}
	sem_post(&mutex_temp5);



	sem_wait(&mutex);

		++readcount;
		if(readcount == 1 && flag == 0)
		{
			sem_wait(&wrt);
		}
		flag = 0;	

	sem_post(&mutex);



	sem_wait(&rdr);
		sem_wait(&mutex_temp2);
			++reader_num;
			rdr_value = reader_num;
		sem_post(&mutex_temp2);
		//Reader Critical Section
		for(int i = 0; i < 50; i++)
		{
			printf("Reader[%d]  %d\n", rdr_value, i);
		}
	sem_post(&rdr);


	
	sem_wait(&mutex);
		--readcount;
		if(readcount == 0)
		{
			printf("I am in readcount==0\n");
			sem_post(&wrt);
		}
	sem_post(&mutex);
}

void* writer(void* arg)
{
	printf("I am writer\n");



	sem_wait(&mutex_temp3);
		++blocked;
	sem_post(&mutex_temp3);

	sem_wait(&wrt);
		sem_wait(&mutex_temp4);
			--blocked;
		sem_post(&mutex_temp4);
		//Writer Critical section
		++writer_num;
		for(int i = 0; i < 50; i++)
		{
			printf("Writer[%d]  %d\n", writer_num, i);
		}
		printf("Writers waiting Semaphore value %d\n", blocked);
	sem_post(&wrt);
}

int main()
{
	sem_init(&wrt, 0, 1);
	sem_init(&mutex, 0, 1);
	sem_init(&rdr, 0, 5);
	sem_init(&mutex_temp1, 0, 1);
	sem_init(&mutex_temp2, 0, 1);
	sem_init(&mutex_temp3, 0, 1);
	sem_init(&mutex_temp4, 0, 1);
	sem_init(&mutex_temp5, 0, 1);

	pthread_t tid_table[300];
	srand(time(0));

	pthread_create(&tid_table[0], NULL, writer, NULL);
	pthread_create(&tid_table[1], NULL, writer, NULL);
	pthread_create(&tid_table[2], NULL, reader, NULL);
	pthread_create(&tid_table[3], NULL, reader, NULL);
	pthread_create(&tid_table[4], NULL, reader, NULL);
	pthread_create(&tid_table[5], NULL, reader, NULL);
	pthread_create(&tid_table[6], NULL, reader, NULL);
	pthread_create(&tid_table[7], NULL, reader, NULL);
	pthread_create(&tid_table[8], NULL, reader, NULL);
	pthread_create(&tid_table[9], NULL, reader, NULL);
	pthread_create(&tid_table[28], NULL, reader, NULL);
	pthread_create(&tid_table[27], NULL, reader, NULL);
	pthread_create(&tid_table[26], NULL, reader, NULL);
	pthread_create(&tid_table[25], NULL, reader, NULL);
	pthread_create(&tid_table[24], NULL, reader, NULL);
	pthread_create(&tid_table[23], NULL, reader, NULL);
	pthread_create(&tid_table[22], NULL, reader, NULL);
	pthread_create(&tid_table[21], NULL, reader, NULL);
	pthread_create(&tid_table[20], NULL, reader, NULL);
	pthread_create(&tid_table[19], NULL, reader, NULL);
	pthread_create(&tid_table[18], NULL, reader, NULL);
	pthread_create(&tid_table[17], NULL, writer, NULL);
	pthread_create(&tid_table[10], NULL, reader, NULL);
	pthread_create(&tid_table[11], NULL, writer, NULL);
	pthread_create(&tid_table[12], NULL, writer, NULL);
	pthread_create(&tid_table[13], NULL, reader, NULL);
	pthread_create(&tid_table[14], NULL, reader, NULL);
	pthread_create(&tid_table[15], NULL, reader, NULL);
	pthread_create(&tid_table[16], NULL, reader, NULL);
	pthread_create(&tid_table[29], NULL, reader, NULL);
	pthread_create(&tid_table[30], NULL, reader, NULL);
	pthread_create(&tid_table[31], NULL, reader, NULL);
	pthread_create(&tid_table[32], NULL, reader, NULL);
	pthread_create(&tid_table[33], NULL, reader, NULL);
	pthread_create(&tid_table[34], NULL, reader, NULL);
	pthread_create(&tid_table[35], NULL, reader, NULL);
	pthread_create(&tid_table[36], NULL, reader, NULL);
	pthread_create(&tid_table[37], NULL, reader, NULL);
	sleep(1);
	pthread_create(&tid_table[38], NULL, reader, NULL);
	pthread_create(&tid_table[39], NULL, reader, NULL);
	pthread_create(&tid_table[40], NULL, writer, NULL);
	pthread_create(&tid_table[49], NULL, reader, NULL);
	pthread_create(&tid_table[48], NULL, reader, NULL);
	pthread_create(&tid_table[47], NULL, reader, NULL);
	pthread_create(&tid_table[46], NULL, reader, NULL);
	pthread_create(&tid_table[45], NULL, reader, NULL);
	pthread_create(&tid_table[44], NULL, reader, NULL);
	pthread_create(&tid_table[43], NULL, reader, NULL);
	pthread_create(&tid_table[42], NULL, reader, NULL);
	pthread_create(&tid_table[41], NULL, reader, NULL);
	/*for(int i = 0; i < 300; i++)
	{
		if(rand()%2 == 0)
		{
			pthread_create(&tid_table[i], NULL, reader, NULL);
		}
		else
		{
			pthread_create(&tid_table[i], NULL, writer, NULL);
		}
	}*/

	for(int i = 0; i < 49; i++)
	{
		pthread_join(tid_table[i], NULL);
	}

	if(hello == 1)
	{
		printf("I was called....\n");	
	}
	if(hello1 == 1)
	{
		printf("Overtake was tried...\n");
	}

	sem_destroy(&wrt);
	sem_destroy(&mutex);
	sem_destroy(&rdr);
	sem_destroy(&mutex_temp1);
	sem_destroy(&mutex_temp2);
	sem_destroy(&mutex_temp3);
	sem_destroy(&mutex_temp4);
	sem_destroy(&mutex_temp5);
}

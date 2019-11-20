#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>


sem_t ready_customer;        	//Number of customers ready for haircut
sem_t haircut_complete;
sem_t door; 			//Door is open
int customers_waiting = 0;
sem_t turn;
sem_t ready_barber;
sem_t leave;


int tot_chairs;
int cuttings = 0;

void* hairstyler(void* arg)
{
	while(1)
	{
		++cuttings;
		sem_post(&ready_barber);
		sem_wait(&ready_customer);
		--customers_waiting;
			//Cut customer's hair
		printf("Cutting hair %d        ", cuttings);
		fflush(stdout);
		sem_post(&haircut_complete);
		sem_wait(&leave);
	}
	pthread_exit(0);
}

void* customer(void* arg)
{
	sem_wait(&door);
	if(customers_waiting == tot_chairs)
	{
		sem_post(&door);
//		printf("Chairs full, I am leaving\n");
		pthread_exit(0);
	}
	else
	{
		++customers_waiting;
		sem_post(&door);
	}

	//Correct
	sem_wait(&turn);
		sem_wait(&ready_barber);
		sem_post(&ready_customer);
			//get hair cut
		sem_wait(&haircut_complete);
		printf("Haircut done! %d\n", cuttings);
		fflush(stdout);
		sem_post(&leave);
	sem_post(&turn);

	pthread_exit(0);
}

int main()
{
	printf("Total chairs for waiting: ");
	scanf("%d", &tot_chairs);

	sem_init(&ready_customer, 0, 0);
	sem_init(&haircut_complete, 0, 0);
	sem_init(&door, 0, 1);
	sem_init(&turn, 0, 1);
	sem_init(&ready_barber, 0, 0);
	sem_init(&leave, 0, 0);

	pthread_t barber_tid;
	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	pthread_create(&barber_tid, NULL, hairstyler, NULL);
	pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);

	pthread_t** customer_tids;
       		
	//Generate a random number random_n
	srand(time(0));
	int random_n = rand()%30;
	customer_tids = malloc(sizeof(pthread_t*)*random_n);
	for(int i = 0; i < random_n; i++)
	{
		int random_m = rand()%80;
		//Generate a random number random_m
		printf("random_m: %d\n", random_m);
		customer_tids[i] = malloc(sizeof(pthread_t)*random_m);
		for(int j = 0; j < random_m; j++)
		{
			pthread_create(&customer_tids[i][j], NULL, customer, NULL);
		}
		sleep(1);
	}	
	
	for(int i = 0; i < random_n; i++)
	{
		for(int j = 0; j < sizeof(customer_tids[i])/(sizeof(pthread_t)); j++)
		{
			pthread_join(customer_tids[i][j], NULL);
		}
	}

	sem_destroy(&ready_customer);
	sem_destroy(&haircut_complete);
	sem_destroy(&door);
	sem_destroy(&turn);
}

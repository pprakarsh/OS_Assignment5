#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>
#define k 7

sem_t ready_customer[k];        	//Number of customers ready for haircut
sem_t haircut_complete[k];
sem_t door; 				//Door is open
int customers_waiting = 0;
sem_t turn;
sem_t ready_barber[k];
sem_t leave[k];
sem_t mutex;
sem_t mutex1;
sem_t mutex2;
sem_t mutex3;

int barber_chair_state[k];

int tot_chairs;
int cuttings = 0;
int cutt[k];
int done[k];

int count_leave = 0;

void* hairstyler(void* arg)
{
	int* barb_ptr = (int* )arg;
	int barb = *barb_ptr;
	cutt[barb] = 0;
	while(1)
	{

		sem_post(&ready_barber[barb]);
		sem_wait(&ready_customer[barb]);

		sem_wait(&mutex2);
			--customers_waiting;
		sem_post(&mutex2);

		//Cut customer's hair
			
		++cutt[barb];
		printf("Cutting hair..[%d] %d        ", barb, cutt[barb]);
		fflush(stdout);
		sem_post(&haircut_complete[barb]);

		sem_wait(&mutex1);
			++cuttings;
		sem_post(&mutex1);

		sem_wait(&leave[barb]);
		barber_chair_state[barb] = 0;
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
		sem_wait(&mutex3);
			++count_leave;
		sem_post(&mutex3);

		pthread_exit(0);
	}
	else
	{
		++customers_waiting;
		sem_post(&door);
	}

	int barb = k;
	sem_wait(&turn);
		sem_wait(&mutex);
		while(1)
		{
			for(int i = 0; i < k; i++)
			{
				if(barber_chair_state[i] == 0)
				{
					barber_chair_state[i] = 1;
					barb = i;
					break;
				}
			}
			if(barb != k)
			{
				break;
			}
		}
		sem_post(&mutex);
		sem_wait(&ready_barber[barb]);
		sem_post(&ready_customer[barb]);
			//get hair cut
		sem_wait(&haircut_complete[barb]);
		++done[barb];
		printf("Haircut done![%d] %d\n", barb, cutt[barb]);
		fflush(stdout);
		sem_post(&leave[barb]);
	sem_post(&turn);

	printf("\nExiting customer, hair cut by Babrber %d\n", barb);
	pthread_exit(0);
}

int main()
{
	printf("Total chairs for waiting: ");
	scanf("%d", &tot_chairs);

	for(int i = 0; i < k; i++)
	{
		sem_init(&ready_customer[i], 0, 0);
	}
	for(int i = 0; i < k; i++)
	{
		sem_init(&ready_barber[i], 0, 0);
	}
	for(int i = 0; i < k; i++)
	{
		sem_init(&haircut_complete[i], 0, 0);
	}
	for(int i = 0; i < k; i++)
	{
		sem_init(&leave[i], 0, 0);
	}

	sem_init(&door, 0, 1);
	sem_init(&turn, 0, k);
	sem_init(&mutex, 0, 1);
	sem_init(&mutex1, 0, 1);
	sem_init(&mutex2, 0, 1);
	sem_init(&mutex3, 0, 1);

	int* barb_no;
	for(int i = 0; i < k; i++)
	{
		pthread_t barber_tid;
		pthread_attr_t tattr;
		pthread_attr_init(&tattr);
		pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
		barb_no = malloc(sizeof(int)); *barb_no = i;	
		pthread_create(&barber_tid, NULL, hairstyler, barb_no);
	}

	pthread_t** customer_tids;
       		
	//Generate a random number random_n
	srand(time(0));
	int random_n = rand()%30;
	random_n = 30;
	customer_tids = malloc(sizeof(pthread_t*)*random_n);
	for(int i = 0; i < random_n; i++)
	{
		int random_m = (rand()%80)+(8);
		random_m = 50;
		//Generate a random number random_m
		customer_tids[i] = malloc(sizeof(pthread_t)*random_m);
		for(int j = 0; j < random_m; j++)
		{
			pthread_create(&customer_tids[i][j], NULL, customer, NULL);
		}
		sleep(2);
	}	
	
	for(int i = 0; i < random_n; i++)
	{
		for(int j = 0; j < sizeof(customer_tids[i])/(sizeof(pthread_t)); j++)
		{
			pthread_join(customer_tids[i][j], NULL);
		}
	}


	int sum = 0;
	for(int i = 0; i < k; i++)
	{
		printf("i: %d ------    %d    %d\n", i, cutt[i], done[i]);
		sum += cutt[i];
	}

	printf("Total customers left: %d\n", count_leave);

	sum = sum + count_leave;
	printf("Total customers visited the shop: %d\n", sum);

	//Destroying all semaphores created

	sem_destroy(&door);
	sem_destroy(&turn);
	sem_destroy(&mutex);
	sem_destroy(&mutex1);
	sem_destroy(&mutex2);
	sem_destroy(&mutex3);


	for(int i = 0; i < k; i++)
	{
		sem_destroy(&ready_customer[i]);
	}
	for(int i = 0; i < k; i++)
	{
		sem_destroy(&ready_barber[i]);
	}
	for(int i = 0; i < k; i++)
	{
		sem_destroy(&haircut_complete[i]);
	}
	for(int i = 0; i < k; i++)
	{
		sem_destroy(&leave[i]);
	}
}

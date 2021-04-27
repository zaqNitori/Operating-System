#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ITEM_AMOUNT 3
#define PRODUCE_ITEM 5

int buf[3];
int count = 0;
int next_con = 0;
int next_pro = 0;
int isStop = 0;

pthread_t con,pro;
pthread_mutex_t mutex;
sem_t sem_con,sem_pro;

void* consume();
void* produce();


int main(void)
{
	puts("Producer Consumer Program Start!!\n");
	pthread_create(&con,NULL,consume,NULL);
	pthread_create(&pro,NULL,produce,NULL);	
	pthread_mutex_init(&mutex,NULL);
	memset( buf, 0, 3);
	sem_init(&sem_pro, 0, 0);
	
	pthread_join(con,NULL);
	pthread_join(pro,NULL);

	puts("Producer Consumer Program End!!\n");
	return 0;
}

void* consume()
{
	sleep(2);
	puts("Consumer Start consuming\n");
	
	while(!isStop || count != 0)
	{
		while(count == 0);
		pthread_mutex_lock(&mutex);

		printf("Consumer consume Item %d\n", buf[next_con]);
		sleep(2);
		buf[next_con++] = 0;
		printf("Item Buffer: %d, %d, %d\n\n", buf[0], buf[1], buf[2]);
		next_con %= MAX_ITEM_AMOUNT;
		count--;

		pthread_mutex_unlock(&mutex);
	}
	puts("Consumer Stop Consuming\n");
	pthread_exit(NULL);
}

void* produce()
{
	puts("Producer Start Producing\n");
	int i;
	for(i = 0;i < PRODUCE_ITEM;i++)
	{
		while(count == MAX_ITEM_AMOUNT);
		pthread_mutex_lock(&mutex);
		
		printf("Producer produce Item %d\n",i+1);
		sleep(1);
		buf[next_pro++] = i+1;
		printf("Item Buffer: %d, %d, %d\n\n", buf[0], buf[1], buf[2]);
		next_pro %= MAX_ITEM_AMOUNT;
		count++;
	
		pthread_mutex_unlock(&mutex);
	}
	isStop = 1;
	puts("Producer Stop Producing\n");
	pthread_exit(NULL);
}	

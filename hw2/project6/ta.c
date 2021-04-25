#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define SEAT_AMOUNT 3

#define DELAY_DURATION 5

sem_t sem_stu;
sem_t sem_ta;

pthread_mutex_t mutex;

int seat[3];
int count = 0;
int next_seat = 0;
int next_teach = 0;
int stu_num = 5;
int *stuStatus;
int *stuID;

pthread_t *stu;
pthread_t ta;

void random_delay();
void* stu_process();
void* ta_teaching();
void initial();
int checkStatus();

int main(void)
{
	puts("TA teaching program Start!!!");
	
	int i;
	
	srand(time(NULL));	

	stu = (pthread_t*)malloc(sizeof(pthread_t) * stu_num);
	stuStatus = (int*)malloc(sizeof(int) * stu_num);
	stuID = (int*)malloc(sizeof(int) * stu_num);

	initial();

	pthread_join(ta, NULL);
	for(i = 0;i < stu_num; i++)
		pthread_join(stu[i],NULL);
	
	puts("\nTA teaching program End!!\n");
	return 0;
}

void initial()
{
	int i,id;
	sem_init(&sem_stu, 0, 0);
	sem_init(&sem_ta, 0, 1);
	pthread_mutex_init(&mutex, NULL);

	pthread_create(&ta, NULL, ta_teaching, NULL);
	for(i = 0;i < 5;i++)
	{
		stuID[i] = i + 1;
		pthread_create(&stu[i], NULL, stu_process, (void*) &stuID[i]);
		stuStatus[i] = 0;
	}
}

void* stu_process(void* data)
{
	int id = *(int*) data;

	while(1)
	{	
		random_delay();
		pthread_mutex_lock(&mutex);
		
		if(count < SEAT_AMOUNT && !stuStatus[id-1])
		{
			seat[next_seat] = id;
			count++;
			printf("student %d is waiting now!\n",id);
			next_seat = (next_seat + 1) % SEAT_AMOUNT;

			pthread_mutex_unlock(&mutex);

			sem_post(&sem_stu);
			sem_wait(&sem_ta);
		}
		else if(stuStatus[id-1])
		{
			pthread_mutex_unlock(&mutex);
			printf("student %d is going home!\n",id);
			pthread_exit(NULL);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			printf("There's no available seat now for student %d!\n",id);
		}
	}
}

void* ta_teaching()
{
	int tmp;
	while(1)
	{
		sem_wait(&sem_stu);

		pthread_mutex_lock(&mutex);

		printf("\nwaiting line: student %d, student %d, student %d\n",seat[0],seat[1],seat[2]);
		printf("TA is now teaching student %d\n",seat[next_teach]);

		count--;
		tmp = seat[next_teach];
		stuStatus[tmp-1] = 1;
		seat[next_teach] = 0;

		next_teach = (next_teach + 1) % SEAT_AMOUNT;

		random_delay();
		printf("TA finish teaching student %d\n",tmp);

		if(!count) printf("TA is sleeping now!\n\n");

		if(checkStatus())
		{
			printf("TA finish teaching All student.TA is going home!\n");
			pthread_mutex_unlock(&mutex);
			break;
		}
		
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_ta);
	}
	pthread_exit(NULL);
}

void random_delay()
{
	int r = (rand() % DELAY_DURATION) + 1;
	sleep(r);
}

int checkStatus()
{
	int i;
	for(i = 0;i < stu_num; i++)
		if(!stuStatus[i])
			return 0;
	return 1;
}



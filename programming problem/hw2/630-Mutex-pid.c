#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define MIN_PID 500
#define MAX_PID 3000
#define THREAD_AMOUNT 30

int last_release_pid=0;
int count = 0;

int allocate_map();
int allocate_pid();
void release_pid(int pid);
void allocate_pthread();
void* randomAllocate_pid();

pthread_t *pthread_pool;
char *pid_map;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(void)
{
    srand(time(NULL));
    printf("Start process PID allocate\n\n");
    int pid,process,i=0;
    int suc = allocate_map();
    if(suc)
    {
        allocate_pthread();
        for(i = 0;i < THREAD_AMOUNT; i++)
            pthread_join(pthread_pool[i],NULL);

        free(pid_map);
    }
    else printf("\nFailed to allocate memory for pid_map!\n");

    printf("\nEnd Process PID allocate\n");
    return 0;
}

void allocate_pthread()
{
    int i;
    pthread_pool = (pthread_t*)malloc(sizeof(pthread_t)*THREAD_AMOUNT);
    for(i = 0;i < THREAD_AMOUNT; i++)
	pthread_create(&pthread_pool[i],NULL,randomAllocate_pid,NULL);
}

void* randomAllocate_pid()
{
    int r;
    while(1)
    {
	r = (rand() % 2500) + 501;

	pthread_mutex_lock( &mutex1 );
	if(!pid_map[r])
	{
	    printf("%d. Allocate PID = %d\n",count++,r);
	    pid_map[r] = 1;
	    release_pid(r);
	    pthread_mutex_unlock( &mutex1 );
	    break;
	}

    }
    pthread_exit(NULL);
}

int allocate_map()
{
    int i;
    pid_map = (char*)malloc(sizeof(char)*(MAX_PID+1));
    if(pid_map)
    {
        for(i=0;i<MIN_PID;i++)
            pid_map[i] = 1;
        for(i=MIN_PID;i<MAX_PID;i++)
            pid_map[i] = 0;
        return 1;
    }
    else return -1;
}

int allocate_pid()
{
    int i;
    if(pid_map[last_release_pid] == 0)
    {
        pid_map[last_release_pid] = 1;
        return last_release_pid;
    }
    for(i=MIN_PID;i<MAX_PID;i++)
        if(pid_map[i] == 0)
        {
            pid_map[i] = 1;
            return i;
        }
    return -1;
}

void release_pid(int pid)
{
    if(pid < MIN_PID && pid > MAX_PID)
    {
        printf("\nInvalid PID: It should be between 300~5000\n");
        return;
    }
    pid_map[pid] = 0;
    last_release_pid = pid;
    printf("PID %d released!\n",pid);
    sleep(1);
    return;
}

void showPIDUsing()
{
    int i;
    printf("Allocated PID\n");
    for(i=MIN_PID;i<MAX_PID;i++)
        if(pid_map[i] == 1)
            printf("%d.%d\n",i,pid_map[i]);
}

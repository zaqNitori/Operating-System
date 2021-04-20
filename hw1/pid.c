#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 500
#define MAX_PID 3000

int last_release_pid=0;
int allocate_map();
int allocate_pid();
void release_pid(int pid);

char *pid_map;

int main(void)
{
    printf("Start process PID allocate\n");
    int pid,process,i=0;
    int suc = allocate_map();
    if(suc)
    {
        while(i++<10)
        {
            pid = allocate_pid();
            printf("Process %d: get PID = %d\n",i,pid);
        }
        showPIDUsing();
        release_pid(505);
        release_pid(509);
        showPIDUsing();

        free(pid_map);
    }
    else printf("\nFailed to allocate memory for pid_map!\n");

    printf("\nEnd Process PID allocate\n");
    return 0;
}

int allocate_map()
{
    pid_map = (char*)malloc(sizeof(char)*(MAX_PID+1));
    if(pid_map)
    {
        for(int i=0;i<MIN_PID;i++)
            pid_map[i] = 1;
        for(int i=MIN_PID;i<MAX_PID;i++)
            pid_map[i] = 0;
        return 1;
    }
    else return -1;
}

int allocate_pid()
{
    if(pid_map[last_release_pid] == 0)
    {
        pid_map[last_release_pid] = 1;
        return last_release_pid;
    }
    for(int i=MIN_PID;i<MAX_PID;i++)
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
    return;
}

void showPIDUsing()
{
    printf("Allocated PID\n");
    for(int i=MIN_PID;i<MAX_PID;i++)
        if(pid_map[i] == 1)
            printf("%d.%d\n",i,pid_map[i]);
}

#include <stdio.h>
#include <pthread.h>

void* prime(void* data)
{
	printf("Start Threading Prime\n");

	int *n = (int*) data;
	int num = (*n),prime;
	int i,j;
	
	printf("following numbers are prime numbers\n");
	num > 2 ? printf("2\n") : pthread_exit(NULL);
	for(i = 3;i < num;i+=2)
	{	
		prime = 1;
		for(j = 2;j*j<=i;j++)
		{
			if(i % j == 0)
			{
				prime = 0;
				break;
			}
		}
		if(prime) printf("%d\n",i);
	}
	printf("End Threading Prime\n");
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t t;
	int num;
	printf("Start Process\n");
	printf("input number: ");
	scanf("%d",&num);

	pthread_create(&t,NULL,prime,(void*)&num);
	pthread_join(t,NULL);
	
	printf("End Process\n");
	return 0;
}
	


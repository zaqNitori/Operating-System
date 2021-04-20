#include <stdio.h>
#include <pthread.h>

typedef struct
{
	int len;
	int number[8];
}numArray;

typedef struct
{
	numArray *numArr[2];
	int numSort[16];
}numCollect;

void swap(int *n1,int *n2)
{
	int tmp;
	tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
}

void* multiSort(void* data)
{
	printf("Before Sorting\n");
	numArray *num = (numArray*)data;
	int i,j;
	for(i=0;i<num->len;i++) printf("%d ",num->number[i]);
	puts("");
	for(i=0;i<num->len;i++)
		for(j=i+1;j<num->len;j++)
			if(num->number[i] > num->number[j])
				swap(&num->number[i],&num->number[j]);
	printf("After Sorting\n");
	for(i=0;i<num->len;i++) printf("%d ",num->number[i]);
	puts("\n");
	pthread_exit(NULL);
}

void* mergeNumArr(void* data)
{
	printf("merge now\n");
	numCollect *nc = (numCollect*)data;
	int i=0,j=0,now=0;
	int len1 = nc->numArr[0]->len;
	int len2 = nc->numArr[1]->len;
	
	while(1)
	{
		if(i < len1 && j < len2)
			nc->numSort[now++] = (nc->numArr[0]->number[i] < nc->numArr[1]->number[j]?nc->numArr[0]->number[i++]:nc->numArr[1]->number[j++]);
		else if(i < len1)
			nc->numSort[now++] = nc->numArr[0]->number[i++];
		else if(j < len2)
			nc->numSort[now++] = nc->numArr[1]->number[j++];
		else
			break;
	}
	pthread_exit(NULL);
}

int main(void)
{
	numArray n1,n2;
	numCollect nCol;
	nCol.numArr[0] = &n1;
	nCol.numArr[1] = &n2;
	int amount,i;
	printf("Start MultiSorting Algorithm\n");
	printf("Input Amount of Numbers <= 16: ");
	scanf("%d",&amount);
	n1.len = n2.len = amount / 2;
	if(amount & 1) n1.len++;

	printf("Input %d number: ",amount);
	for(i=0;i<n1.len;i++) scanf("%d",&n1.number[i]);
	for(i=0;i<n2.len;i++) scanf("%d",&n2.number[i]);

	pthread_t p1,p2,p3;
	pthread_create(&p1,NULL,multiSort,(void*) &n1);
	pthread_create(&p2,NULL,multiSort,(void*) &n2);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);

	pthread_create(&p3,NULL,mergeNumArr,(void*) &nCol);
	pthread_join(p3,NULL);

	printf("After Merge\n");
	for(i=0;i<amount;i++)
		printf("%d ",nCol.numSort[i]);
	puts("");

	printf("End MultiSorting Algorithm\n");
	return 0;
}



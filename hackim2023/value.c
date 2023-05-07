#include<stdlib.h>
#include<stdio.h>
int main()
{
	int swap1,swap2,count;
	count = 256;
	for(int i = 0;i<count;i++)
	{
		swap1 = rand()%count;
		printf("%d\n",swap1);
		swap2 = (swap1 + 1 + rand() % (count - 1)) % count;
		printf("%d\n",swap2);
	}
}

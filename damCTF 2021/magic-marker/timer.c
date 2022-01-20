#include<stdlib.h>
#include<time.h>
#include<stdio.h>
int main()
{
	srand(time(0));
	printf("%u\n",rand()%0x28);
	printf("%u\n",rand()%0x28);
}

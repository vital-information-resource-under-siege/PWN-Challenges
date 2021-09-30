#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
long unsigned int i;
void no_gadget_here_so_let_me_help_you()
{
	execve("/bin/sh",0,0);
}
void vuln()
{
	sleep(2);
	printf("OOPS!!!!I AM LEAKING CRITICAL STUFF %p %p!!\nI am so Insecure!!!!\n",system,vuln);
	char a[32];
	long unsigned int *j;
	j = &i;
	read(0,a,48);
	printf("\n");
	__asm__("xor %r12,%r12");
	scanf("%lu",j);
}
int main()
{
	vuln();
}


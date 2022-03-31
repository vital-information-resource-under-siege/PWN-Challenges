#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	char a[33];
	char b[2] = {'\n'};
	int fd = open("/home/strikerr/Desktop/CTF_WRITEUP_AND_CTF_TEMP/tetctf/newbie_chall/hash.txt",O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
	memset(a,0,33);
	for(int i =0;i< 256*256;i++)
	{
		srand(i);
		for(int j = 0;j<32;j++)
		{
			a[j] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"[rand()%62];
		}
		write(fd,a,32);
		write(fd,b,1);
		printf("%d\n",i);
	}
	close(fd);
}

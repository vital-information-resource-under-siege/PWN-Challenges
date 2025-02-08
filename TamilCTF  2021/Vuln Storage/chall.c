#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
struct chunk
{
	char * address;
	int request_size;
};
typedef struct chunk chunk;
int main()
{
	printf("============================================================================\n");
	printf("Please don't pwn me.. My creator did not made me up with best code practises\n");
	printf("============================================================================\n");
	int i = 0;
	int choice;
	unsigned long size;
	char *storage;
	int len;
	char input[32];
	chunk m_array[15];
	int free_index;
	int edit_index;
	int view_index;
	int freers;
	while(1)
	{
	printf("Storage space created %d/15\n\n",i);		
	puts("1.Create storage space\n\n2.Destroy the storage space\n\n3.Edit the storage space\n\n4.View the storage space\n\n5.Exit\n");
	printf(">>");
	scanf("%d",&choice);
	switch(choice)
	{
		case 1:
			if((i < 15) && (i >= 0))
			{
				write(1,"\nEnter the size:",16);
				read(0,input,0x1f);
				size = strtoul(input,0,0);
				if((size == system+0x121f63) || (size > 0x17) && (size < 0x3e9) && (size > 0))
				{
					storage = (char *)malloc(size);
					if(storage != 0)
					{
					m_array[i].address = storage;
					m_array[i].request_size = size;
					memset(storage,'\0',16);
					i+=1;
					}
					else
					{
						puts("Chunk request failed");
					}

				}
				else
				{
				puts("Request size should be between 24 and 1000");
				}
			}
			else
			{
				puts("Only 15 requests available");
			}
			break;
		case 2:
			write(1,"\nEnter the index:",17);
			scanf("%d",&free_index);
			if((free_index >= 0) && (free_index < 15))
			{
				if((m_array[free_index].address != NULL) && (free_index < i))
				{
					free(m_array[free_index].address);
					m_array[free_index].address = NULL;
					puts("\nFree\n");
				}
				else
				{
					puts("Trying UAF or Double free is a joke here\n");
				}
			}
			else
			{
				puts("\nTrying out of bounds huh!!Don't make me laugh!! Try better!!\n");
			}
			break;
		case 3:
			write(1,"\nEnter the index:",17);
			scanf("%d",&edit_index);
			if((edit_index >= 0) && (edit_index < 15))
			{
				if((m_array[edit_index].address != NULL) && (edit_index < i))
				{
					write(1,"\nEnter the data:",16);
					int read_size = read(0,m_array[edit_index].address,m_array[edit_index].request_size);
					memset((m_array[edit_index].address + read_size),'\0',1);
					puts("Data successfully written on chunk");
				}
				else
				{
					puts("What are you even trying uaf or double free!!Nothing works here");
				}
			}
			else
			{
				puts("But we cannot give you any integer bugs to exploit");
			}
			break;
		case 4:
			write(1,"\nEnter the index:",17);
			scanf("%d",&view_index);
			if((view_index >= 0) && (view_index < 15))
			{
				if((m_array[view_index].address != NULL) && (view_index < i))
				{
					write(1,"\nHere is your chunk contents\n",28);
					write(1,m_array[view_index].address,m_array[view_index].request_size);
				}
				else
				{
					puts("Guys!! Don't be so stupid to try read after after here!!");
				}
			}
			else
			{
				puts("Sorry no out of bound read using integer bugs");
			}
			break;
		case 5:
			puts("\nExit\n");
			exit(0);
		default :
			puts("\nInvalid Index\n");
	}
	}
}
	

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
struct chunk
{
	char name[20];
	int marks;
	char remarks[56];
	char * address;
	int request_size;
};
typedef struct chunk chunk;
int main()
{
	setbuf(stdout,NULL);
	setbuf(stdin,NULL);
	chunk m_array[30];
	unsigned int i = 0;
	unsigned long size;
	char input[32];
	int choice;
	int index;
	char * m_address;
	int marks;
	while(1)
	{
		putchar(10);
		puts("===============================================");
		puts("Revaluation Panel of Akka University");
		puts("===============================================");
		printf("Remaining papers available %u/30\n\n",i);
		choice = 0;
		write(1,"1.Evaluate a answer sheet\n\n2.Put arrears\n\n3.View the answer sheet again\n\n4.Re-Evaluate a answer sheet\n\n5.Check out\n>>",117);
		scanf("%d",&choice);
		putchar(10);
		switch(choice)
		{
			case 1:
				if((i >= 0) && (i<30))
				{
					write(1,"Enter the size of the student record\n>>",39);
					read(0,input,0x1f);
					size = strtoul(input,0,0);
					if((size == system+0x16219a) || (size > 0x17) && (size < 0x89) && (size > 0))
					{
						m_address = (char *)malloc(size);
						if(m_address != 0)
						{
							write(1,"Enter the student's name\n>>",27);
							read(0,m_array[i].name,sizeof(m_array[i].name));
							write(1,"Enter the student's marks\n>>",28);
							scanf("%d",&marks);
							if((marks >= 40) && (marks <= 100))
							{
								m_array[i].marks = marks;
							}
							else
							{
								puts("Students should be given pass marks which is between 40 and 100.If not falling back to 40");
								m_array[i].marks = 40;
							}
							write(1,"Enter the Remarks for the Students\n>>",37);
							read(0,m_array[i].remarks,sizeof(m_array[i].remarks));
							memset(m_address,'\0',16);
							m_array[i].address = m_address;
							m_array[i].request_size = size;
							write(1,"Enter something as a log for correcting this paper\n>>",53);
							read(0,m_array[i].address,m_array[i].request_size);
							i+=1;
						}
						else
						{
							puts("Sorry can't retrieve the answer sheet");
						}

					}
					else
					{
						puts("Sorry student record size should be only between 24 and 136");
					}
				}
				else
				{
					puts("Sorry No more answer sheets available");
				}
				break;
			case 2:
				write(1,"Enter the index number of the record\n>>",39);
				scanf("%d",&index);
				if((index >= 0) && (index < 30))
				{
					if((m_array[index].address != NULL) && (index < i))
					{
						free(m_array[index].address);
						m_array[index].address = NULL;
						puts("Anna university to Students:Fun panrom");
					}
					else
					{
						puts("Sorry to disappoint you but we don't entertain double arrears here");
					}
				}
				else
				{
					puts("No integer bugs and index value greater than or equal to 30");
				}
				break;
			case 3:
				write(1,"Enter the index number of the record you want to view\n>>",56);
				scanf("%d",&index);
				if((index >= 0) && (index < 30))
				{
					if((m_array[index].address != NULL) && (index < i))
					{
						write(1,"Here are the chunk contents\n",28);
						write(1,m_array[index].address,m_array[index].request_size);
					}
					else
					{
						puts("Sorry we don't prefer Read after getting arrears");
					}
				}
				else
				{
					puts("No integer bugs and index value greater than or equal to 30");
				}
				break;
			case 4:
				write(1,"Enter the index number of the record you want to edit\n>>",56);
				scanf("%d",&index);
				if((index >= 0) && (index < 30))
				{
					if((m_array[index].address != NULL) && (index < i))
					{
						write(1,"Enter the student's name\n>>",27);
                                                read(0,m_array[index].name,sizeof(m_array[index].name));
                                                write(1,"Enter the student's marks\n>>",28);
                                                scanf("%d",&marks);
						if((marks >= 40) && (marks <= 100))
						{
							m_array[index].marks = marks;
						}
						else
						{
							puts("Students should be given pass marks which is between 40 and 100.If not falling back to 40");
							m_array[index].marks = 40;
						}
						size = strlen(m_array[index].remarks);
						write(1,"Enter the Remarks for the Students\n>>",37);
						read(0,m_array[index].remarks,size);
						if(m_array[index].address >= 0x7f0000000000)
						{
							puts("Sorry I can't let you change the answer contents to give pass marks");
							exit(0);
						}
						else
						{
							write(1,"Enter new log for correcting this paper\n>>",42);
							read(0,m_array[index].address,m_array[index].request_size);
						}
					}
					else
					{
						puts("Na Na Na!!!Sorry we don't allow you to write after you got arrears!!Wait for the upcoming semester");
					}
				}
				else
				{
					puts("Sorry no integer bugs and index value greater than 30");
				}
				break;
			case 5:
				puts("Thanks for spoiling most students life by putting arrears for getting revaluation money");
				exit(0);
				break;
			default:
				puts("Sorry!!Wrong option choose from something that is in the menu");
				break;
		}

	}
}

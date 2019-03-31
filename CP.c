#include<stdio.h>
#include<stdlib.h>

// Semaphores
int s_mutex=1;
int s_full=0;
int s_empty=6;

// for tracking current item
int cur_item=0;

int wait(int s)
{
	return (--s);
}

int signal(int s)
{
	return(++s);
}

// Producer Code
void producer()
{
	s_mutex = wait(s_mutex);
	s_full = signal(s_full);
	s_empty = wait(s_empty);
	cur_item++;
	printf("\nItem %d is Produced",cur_item);
	s_mutex=signal(s_mutex);
}

// consumer code
void consumer()
{
	s_mutex=wait(s_mutex);
	s_full=wait(s_full);
	s_empty=signal(s_empty);
	printf("\nItem %d is consumed",cur_item);
	cur_item--;
	s_mutex=signal(s_mutex);
}


int main()
{
	int choice;
	printf("Choose one of the following:\n");
	printf("\n1.Producer\n2.Consumer\n3.Terminate");
	while(1)
	{
		printf("\nEnter Choice:");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:	if((s_mutex==1)&&(s_empty!=0))
						producer();
					else
						printf("Buffer full!");
					break;
			case 2:	if((s_mutex==1)&&(s_full!=0))
						consumer();
					else
						printf("Buffer empty!!");
					break;
			case 3:
					return 0;
					break;
		}
	}

	return 0;
}

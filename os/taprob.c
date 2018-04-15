
#include <pthread.h>		
#include <time.h>			
#include <unistd.h>			
#include <semaphore.h>		
#include <stdlib.h>			
#include <stdio.h>			

pthread_t *student;		
pthread_t TA;				

int ChairsCount = 0;
int CurrentIndex = 0;


sem_t teachingassistentsleeping;
sem_t Student_Sem;
sem_t ChairsSem[3];
pthread_mutex_t accesschair;


void *taactivity();
void *Student_Activity(void *threadID);

int main(int argc, char* argv[])
{
	int numberofstudents;		
	int id;
	srand(time(NULL));

	
	sem_init(&teachingassistentsleeping, 0, 0);
	sem_init(&Student_Sem, 0, 0);
	for(id = 0; id < 3; ++id)			
		sem_init(&ChairsSem[id], 0, 0);

	pthread_mutex_init(&accesschair, NULL);
	
	if(argc<2)
	{
		printf("Using 6 as default number of students\n");
		numberofstudents = 6;
	}
	else
	{
		printf("Creating %d threads.\n", numberofstudents);
		numberofstudents = atoi(argv[1]);
	}
		
	
	student = (pthread_t*) malloc(sizeof(pthread_t)*numberofstudents);

	
	pthread_create(&TA, NULL, taactivity, NULL);	
	for(id = 0; id < numberofstudents; id++)
		pthread_create(&student[id], NULL, Student_Activity,(void*) (long)id);

	
	pthread_join(TA, NULL);
	for(id = 0; id < numberofstudents; id++)
		pthread_join(student[id], NULL);

	
	free(student); 
	return 0;
}

void *taactivity()
{
	while(1)
	{
		sem_wait(&teachingassistentsleeping);		
		printf("Student woke up the teaching assistent\n");

		while(1)
		{
			
			pthread_mutex_lock(&accesschair);
			if(ChairsCount == 0) 
			{
				
				pthread_mutex_unlock(&accesschair);
				break;
			}
			
			sem_post(&ChairsSem[CurrentIndex]);
			ChairsCount--;
			printf("Student the chair.Chairs left: %d\n", 3 - ChairsCount);
			CurrentIndex = (CurrentIndex + 1) % 3;
			pthread_mutex_unlock(&accesschair);
			

			printf("\t Teaching assistant is helping the student.\n");
			sleep(6);
			sem_post(&Student_Sem);
			usleep(1000);
		}
	}
}

void *Student_Activity(void *threadID) 
{
	int ProgrammingTime;

	while(1)
	{
		printf("Student %ld is doing programming assignment.\n", (long)threadID);
		ProgrammingTime = rand() % 10 + 1;
		sleep(ProgrammingTime);		

		printf("Student %ld needs help \n", (long)threadID);
		
		pthread_mutex_lock(&accesschair);
		int count = ChairsCount;
		pthread_mutex_unlock(&accesschair);

		if(count < 3)		
		{
			if(count == 0)		
				sem_post(&teachingassistentsleeping);
			else
				printf("Student %ld sat on a chair waiting for Teaching Assistant \n", (long)threadID);

			
			pthread_mutex_lock(&accesschair);
			int index = (CurrentIndex + ChairsCount) % 3;
			ChairsCount++;
			printf("Chairs Remaining: %d\n", 3 - ChairsCount);
			pthread_mutex_unlock(&accesschair);
			

			sem_wait(&ChairsSem[index]);		
			printf("\t teaching Assistant is helping student: %ld  \n", (long)threadID);
			sem_wait(&Student_Sem);		
			printf("Student %ld left the room.\n",(long)threadID);
		}
		else 
			printf("Student %ld will come at another time. \n", (long)threadID);
			
	}
}

#include <pthread.h>		
#include <time.h>			
#include <unistd.h>			
#include <semaphore.h>		
#include <stdlib.h>			
#include <stdio.h>			

pthread_t *student;		
pthread_t TA;				

int chairCount = 0;
int cindex = 0;


sem_t teachingassistentsleeping;
sem_t Student_Semaphores;
sem_t ChairsSemaphores[3];
pthread_mutex_t accesschair;


void *taactivity();
void *Student_Activity(void *threadid);

int main(int a, char* b[])
{
	int numberofstudents;		
	int id;
	srand(time(NULL));

	
	sem_init(&teachingassistentsleeping, 0, 0);
	sem_init(&Student_Semaphores, 0, 0);
	for(id = 0; id < 3; ++id)			
		sem_init(&ChairsSemaphores[id], 0, 0);

	pthread_mutex_init(&accesschair, NULL);
	
	if(a<2)
	{
		printf("Using 6 as default number of students\n");
		numberofstudents = 6;
	}
	else
	{
		printf("Creating %d threads.\n", numberofstudents);
		numberofstudents = atoi(b[1]);
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
			if(chairCount == 0) 
			{
				
				pthread_mutex_unlock(&accesschair);
				break;
			}
			
			sem_post(&ChairsSemaphores[cindex]);
			chairCount--;
			printf("Student the chair.Chairs left: %d\n", 3 - chairCount);
			cindex = (cindex + 1) % 3;
			pthread_mutex_unlock(&accesschair);
			

			printf("\t Teaching assistant is helping the student.\n");
			sleep(6);
			sem_post(&Student_Semaphores);
			usleep(1000);
		}
	}
}

void *Student_Activity(void *threadid) 
{
	int ProgrammingTime;

	while(1)
	{
		printf("Student %ld is doing programming assignment.\n", (long)threadid);
		ProgrammingTime = rand() % 10 + 1;
		sleep(ProgrammingTime);		

		printf("Student %ld needs help \n", (long)threadid);
		
		pthread_mutex_lock(&accesschair);
		int count = chairCount;
		pthread_mutex_unlock(&accesschair);

		if(count < 3)		
		{
			if(count == 0)		
				sem_post(&teachingassistentsleeping);
			else
				printf("Student %ld sat on a chair waiting for Teaching Assistant \n", (long)threadid);

			
			pthread_mutex_lock(&accesschair);
			int index = (cindex + chairCount) % 3;
			chairCount++;
			printf("Chairs Remaining: %d\n", 3 - chairCount);
			pthread_mutex_unlock(&accesschair);
			

			sem_wait(&ChairsSemaphores[index]);		
			printf("\t teaching Assistant is helping student: %ld  \n", (long)threadid);
			sem_wait(&Student_Semaphores);		
			printf("Student %ld left the room.\n",(long)threadid);
		}
		else 
			printf("Student %ld will come at another time. \n", (long)threadid);
			
	}
}

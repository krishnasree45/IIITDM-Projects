/*
Lazy Dentist Problem: There is a dental clinic with N chairs for waiting patients; the clinic has one
doctor. If a patient enters the clinic and there are no free chairs, the patient leaves. If a patient enters
the clinic and the dentist is sleeping, the patient wakes up the dentist and consults him. Otherwise, the
patient enters the clinic, takes a seat, and waits. If the dentist finishes with one patient and there are
waiting patients, the dentist takes the next patient. Otherwise, the dentist goes to sleep in his chair.

Graphical output: top rectangle for dentist chair and bot 3 rectangles to represent waiting room
command to compile:gcc lazy_dentist.c -lpthread -lgraph
then ./a.out
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <pthread.h> // pthread_t, pthread_create, pthread_join
#include <semaphore.h> // sem_init, sem_wait, sem_post
#include <time.h>
#include <graphics.h>

#define NUM_SEAT 3
#define SLEEP_MAX 5

sem_t sem_patient;//number of patients
sem_t sem_dentist;//for dentist 

pthread_mutex_t mutex;

int gd=DETECT,gm;      // to detect current available graph mode 
int x,y;

int chair[3];
int count = 0; //number of waiting patients
int next_seat = 0;
int next_treat = 0;
int remain=0;
int counter=0;//for colours
int counter1=0;

void rand_sleep(void);
void* patient_waiting(void* patient_id);
void* dentist_treating();
void  printing();

void initialize()//creating graph
{
	sem_init(&sem_patient,0,0);//initialising semaphores
	sem_init(&sem_dentist,0,1);
	printf("initializing graph\n");
	initgraph(&gd,&gm,NULL);
	x=getmaxx()/2;
	y=getmaxy()/2;
	printf("created graph\n");

	setcolor(GREEN);
	rectangle(x+165,y-100,x+265,y-200);
	floodfill(x+215,y-150,LIGHTBLUE);

	setcolor(WHITE);
	line(getmaxx()/2,0,getmaxx()/2,getmaxy());

	setcolor(RED);
	rectangle(x+10,y+50,x+110,y-50);
	floodfill(x+60,y,LIGHTBLUE);
	setcolor(RED);
	rectangle(x+120,y+50,x+220,y-50);
	floodfill(x+170,y,LIGHTBLUE);
	setcolor(RED);
	rectangle(x+230,y+50,x+330,y-50);
	floodfill(x+280,y,LIGHTBLUE);

	outtextxy(x+175,y-150,"SLEEPING");
	outtextxy(x+40,y,"ZERO");
	outtextxy(x+150,y,"ZERO");
	outtextxy(x+260,y,"ZERO");

}

int main(int argc, char **argv)
{
	srand(time(NULL));
	initialize();	
	pthread_t *patients;
	pthread_t dentist;
	int* patient_ids;
	int patient_num;	
	int i;
	patient_num=5;
	remain=patient_num;
	patients = (pthread_t*)malloc(sizeof(pthread_t) * patient_num);
	patient_ids = (int*)malloc(sizeof(int) * patient_num);
	memset(patient_ids, 0, patient_num);
	
	pthread_create(&dentist,NULL,dentist_treating,NULL);//dentist thread
	
	for(i=0; i<patient_num; i++)
	{
		patient_ids[i] = i+1;
		pthread_create(&patients[i], NULL, patient_waiting, (void*) &patient_ids[i]);//patients threads
	} 
	pthread_join(dentist, NULL);//join thread
	for(i=0; i<patient_num;i++)
	{
		pthread_join(patients[i],NULL);
	}	
	delay(5000 );
	closegraph();
	return 0;
}

void* dentist_treating()
{
	while(remain!=0)
	{
		counter++;//to fill colours
		sem_wait(&sem_patient);
		pthread_mutex_lock(&mutex);
		printf("[dentist] dentist is treating patient %d\n",chair[next_treat]);

		if(counter%2==0)//just for changing colours to avoid overwriting
		{
		 setcolor(GREEN);
		 rectangle(x+165,y-100,x+265,y-200);
		 floodfill(x+215,y-150,LIGHTBLUE);
		}
		else
		{
		  setcolor(BLUE);
		  rectangle(x+165,y-100,x+265,y-200);
		  floodfill(x+215,y-150,LIGHTBLUE);
		}

		if(chair[next_treat]==0)
		outtextxy(x+175,y-150,"SLEEPING");
		else if(chair[next_treat]==1)
		outtextxy(x+200,y-150,"ONE");
		else if(chair[next_treat]==2)
		outtextxy(x+200,y-150,"TWO");
		else if(chair[next_treat]==3)
		outtextxy(x+200,y-150,"THREE");
		else if(chair[next_treat]==4)
		outtextxy(x+200,y-150,"FOUR");
		else if(chair[next_treat]==5)
		outtextxy(x+200,y-150,"FIVE");
		
		chair[next_treat]=0;
		count--;
		printf("waiting patients : [1] %d [2] %d [3] %d\n",chair[0],chair[1],chair[2]);//status of waiting room
		printing();
		next_treat = (next_treat + 1) % NUM_SEAT;
		rand_sleep();
		printf("[dentist] treating finish.\n");
		remain=remain-1;
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_dentist);
	}
	printf("all the patient over... dentist leaving home\n");
	pthread_exit(0);	
}

void* patient_waiting(void* patient_id)
{
	int id = *(int*)patient_id;
	{
		rand_sleep();
		pthread_mutex_lock(&mutex);
		printf("[patient] patient %d has entered \n",id);
		if(count < NUM_SEAT)	
		{
			chair[next_seat] = id;
			count++;
			printf("	[patient] patient %d is waiting\n",id);
			printf("waiting patients : [1] %d [2] %d [3] %d\n",chair[0],chair[1],chair[2]);//status of waiting room
			printing();
			next_seat = (next_seat+1) % NUM_SEAT;
			pthread_mutex_unlock(&mutex);
			//wakeup dentist
			sem_post(&sem_patient);
			sem_wait(&sem_dentist);
			
		}
		else //no more chairs
		{
			printf("[patient] no more chairs.patient %d is leaving\n",id);
			remain=remain-1;
			pthread_mutex_unlock(&mutex);
				
		        pthread_exit(0);
		}
	}	
}

void  printing()// fro printing in the rectangular boxes
{
	counter1++;
	if(counter1%2==0)
	{
	 setcolor(RED);
	rectangle(x+10,y+50,x+110,y-50);
	floodfill(x+60,y,RED);
	setcolor(RED);
	rectangle(x+120,y+50,x+220,y-50);
	floodfill(x+170,y,RED);
	setcolor(RED);
	rectangle(x+230,y+50,x+330,y-50);
	floodfill(x+280,y,RED);
	}
	else
	{
	 setcolor(CYAN);
	rectangle(x+10,y+50,x+110,y-50);
	floodfill(x+60,y,CYAN);
	setcolor(CYAN);
	rectangle(x+120,y+50,x+220,y-50);
	floodfill(x+170,y,CYAN);
	setcolor(CYAN);
	rectangle(x+230,y+50,x+330,y-50);
	floodfill(x+280,y,CYAN);

	}
			if(chair[0]==0)	
			outtextxy(x+40,y,"ZERO");
			else if(chair[0]==1)	
			outtextxy(x+40,y,"ONE");
			else if(chair[0]==2)	
			outtextxy(x+40,y,"TWO");
			else if(chair[0]==3)	
			outtextxy(x+40,y,"THREE");
			else if(chair[0]==4)	
			outtextxy(x+40,y,"FOUR");
			else if(chair[0]==5)	
			outtextxy(x+40,y,"FIVE");

			if(chair[1]==0)	
			outtextxy(x+150,y,"ZERO");
			else if(chair[1]==1)	
			outtextxy(x+150,y,"ONE");
			else if(chair[1]==2)	
			outtextxy(x+150,y,"TWO");
			else if(chair[1]==3)	
			outtextxy(x+150,y,"THREE");
			else if(chair[1]==4)	
			outtextxy(x+150,y,"FOUR");
			else if(chair[1]==5)	
			outtextxy(x+150,y,"FIVE");
			
			if(chair[2]==0)	
			outtextxy(x+260,y,"ZERO");
			else if(chair[2]==1)	
			outtextxy(x+260,y,"ONE");
			else if(chair[2]==2)	
			outtextxy(x+260,y,"TWO");
			else if(chair[2]==3)	
			outtextxy(x+260,y,"THREE");
			else if(chair[2]==4)	
			outtextxy(x+260,y,"FOUR");
			else if(chair[2]==5)	
			outtextxy(x+260,y,"FIVE");

			return;
}

void rand_sleep(void)
{
	int time = rand() % SLEEP_MAX + 1;
	sleep(time);
}


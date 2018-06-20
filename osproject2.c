#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include<string.h>
#include<conio.h>

sem_t mutex, question, result;//declaration of semaphores

typedef struct
{ 
char question[100];
char *options[4];
int displayedSequence[4];
int correctSequence[4];
int answerBoard[5];
} FFFGame; 

FFFGame myGame;   //instance of a strructure
int answers[10][5]; //for storing the answers 
int answersI=0;

void *myThreadFun1(void *arg){
	sleep(1+rand()%4);
	sem_wait(&question);


	int tid = *(int *) arg;
	printf("\nPlayer %d", tid);
	myGame.answerBoard[0] =tid;
	myGame.answerBoard[1] =4;//rand()%4 + 1;
	myGame.answerBoard[2] =2;//rand()%4 + 1;
	myGame.answerBoard[3] =3;//rand()%4 + 1;
	myGame.answerBoard[4] =1;//rand()%4 + 1;
   	
    sem_post(&result);
}

void *AB(void *arg)
{
	strcpy(myGame.question, "Arrange the states of Indian Subcontinent is order from North to South.");
	myGame.options[0] = malloc(100);
	myGame.options[1] = malloc(100);
	myGame.options[2] = malloc(100);
	myGame.options[3] = malloc(100);
	strcpy(myGame.options[0], "1.Kerela");
	strcpy(myGame.options[1], "2.Madhya Pradesh");
	strcpy(myGame.options[2], "3.Andhra Pradesh");
	strcpy(myGame.options[3], "4.Jammu&Kashmir");
	printf("WELCOME TO KBC!!\n\n");
	sleep(1);
	printf("\nWELCOME AMITABH BACHCHAN\n\n");
	sleep(5);
	printf("\nFastest finger first ka prashn,Yeh raha aapki screen par!\n\n");
	sleep(5);
	puts(myGame.question);
	printf("\n");
	printf(myGame.options[0]);
	printf("\n");	
	printf(myGame.options[1]);
	printf("\n");
	printf(myGame.options[2]);
	printf("\n");
	printf(myGame.options[3]);
	printf("\n");
	sleep(7);
		
    sem_post(&question);
    return NULL;
}

void *resultant(void *arg)
{
	while(1){
	sem_wait(&result);
	printf("\nchecking answer");
	
    int i;
    for(i=0; i<5; i++)
    	answers[answersI][i] = myGame.answerBoard[i];
    answersI++;
    
    if(answersI==10){
    	printf("\nAll threads finished");
    	int i, j, found;
    	for(i=0; i<answersI; i++){
			found = 1;  
			for(j=0; j<4; j++){
				if(myGame.correctSequence[j] != answers[i][j+1]){
				
					found = 0;
					break;
				}
			}
			
			if(found){
				printf("\nWinner is %d\n\n", answers[i][0]);
				return;
			}
		}
		
		if(!found) printf("\nNo Winner");
	
	}
    sem_post(&question);
    }
}

void main()
{	
	sem_init(&question, 0, 0);
	sem_init(&mutex, 0, 0);
	sem_init(&result, 0, 0);
	int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; //10 different players
	
    pthread_t tid[10], tidAB, tidRes;//initialising 12 threads
    printf("Before Thread\n");

	myGame.correctSequence[0] =4;
	myGame.correctSequence[1] =2;	
	myGame.correctSequence[2] =3;
	myGame.correctSequence[3] =1;	

	pthread_create(&tidAB, NULL, AB, NULL);
    int i;
    for(i=0; i<10; i++)
    	pthread_create(&tid[i], NULL, myThreadFun1, &a[i]);
    
	pthread_create(&tidRes, NULL, resultant, NULL);	

    for(i=0; i<10; i++)
    	pthread_join(tid[i], NULL);
    pthread_join(tidAB, NULL);
    pthread_join(tidRes, NULL);
    printf("After Thread\n");
    exit(0);
}

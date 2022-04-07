#include<stdio.h>
#include<stdlib.h> 
#include<time.h> 
#include<ctype.h> 
#include<stdbool.h> 

#define COUNT_MAX 3

#define DELAY 2

void gameInstructions(void);
void startGamePlay(void);
void printSequence(unsigned int seed, int sequenceLength);
bool inputSeqCheck(unsigned int seed, int sequenceLength);
void delayNErase(int sequenceLength);
void printScore(int counter,time_t gameStart);


int main(int argc, char * argv[]){
	
	gameInstructions();
	startGamePlay();
	return 0;
}

void gameInstructions(void){
	printf(" print instructions \n\n\n");
	
}

void startGamePlay(void)
{
	char anotherGame = 'Y'; 
	bool gameWon = true; 
	int sequenceLength = 2;
	int counter = 0;
	
	
	do{
		
		time_t gameStart = clock(); //total time taken to play the game
		
		while(gameWon)
		{
			unsigned int seed = time(NULL);
			
			
			/* increase the sequence length if counter has reached COUNT_MAX:
			 * check the way the increment is done with out using if construct 
			 *also note that counter is incremented in postfix */
			sequenceLength += ((counter++ % COUNT_MAX) == 0);
			
			/*print sequence to screen*/
			printSequence(seed, sequenceLength);
			
			/*Erase digits on the screen*/
			delayNErase(sequenceLength);
			
			if (counter==1)
				printf("\nNow enter the sequence including the spaces \n");
			else
				printf("\r");
			
			/*take user input and check if correct return true else false*/
			gameWon=inputSeqCheck(seed, sequenceLength);
			
			
			
			printf("%s\n",gameWon?"Correct !":"You lost the Game");
			
		}
		printScore(counter, gameStart);
		
		printf("\nWanna play again (y/n)? ");	
		scanf("%c",&anotherGame);
		
	}while(toupper(anotherGame)=='Y');
	
}

void printSequence(unsigned int seed, int sequenceLength)
{
	/* Print the digits to the screen:
	 * We use this method instead of limiting the rand range because when
	 * you limit a rand range its output can be any number from 0-limit but
	 * we need the output to be of particular sequence length always + we 
	 * need seperate digits to make a sequence not a single number */
	
	srand(seed);
	int i=0;
	for  (i=0; i<= sequenceLength; i++){
		printf("%d ",rand()%8);
	}
	
	/*clear buffers to actually print characters u erase them*/
	fflush(stdout);
	
}

bool inputSeqCheck(unsigned int seed, int sequenceLength)
{
	/*1. Take input sequence from the user:
	 * get the previous random sequence using same seed
	 * verify the u input against the random sequence and if
	 * right continue else exit the loop setting gameLost*/
	
	srand(seed);
	int i=0;
	for (i=0; i<=sequenceLength; i++){
		int number=0;
		scanf("%d",&number);
		if (number != rand()%8){
			return false;
		}
	}
	return true;
}

void delayNErase(int sequenceLength)
{
	/* 1. Provides a delay of DELAY seconds and erases the 
	 * line which gives the effect of hiding the input.
	 * 2. Uses CLOCKS_PER_SEC and clock() to check the number
	 * of seconds elapsed. */
	
	/*time_t is a type used by time.h functions such as clock and time*/
	time_t now = clock();
	for( ;clock() - now < DELAY*CLOCKS_PER_SEC; );
	
	/*Erase the current line*/
	printf("\r");
	int i=0;
	for(i=0; i <sequenceLength; i++){
		printf("    ");
	}
}

void printScore(int counter,time_t gameStart)
{
	int timeTaken = (clock() - gameStart)/CLOCKS_PER_SEC;
	printf("Your score: %d \n", (--counter*100)/ timeTaken);
}
/*
**	MONTECARLO.c
**	Put under the public domain by Andrew Zeng
**
**	This program is a simple AI that uses
**	Monte-Carlo tree search methods to play 
**	a simplified version of chess. 
**	The game is played on a 4x8 board, with 
**	each side having 8 pawns. The goal is to
**	move a pawn to the other side.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "game.c"

//the game board
char* MAIN_BOARD;

void game()
{
	//get user move:
	int move_piece, dest;
	char** possible_next = NULL;
	struct position** candidate_positions;
	unsigned ri;
	struct position* tp = NULL;
	unsigned tNVM = 0;
	for(;;)
	{
		display(MAIN_BOARD);
		printf("Enter the source and the destination, please\n");
		
		scanf("%d %d",&move_piece,&dest);

		*(MAIN_BOARD+move_piece) = ' ';
		*(MAIN_BOARD+dest) = 'A';
		
		switch(endgame(MAIN_BOARD))
		{
			case 'c':
			break;
			case 'd':
			display(MAIN_BOARD);
			printf("Game is drawn.");
			getch();
			exit(0);
			break;
			case 'a':
			display(MAIN_BOARD);
			printf("Machine victorious.");
			getch();
			exit(0);
			break;
			case 'A':
			display(MAIN_BOARD);
			printf("Humanity victorious.");
			getch();
			exit(0);
			break;
		}
		
		possible_next = get_valid_moves(MAIN_BOARD,'a');
		candidate_positions = malloc(sizeof(struct position*)*NUM_VALID_MOVES);
		//evaluate each of the possible positions
		tNVM = NUM_VALID_MOVES;
	//	printf("there are %d valid moves up here\n",tNVM);	
		for(int i = 0; i<tNVM; ++i)
		{
		//	printf("i = %d\n",i);
			tp = update_search(*(possible_next+i));
			*(candidate_positions+i) = tp;
			for(int x = 0; x<thinktime; ++x)
			{
				evaluate_position(*(candidate_positions+i));
			}
		}
		float best = -10.0f;
		
	/*	printf("there are %d valid moves\n",tNVM);
		for(int i = 0; i<tNVM; ++i)
		{
			printf("%d %d\n",i,(*(candidate_positions+i))->total);
		}
		printf("didnt  crash\n");*/
		for(int i = 0; i<tNVM; ++i)
		{
		/*	printf("AAA %d",(*(candidate_positions+i)) == NULL);
			printf("WINS: %d LOSSES: %d TOTAL: %d\n",(*(candidate_positions+i))->wins,(*(candidate_positions+i))->losses,(*(candidate_positions+i))->total);
			printf("BBB");*/
			if((*(candidate_positions+i))->wins/(float)(*(candidate_positions+i))->total-
			(*(candidate_positions+i))->losses/(float)(*(candidate_positions+i))->total
			 > best)
			{
				
				best = (*(candidate_positions+i))->wins/(float)(*(candidate_positions+i))->total-
			(*(candidate_positions+i))->losses/(float)(*(candidate_positions+i))->total;
			//	printf("best is now %f\n",best);
				ri = i;
			}
		}
		printf("BEST IS %f\n",best);
		free(candidate_positions);
		printf("Processing done.");		
		
		memcpy(MAIN_BOARD,*(possible_next+ri),32);
		
		
		for(int i = 0; i<tNVM; ++i)
			free(*(possible_next+i));
		free(possible_next);
		
		switch(endgame(MAIN_BOARD))
		{
			case 'c':
			break;
			case 'd':
			display(MAIN_BOARD);
			printf("Game is drawn.");
			getch();
			exit(0);
			break;
			case 'a':
			display(MAIN_BOARD);
			printf("Machine victorious.");
			getch();
			exit(0);
			break;
			case 'A':
			display(MAIN_BOARD);
			printf("Humanity victorious.");
			getch();
			exit(0);
			break;
		}
		
	}
	
}




char* starting = "aaaaaaaa                AAAAAAAA";

int main()
{
	
	FILE *f = fopen("S.txt","r");
	fscanf(f,"preptime: %d\nthinktime: %d",&preptime,&thinktime);
	fclose(f);
	printf("Loaded: preptime: %d, thinktime: %d\n",preptime,thinktime);
	srand(time(0));
	int time0 = rand()%1000000;
	printf("Seed: %d\n",time0);
	srand(1);
	MAIN_BOARD = malloc(32);
	for(int i = 0; i<32; ++i)
	{
		*(MAIN_BOARD+i) = *(starting+i);
	}

	struct position* m;
	for(int i = 0; i<preptime; ++i)
	{
		m = new_position("aaaaaaaa                AAAAAAAA");
		evaluate_position(m);
		free(m);
	}
	save_data();
	
	//play the game
	game();
	
	


}

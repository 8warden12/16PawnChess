#ifndef GAME_C
#define GAME_C
#include "board.c"
#include "conio.h"
unsigned NUM_VALID_MOVES = 0;
char** get_valid_moves(const char* p, char POV)
{
	char** valid_moves = (char**)malloc(sizeof(char*)*50);
	for(int i = 0; i<50; ++i)
	{
		*(valid_moves+i) = NULL;
	}
	unsigned push_counter = 0;
	
	if(POV == 'a')
	{
		for(int i = 0; i<32; ++i)
		{
			if(*(p+i) == 'a')
			{
				//check if square ahead of it is free
				if(i < 28 && *(p+i+4) == ' ')
				{
					*(valid_moves+push_counter) = malloc(sizeof(char)*32);
					memcpy(*(valid_moves+push_counter),p,sizeof(char)*32);
					//new position with the piece one square ahead
					*(*(valid_moves+push_counter)+i+4) = 'a';
					*(*(valid_moves+push_counter)+i) = ' ';
					++push_counter;
				}
				//capture of left side
				if(i < 29 && *(p+i+3) == 'A' && i % 4 != 0 /*lol*/)
				{
					*(valid_moves+push_counter) = malloc(sizeof(char)*32);
					memcpy(*(valid_moves+push_counter),p,sizeof(char)*32);
					//new position with 'P' captured
					*(*(valid_moves+push_counter)+i+3) = 'a';
					*(*(valid_moves+push_counter)+i) = ' ';
					++push_counter;
				}
				//capture of right side
				if(i < 27 && *(p+i+5) == 'A' && (i+1) % 4 != 0)
				{
					*(valid_moves+push_counter) = malloc(sizeof(char)*32);
					memcpy(*(valid_moves+push_counter),p,sizeof(char)*32);
					//new position with 'P' captured
					*(*(valid_moves+push_counter)+i+5) = 'a';
					*(*(valid_moves+push_counter)+i) = ' ';
					++push_counter;
				}
			}
		}
	}else if(POV == 'A')
	{
		for(int i = 0; i<32; ++i)
		{
			if(*(p+i) == 'A')
			{
				//check if square ahead of it is free
				if(i > 3 && *(p+i-4) == ' ')
				{
					*(valid_moves+push_counter) = malloc(sizeof(char)*32);
					memcpy(*(valid_moves+push_counter),p,sizeof(char)*32);
					//new position with the piece one square ahead
					*(*(valid_moves+push_counter)+i-4) = 'A';
					*(*(valid_moves+push_counter)+i) = ' ';
					++push_counter;
				}
				//capture of left side
				if(i > 4 && *(p+i-5) == 'a' && i % 4 != 0)
				{
					*(valid_moves+push_counter) = malloc(sizeof(char)*32);
					memcpy(*(valid_moves+push_counter),p,sizeof(char)*32);
					//new position with 'P' captured
					*(*(valid_moves+push_counter)+i-5) = 'A';
					*(*(valid_moves+push_counter)+i) = ' ';
					++push_counter;
				}
				//capture of right side
				if(i > 3 && *(p+i-3) == 'a' && (i+1) % 4 != 0)
				{
					*(valid_moves+push_counter) = malloc(sizeof(char)*32);
					memcpy(*(valid_moves+push_counter),p,sizeof(char)*32);
					//new position with 'P' captured
					*(*(valid_moves+push_counter)+i-3) = 'A';
					*(*(valid_moves+push_counter)+i) = ' ';
					++push_counter;
				}
			}
		}
	}
	NUM_VALID_MOVES = push_counter;
	return valid_moves;
}


//returns 'c' if game is in session, 'd' if draw, 
char endgame(const char* board)
{
	for(int i = 0; i<4; ++i)
	{
		if(*(board+i) == 'A')
		return 'A';
		if(*(board+31-i) == 'a')
		return 'a';
	}	
	get_valid_moves(board,'a');
	if(NUM_VALID_MOVES == 0)
	return 'd';
	get_valid_moves(board,'A');
	if(NUM_VALID_MOVES == 0)
	return 'd';
	
	return 'c';
}



//it is 'p''s turn to play
//randomly choose moves for a position until game ends;
void evaluate_position(struct position* p)
{
	char* current = p->board;
	char** branches = malloc(sizeof(char*)*200);
	unsigned current_branch = 0;
	*branches = current;
	char egs;
	char** possible_next = NULL;
	
	for(;;)
	{
		
		//get random move for p
		possible_next = get_valid_moves(current,'a');
		current = *(possible_next + rand()%NUM_VALID_MOVES);
		for(int i = 0; i<NUM_VALID_MOVES; ++i)
		{ //free memory allocated by get_valid moves
			if(*(possible_next + i) != current)
			free(*(possible_next + i));
		}
		free(possible_next);
		*(branches + ++current_branch) = current;
		egs = endgame(current);
		switch(egs)
		{
			case 'c':
			break;
			case 'd'://draw
				goto endgame_draw;
			case 'a':
				goto endgame_p_victory;
			case 'A':
				goto endgame_P_victory;
		}
		
		
		//get random move for P
		possible_next = get_valid_moves(current,'A');
		current = *(possible_next + rand()%NUM_VALID_MOVES);
		
		for(int i = 0; i<NUM_VALID_MOVES; ++i)
		{
			if(*(possible_next + i) != current)
			free(*(possible_next + i));
		}
		
		free(possible_next);
		egs = endgame(current);

		switch(egs)
		{
			case 'c':
			break;
			case 'd'://draw
				goto endgame_draw;
			case 'a':
				goto endgame_p_victory;
			case 'A':
				goto endgame_P_victory;
		}
		
	}
	
	
	
	struct position* tp;
		
	endgame_draw:;
	//display(current);
		for(int i = 0; i<=current_branch; ++i)
		{
			tp = update_search(*(branches+i));
			++tp->draws;
			++tp->total;
		}
	return;
	
	
	endgame_p_victory:
		//display(current);
		for(int i = 0; i<=current_branch; ++i)
		{
			tp = update_search(*(branches+i));
			++tp->wins;
			++tp->total;
		}
	return;
	
	
	endgame_P_victory:
	//	display(current);
		for(int i = 0; i<=current_branch; ++i)
		{
			tp = update_search(*(branches+i));
			++tp->losses;
			++tp->total;
		}
	return;	
}

#endif

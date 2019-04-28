#ifndef BOARD_C
#define BOARD_C
#include "charop.c"
struct position{
	char* board;
	
	unsigned wins;
	unsigned losses;
	unsigned draws;
	unsigned total;
	
	/*struct position* left;
	struct position* right;
	struct position* parent;*/
	unsigned hash;
} *head;

struct position* new_position(const char* board)
{
	struct position* p = malloc(sizeof(struct position));
	p->board = malloc(32); //32 bytes
	memcpy(p->board,board,32);
	p->wins = p->losses = p->draws = p->total = 0;
	p->hash = hash(board);
	
	return p;
}


struct hash_node{
	struct position** boards;
	unsigned num_boards;
	unsigned hash;
} **seen_positions;

int num_seen_positions = 0;

void save_data()
{
	FILE* f = fopen("H.txt","w");
	for(int i = 0; i<num_seen_positions; ++i)
	{
		for(int x = 0; x<(*(seen_positions+i))->num_boards; ++x)
		{
			fprintf(f,"%s ",(*((*(seen_positions+i))->boards+x))->board);
			fprintf(f,"%d ",(*((*(seen_positions+i))->boards+x))->wins);
			fprintf(f,"%d ",(*((*(seen_positions+i))->boards+x))->losses);
			fprintf(f,"%d ",(*((*(seen_positions+i))->boards+x))->draws);
			fprintf(f,"%d\n",(*((*(seen_positions+i))->boards+x))->total);
		}
	}
	
	fclose(f);
}

struct hash_node* hash_node_search(const char* board)
{
	unsigned hashv = hash(board);
	for(int i = 0; i<num_seen_positions; ++i)
	{
		if((*(seen_positions+i))->hash == hashv)
		return *(seen_positions+i);
	}
	return NULL;
}

struct position* linear_search(const char* board, const struct hash_node* node)
{
	for(int i = 0; i<node->num_boards; ++i)
	{
		if(equals((*(node->boards+i))->board, board))
		return *(node->boards+i);
	}
	return NULL;
	/*
	unsigned hashv = hash(board);
	for(int i = 0; i<num_seen_positions; ++i)
	{
		if((*(seen_positions+i))->hash == hashv)
		{
			for(int x = 0; x<(*(seen_positions+i))->num_boards; ++x)
			{
				if(equals(board,(*((*(seen_positions+i))->boards+x))->board))
				{
					return (*((*(seen_positions+i))->boards+x));
				}
			}
			return NULL;
		}
	}*/
	
	return NULL;
}

//search for a position, crate it if it doesn't exist
struct position* update_search(const char* board)
{
	struct position* ts = NULL;
	struct hash_node* thn = NULL;
	unsigned hashv;
	
	//search for hash_node with same hash
	
	hashv = hash(board);
	
	
	for(int i = 0; i<num_seen_positions; ++i)
	{
		if((*(seen_positions+i))->hash == hashv)
		{
			thn = *(seen_positions+i);
			break;
		}
	}
	
	if(!thn)//not found: create new hash_node
	{
		//create and initialize hash_node
		struct hash_node* h = malloc(sizeof(struct hash_node));
		h->num_boards = 1;
		h->hash = hash(board);
			
		h->boards = malloc(sizeof(struct position*));
		*h->boards = new_position(board);
		seen_positions = realloc(seen_positions,(++num_seen_positions)*sizeof(struct hash_node*));
		*(seen_positions + num_seen_positions - 1) = h;
		
		return *h->boards;
		
	}else{//found a hash_node, but does the position reside there?
		//search for position within the hash_node
		for(int i = 0; i<thn->num_boards; ++i)
		{
			if(equals((*(thn->boards+i))->board, board))
			{
				ts = *(thn->boards+i);
				break;
			}
		}
		
		if(!ts) //position not found: must create new position
		{
			thn->boards = realloc(thn->boards,sizeof(struct position*) * (++thn->num_boards));
			*(thn->boards + thn->num_boards - 1) = new_position(board);
			return *(thn->boards + thn->num_boards - 1);
		}else{ //everything went smoothly: position found!
			return ts;//!
		}
	}
	
}

#endif

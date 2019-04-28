#ifndef CHAROP_C
#define CHAROP_C

unsigned MASTER_COUNTER = 0;
unsigned MC2 = 0;
unsigned tMASTER_COUNTER = 3;
unsigned tMC2 = 81;

int preptime = 10000;
int thinktime = 500;

bool e()
{
	if(MASTER_COUNTER == tMASTER_COUNTER && MC2 == tMC2)
	return true;
	
	return false;
}

void display(const char* c)
{
	for(int i = 0; i<6; ++i)
	{
		printf("#");
	}
	printf("\n");
	
	for(int i = 0; i<8; ++i)
	{
		printf("#");
		for(int x = 0; x<4; ++x)
		{
			printf("%c",*(c+ (4*i + x)));
		}
		printf("#\n");
	}
	
	for(int i = 0; i<6; ++i)
	{
		printf("#");
	}
	printf("\n\n");
}

bool equals(const char* a, const char* b)
{
	for(int i = 0; i<8; ++i)
		for(int x = 0; x<4; ++x)
			if(*(a+x+(4*i)) != *(b+x+(4*i)))
			return false;
	return true;
}

unsigned hash(const char* a)
{
	unsigned total = 0;
	for(int i = 1; i<=32; ++i)
	{
		switch(*(a+i-1))
		{
			case 'A':
				total += i;
			break;
			
			case 'a':
				total += (2*i);
			break;
		}
	}
	return total;
}

#endif

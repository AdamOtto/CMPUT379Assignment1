#include "../findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void createPattern(unsigned char *pattern, unsigned int patlength);

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

int main() {
	
	unsigned char *pattern = (char *) 'A';
	unsigned int patlength = 2;

	struct patmatch locations[10];
	unsigned int loclength = 10;
	
	
	printf("Test 1:\n");
	unsigned int number = findpattern(pattern, patlength, locations, loclength);
	
	//First test	
	printf("Pass 1\nTotal Matches: %d\n", number);
	int i;
		
	for (i = 0; i < loclength; i++)
	{
		if( i == number)
			break;
		printf("0x%x\t%c\n",(locations[i]).location, (locations[i]).mode);
					
	}
	

	createPattern(pattern, patlength);

	unsigned char *pattern2 = (char *) 'A';
	unsigned int patlength2 = 2;

	struct patmatch locations2[10];
	unsigned int loclength2 = 10;	
	
	//Second test with changes.
	printf("Pass 2\n");
	number = findpattern(pattern2, patlength2, locations2, loclength2);
	printf("Total Matches: %d\n", number);
	int oldFindPatternIndex = 0;

	for (i = 0; i < loclength2; i++)
	{
		if( i == number)
			break;
		
		printf("0x%x\t%c",(locations2[i]).location, (locations2[i]).mode);

		if( (locations2[i]).location == (locations[oldFindPatternIndex]).location )
		{
			if((locations2[i]).mode == (locations[oldFindPatternIndex]).mode)
				printf("\tU\n");
			else
				printf("\tC\n");
			oldFindPatternIndex++;
		}
		else
			printf("\tN\n");					
	}
}

/*
Creates a new pattern at the first read/write location found

Returns nothing.

pattern: char containing pattern character.
patlength: length of the pattern
*/
void createPattern(unsigned char *pattern, unsigned int patlength)
{
	char *currentAddress = 0x00000000;
	int pageSize = getpagesize();
	long pageTotal = (0xffffffff) / pageSize;
	long currentPage = 0;
	int MemoryReadWriteType;
	
	for(currentPage = 0; currentPage <= pageTotal; currentPage++)
	{
		MemoryReadWriteType = determineIfReadWriteAddressLocation(currentAddress);
			
		if(MemoryReadWriteType == 0 || MemoryReadWriteType == -1)
		{
			currentAddress += pageSize;
			continue;
		}
		else if(MemoryReadWriteType == 1)
		{
			int i;
			for(i = 0; i < patlength ; i++)
			{	
				//*currentAddress = *pattern;
				currentAddress++;
			}
			return;
		}
	}
}


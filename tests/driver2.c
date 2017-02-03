#include "../findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void createPattern2(unsigned char pattern, unsigned int patlength);

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

int main() {
	
	unsigned char *pattern = (char *) 'F';

	unsigned int patlength = 2;

	struct patmatch locations[10];
	unsigned int loclength = 10;
	
	FILE *f;
	f = fopen("Test_Results2", "w");
	fprintf(f,"\nTest 2:\n");
	unsigned int number = findpattern(pattern, patlength, locations, loclength);
	
	
	//First test	
	fprintf(f,"Pass 1\nTotal Matches: %d\n", number);
	
	printf("Test 1:\n");
	number = findpattern(pattern, patlength, locations, loclength);
	
	//First test	
	printf("Pass 1\nTotal Matches: %d\n", number);

	int i;
		
	for (i = 0; i < loclength; i++)
	{
		if( i == number)
			break;
		fprintf(f,"0x%x\t",(locations[i]).location);
		if((locations[i]).mode == '0')
			fprintf(f,"MEM_RO\n");
		else
			fprintf(f,"MEM_RW\n");

					
	}
	


	createPattern2('F', patlength);

	unsigned char *pattern2 = (char *) 'A';
	unsigned int patlength2 = 2;

	struct patmatch locations2[10];
	unsigned int loclength2 = 10;	
	
	//Second test with changes.

	fprintf(f,"Pass 2\n");
	number = findpattern(pattern2, patlength2, locations2, loclength2);
	fprintf(f,"Total Matches: %d\n", number);

	int oldFindPatternIndex = 0;

	for (i = 0; i < loclength2; i++)
	{
		if( i == number)
			break;
		

		fprintf(f,"0x%x\t",(locations2[i]).location);
		
		if((locations2[i]).mode == '0')
			fprintf(f,"MEM_RO\t");
		else
			fprintf(f,"MEM_RW\t");		
		
		if( (locations2[i]).location == (locations[oldFindPatternIndex]).location )
		{
			if((locations2[i]).mode == (locations[oldFindPatternIndex]).mode)

				fprintf(f,"U\n");
			else
				fprintf(f,"C\n");
			oldFindPatternIndex++;
		}
		else
			fprintf(f,"N\n");					
	}
	char ch;

	fclose(f);
}

/*
Creates a new pattern at the first read/write location found

Returns nothing.

pattern: char containing pattern character.
patlength: length of the pattern
*/
void createPattern2(unsigned char pattern, unsigned int patlength)
{
	char *currentAddress = 0x00000000;
	int pageSize = getpagesize();
	long pageTotal = (0xffffffff) / pageSize;
	long currentPage = 0;
	int MemoryReadWriteType;
	
	for(currentPage = 0; currentPage <= pageTotal; currentPage++)
	{
		int i;
		for (i = 0; i < pageSize; i++)
		{
			MemoryReadWriteType = determineIfReadWriteAddressLocation(currentAddress);
			
			if(MemoryReadWriteType == -1)
			{
				currentAddress += pageSize;
				break;
			}
			else if(MemoryReadWriteType == 0)
			{
				currentAddress++;
			}
			else if(MemoryReadWriteType == 1)
			{
				*currentAddress = pattern;
				currentAddress++;
			}
		}
	}
}


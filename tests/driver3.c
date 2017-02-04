#include "../findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

void createPattern3(unsigned char pattern, unsigned int patlength);

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

int main() {
	//Initialize variables for the first call to find pattern
	unsigned char *pattern = (char *) 'A';
	unsigned int patlength = 2;
	struct patmatch locations[10];
	unsigned int loclength = 10;	
	FILE *f;
	f = fopen("Test_Results3", "w");

	//First test
	fprintf(f,"\nTest 3:\n");
	unsigned int number = findpattern(pattern, patlength, locations, loclength);
	
	//Display the patterns found	
	fprintf(f,"Pass 1\nTotal Matches: %d\n", number);
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
	
	//Change the address space for the second test
	createPattern3('A', patlength);

	//Initialize variables for test 2
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
	fclose(f);
}

/*
Creates a new pattern at the first read/write location found.
Maps read address's to write.

Returns nothing.

pattern: char containing pattern character.
patlength: length of the pattern
*/
void createPattern3(unsigned char pattern, unsigned int patlength)
{
	//initialize Variable
	char *currentAddress = 0x00000000;
	int pageSize = getpagesize();
	long pageTotal = (0xffffffff) / pageSize;
	long currentPage = 0;
	int MemoryReadWriteType;
	int timesPatternCharWasInserted = 0;
	
	for(currentPage = 0; currentPage <= pageTotal; currentPage++)
	{
		int i;
		for (i = 0; i < pageSize; i++)
		{
			//Determine the mode of the address
			MemoryReadWriteType = determineIfReadWriteAddressLocation(currentAddress);
			
			//Memory isn't read-write
			if(MemoryReadWriteType == -1)
			{
				currentAddress += pageSize;
				break;
			}
			//Memory is read only
			else if(MemoryReadWriteType == 0)
			{
				//Map the address into memory
				void* mapped = mmap(NULL, getpagesize( ), PROT_WRITE, MAP_SHARED, (int)currentAddress, 0);
				
				currentAddress++;
			}
			//Memory is read-write
			else if(MemoryReadWriteType == 1)
			{
				*currentAddress = pattern;
				currentAddress++;
				timesPatternCharWasInserted++;
				if(timesPatternCharWasInserted >= patlength)
					return;
			}
		}
	}
}

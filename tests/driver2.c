#include "../findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void createPattern2(unsigned char pattern, unsigned int patlength);

int main() {
	//Initialize variables for the first call to find pattern
	unsigned char *pattern = (char *) 'F';
	unsigned int patlength = 2;
	struct patmatch locations[10];
	unsigned int loclength = 10;	
	FILE *f;
	f = fopen("Test_Results2", "w");
	
	//First test
	printf("\nTest 2:\n");
	unsigned int number = findpattern(pattern, patlength, locations, loclength);
			
	printf("Pass 1\n");
	
	//Display the patterns found
	printf("Total Matches: %d\n", number);

	int i;
		
	for (i = 0; i < loclength; i++)
	{
		if( i == number)
			break;
		printf("0x%x\t",(locations[i]).location);
		if((locations[i]).mode == '0')
			{printf("MEM_RO\n");}
		else
			{printf("MEM_RW\n");}

					
	}
	
	//Change the address space for the second test
	createPattern2('F', patlength * 2);

	//Initialize variables for test 2
	unsigned char *pattern2 = (char *) 'F';
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
		
		printf("0x%x\t",(locations2[i]).location);
		
		if((locations2[i]).mode == '0')
			printf("MEM_RO\t");
		else
			printf("MEM_RW\t");		
		
		if( (locations2[i]).location == (locations[oldFindPatternIndex]).location )
		{
			if((locations2[i]).mode == (locations[oldFindPatternIndex]).mode)

				printf("U\n");
			else
				printf("C\n");
			oldFindPatternIndex++;
		}
		else
			printf("N\n");					
	}
	char ch;

	fclose(f);
}

/*
Creates a new pattern in all read/write location found.

Returns nothing.

pattern: char containing pattern character.
patlength: length of the pattern
*/
static sigjmp_buf signalBuffer;

void createPattern2(unsigned char pattern, unsigned int patlength)
{
	//initialize Variable 
	char *currentAddress = 0x00000000;
	int pageSize = getpagesize();
	long pageTotal = (0xffffffff) / pageSize;
	long currentPage = 0;
	int MemoryReadWriteType;
	int SignalValue;	

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
				currentAddress++;
			}
			//Memory is read-write
			else if(MemoryReadWriteType == 1)
			{
				//Write the pattern characater to the address.
				//*currentAddress = pattern;
				currentAddress++;
			}
		}
	}
}

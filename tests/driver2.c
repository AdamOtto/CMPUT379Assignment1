#include "../findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

<<<<<<< HEAD
void createPattern2(unsigned char pattern, unsigned int patlength);
=======
void createPattern(unsigned char pattern, unsigned int patlength);
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

int main() {
	
<<<<<<< HEAD
	unsigned char *pattern = (char *) 'F';
=======
	unsigned char *pattern = (char *) 'A';
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
	unsigned int patlength = 2;

	struct patmatch locations[10];
	unsigned int loclength = 10;
	
<<<<<<< HEAD
	FILE *f;
	f = fopen("Test_Results2", "w");
	fprintf(f,"\nTest 2:\n");
	unsigned int number = findpattern(pattern, patlength, locations, loclength);
	
	
	//First test	
	fprintf(f,"Pass 1\nTotal Matches: %d\n", number);
=======
	
	printf("Test 1:\n");
	unsigned int number = findpattern(pattern, patlength, locations, loclength);
	
	//First test	
	printf("Pass 1\nTotal Matches: %d\n", number);
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
	int i;
		
	for (i = 0; i < loclength; i++)
	{
		if( i == number)
			break;
<<<<<<< HEAD
		fprintf(f,"0x%x\t",(locations[i]).location);
		if((locations[i]).mode == '0')
			fprintf(f,"MEM_RO\n");
		else
			fprintf(f,"MEM_RW\n");
=======
		printf("0x%x\t",(locations[i]).location);
		if((locations[i]).mode == '0')
			printf("MEM_RO\n");
		else
			printf("MEM_RW\n");
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
					
	}
	

<<<<<<< HEAD
	createPattern1('F', patlength);
=======
	createPattern('A', patlength);
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083

	unsigned char *pattern2 = (char *) 'A';
	unsigned int patlength2 = 2;

	struct patmatch locations2[10];
	unsigned int loclength2 = 10;	
	
	//Second test with changes.
<<<<<<< HEAD
	fprintf(f,"Pass 2\n");
	number = findpattern(pattern2, patlength2, locations2, loclength2);
	fprintf(f,"Total Matches: %d\n", number);
=======
	printf("Pass 2\n");
	number = findpattern(pattern2, patlength2, locations2, loclength2);
	printf("Total Matches: %d\n", number);
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
	int oldFindPatternIndex = 0;

	for (i = 0; i < loclength2; i++)
	{
		if( i == number)
			break;
		
<<<<<<< HEAD
		fprintf(f,"0x%x\t",(locations2[i]).location);
		
		if((locations2[i]).mode == '0')
			fprintf(f,"MEM_RO\t");
		else
			fprintf(f,"MEM_RW\t");		
=======
		printf("0x%x\t",(locations2[i]).location);
		
		if((locations2[i]).mode == '0')
			printf("MEM_RO\t");
		else
			printf("MEM_RW\t");		
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
		
		if( (locations2[i]).location == (locations[oldFindPatternIndex]).location )
		{
			if((locations2[i]).mode == (locations[oldFindPatternIndex]).mode)
<<<<<<< HEAD
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
=======
				printf("U\n");
			else
				printf("C\n");
			oldFindPatternIndex++;
		}
		else
			printf("N\n");					
	}
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
}

/*
Creates a new pattern at the first read/write location found

Returns nothing.

pattern: char containing pattern character.
patlength: length of the pattern
*/
<<<<<<< HEAD
void createPattern2(unsigned char pattern, unsigned int patlength)
=======
void createPattern(unsigned char pattern, unsigned int patlength)
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
{
	char *currentAddress = 0x00000000;
	int pageSize = getpagesize();
	long pageTotal = (0xffffffff) / pageSize;
	long currentPage = 0;
	int MemoryReadWriteType;
<<<<<<< HEAD
=======
	int timesPatternCharWasInserted = 0;
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
	
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
<<<<<<< HEAD
=======
				timesPatternCharWasInserted++;
				if(timesPatternCharWasInserted >= patlength)
					return;
>>>>>>> ce7c71f5c431b13a7f5b898c24d8b284902b0083
			}
		}
	}
}


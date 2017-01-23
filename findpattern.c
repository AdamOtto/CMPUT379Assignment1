#include "findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	
	/*For testing purposes -------*/
	unsigned char *testPattern = (char*)'f';
	unsigned int testPatLength = 1;
	struct patmatch *testLocations[10];
	unsigned int testLocLength = 10;
	/*----------------------------*/
	findpattern((char*)testPattern, testPatLength, (struct patmatch *)testLocations, testLocLength);
	return 0;
}


unsigned int findpattern(unsigned char *pattern, unsigned int patlength, struct patmatch *locations, unsigned int loclength)
{
	char *currentAddress = 0x00000000;
	int pageSize = getpagesize();
	int pageTotal = 0xffffffff / pageSize;
	int currentPage = 0;
	int patternCount = 0;
	char *locationOfWherePatternStarts = currentAddress;
	char data = ' ';
	
	for(currentPage = 0; currentPage < pageTotal; currentPage++)
	{
		printf("current address: 0x%x", (int)currentAddress);
		data = currentAddress++;
		printf(" current data: %c\n",data);

		if(data == &pattern)
		{
			printf("Pattern found...\n");
			patternCount++;
		}
		else
			patternCount = 0;
		
		
		if(patternCount == patlength)
		{
			printf("Pattern found at: 0x%x\n", (int)currentAddress );
		}

		currentAddress++;
	}
}

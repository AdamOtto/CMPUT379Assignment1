#include "findpattern.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
	
	/*For testing purposes -------*/
	unsigned char *testPattern = (char*)'f';
	unsigned int testPatLength = 8;
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
		/*This causes program to crash.  The program doesn't like reading things from
		pointers for some reason.		
		.*/
		/*
		data = *currentAddress;
		
		if(data == *pattern)
		{
			patternCount++;
		}
		else
			patternCount = 0;
		*/
		
		if(patternCount == patlength)
		{
			printf("Pattern found at: %x\n", (int)currentAddress );
		}

		currentAddress++;
	}
}

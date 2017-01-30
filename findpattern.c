#include "findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int determineIfReadWriteAddressLocation(char *);
void initiate_handler();
void restore_handler();
void handler(int);

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

int main(int argc, char *argv[]) {
	
	/*For testing purposes -------*/
	unsigned char *testPattern = (char*)'A';
	unsigned int testPatLength = 1;
	struct patmatch *testLocations[10];
	unsigned int testLocLength = 10;
	/*----------------------------*/

	int count = findpattern((char*)testPattern, testPatLength, (struct patmatch *)testLocations, testLocLength);
	return count;
}

/*
Scans the addresss space of the calling process and determines the
access permissions to all possible memory locations.

Returns the total number of locations where the pattern was found.

pattern: array of bytes
patlength: length of pattern array
locations: array of patmatch
loclength: length of locations array
*/
unsigned int findpattern(unsigned char *pattern, unsigned int patlength, struct patmatch *locations, unsigned int loclength)
{
	char *currentAddress = 0x00000000;
	char *patternFoundAddress = 0x00000000;
	int pageSize = getpagesize();
	long pageTotal = 0xffffffff / pageSize;
	long currentPage = 0;
	int patternCount = 0;
	char *locationOfWherePatternStarts = currentAddress;
	char data = ' ';

	char locationLengthCount = 0;
	int MemoryReadWriteType;
	int patternFoundCount = 0;	
	
	for(currentPage = 0; currentPage <= pageTotal; currentPage++)
	{
		MemoryReadWriteType = determineIfReadWriteAddressLocation(currentAddress);
		switch (MemoryReadWriteType)
		{
			//-1 means not read/write
			case -1:
				currentAddress += pageSize; 
				continue;
			//0 means read, not write
			case 0:
				data = *currentAddress;
				break;
			//1 means read and write
			case 1:
				data = *currentAddress;
				break;
		}

		//printf("Current Address: 0x%x ", (int)currentAddress);
		//printf("Current data: %c\n",data);
		currentAddress += pageSize;
		
		if(data == (int)pattern)
		{
			//printf("Pattern segment found...\n");
			if(patternCount == 0)
				patternFoundAddress = currentAddress;
			patternCount++;
		}
		else
		{
			//printf("No pattern found...\n");
			patternCount = 0;
		}
		
		
		if(patternCount == patlength)
		{
			printf("Pattern found at: 0x%x\n", (int)patternFoundAddress );
			if(locationLengthCount < loclength)
			{
				locations[locationLengthCount].location = (int)patternFoundAddress;
				if(MemoryReadWriteType == 0)
					locations[locationLengthCount].mode = (char)'0';
				else
					locations[locationLengthCount].mode = (char)'1';
				locationLengthCount++;
			}
			patternFoundCount++;
		}
	}
	return patternFoundCount;
}


int determineIfReadWriteAddressLocation(char * address)
{
	//printf("start initiate_handler.\n");
	//initiate_handler();
	char readWriter;
	
	newSignalHandler.sa_handler = handler;
	sigemptyset(&newSignalHandler.sa_mask);
	newSignalHandler.sa_flags = 0;
	if (sigaction(SIGSEGV, &newSignalHandler, &oldSignalHandler) == -1)
	{
		err(1, "Cannot save previous sigaction.\n");
	}


	//printf("start sigsetjmp.\n");
	int SignalValue = sigsetjmp(signalBuffer, 1);
	if ( SignalValue == 0)
		readWriter = *address;
	else
	{
		//Cant read address.
		//printf("cant read address.\n");
		sigaction(SIGSEGV, &oldSignalHandler, NULL);
		return -1;
	}

	SignalValue = sigsetjmp(signalBuffer, 1);

	if ( SignalValue == 0)
	{
		*address = readWriter;
	}
	else
	{
		//Cant write to address.
		//printf("cant write address, but can read.\n");
		sigaction(SIGSEGV, &oldSignalHandler, NULL);
		return 0;
	}
	
	*address = readWriter;
	//printf("can read and write to address.\n");
	sigaction(SIGSEGV, &oldSignalHandler, NULL);
	return 1;
}



void initiate_handler() {
	newSignalHandler.sa_handler = handler;
	sigemptyset(&newSignalHandler.sa_mask);
	newSignalHandler.sa_flags = 0;
	if (sigaction(SIGSEGV, &newSignalHandler, &oldSignalHandler) == -1)
	{
		err(1, "Cannot save previous sigaction.\n");
	}	
}


void restore_handler() {
	if (sigaction(SIGSEGV, &oldSignalHandler, NULL) == -1) {
		err(1, "Cannot restore previous signal handler.\n");
	}
}


void handler(int signo) {
	siglongjmp(signalBuffer, 1);
	printf("Does this print?");
}


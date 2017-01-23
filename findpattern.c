#include "findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

int determineIfReadWriteAddressLocation(char *);
void initiate_handler();
void restore_handler();
void handler(int);

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

int main(int argc, char *argv[]) {
	
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
		printf("Start switch.\n");
		int hold = determineIfReadWriteAddressLocation(currentAddress);
		printf("hold is: %d\n", hold);
		switch (hold)
		{
			//-1 means not read/write
			case -1: currentAddress++; continue; break;
			//0 means read, not write
			case 0:
				data = *currentAddress++; printf(" current data: %c\n",data); break;
			//1 means read and write
			case 1:
				printf("attempting to read from address...");
				data = *currentAddress++;
				printf(" current data: %c\n",data);
				break;
		}
		printf("Current data: %c\n",data);

		if(data == *pattern)
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
		printf("cant read address.\n");
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
		printf("cant write address, but can read.\n");
		sigaction(SIGSEGV, &oldSignalHandler, NULL);
		return 0;
	}
	
	*address = readWriter;
	printf("can read and write to address.\n");
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


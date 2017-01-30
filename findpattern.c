#include "findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int determineIfReadWriteAddressLocation(char *);

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

int main(int argc, char *argv[]) {
	
	/*For testing purposes -------*/
	unsigned char *testPattern = (char*)'A';
	unsigned int testPatLength = 2;
	struct patmatch *testLocations[10];
	unsigned int testLocLength = 10;
	/*----------------------------*/

	int count = findpattern((char*)testPattern, testPatLength, (struct patmatch *)testLocations, testLocLength);
	return count;
}

/*
Scans the addresss space of the calling process and return in the
locations array all the different locations at which the pattern
is found.

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
	printf("pageSize: %d\n",pageSize);
	long pageTotal = (0xffffffff) / pageSize;
	printf("pageTotal: %ld\n",pageTotal);
	long currentPage = 0;
	int patternCount = 0;
	char data = ' ';
	
	char locationIndex = 0;
	int MemoryReadWriteType;
	int patternFoundCount = 0;	

	for(currentPage = 0; currentPage < pageTotal; currentPage++)
	{
		if(currentPage == pageTotal){
			printf("last address read: 0x%x\n",(int)currentAddress);}
		
		long i;	
		for(i = 0; i < pageSize; i++)
		{
			MemoryReadWriteType = determineIfReadWriteAddressLocation(currentAddress);
			/*			
			switch (MemoryReadWriteType)
			{
				//-1 means not read/write
				case -1:
					currentAddress += pageSize;					
					break;
				//0 means read, not write
				case 0:
					data = *currentAddress;
					break;
				//1 means read and write
				case 1:
					data = *currentAddress;
					break;
			}
			*/
			if(MemoryReadWriteType == -1)
			{
				currentAddress += pageSize;
				break;
			}

			//printf("reading: 0x%x\n", (int)currentAddress);
			currentAddress += 1;

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
				if(locationIndex < loclength)
				{
					locations[locationIndex].location = (int)patternFoundAddress;
					if(MemoryReadWriteType == MEM_RW)
						locations[locationIndex].mode = (char)MEM_RW;
					else
						locations[locationIndex].mode = (char)MEM_RO;
					locationIndex++;
				}
				patternFoundCount++;
			}
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


	//Determine if address is readable.
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
	
	//Determine if address is writable.
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
	
	sigaction(SIGSEGV, &oldSignalHandler, NULL);
	return 1;
}
<<<<<<< HEAD

void handler(int signo) {
	siglongjmp(signalBuffer, 1);
	//printf("Does this print?");
}

=======
>>>>>>> 10f4fc8ebdfd9e08e055692da812557eb3ed1fe8

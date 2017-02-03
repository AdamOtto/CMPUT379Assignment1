#include "findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int determineIfReadWriteAddressLocation(char *);

static sigjmp_buf signalBuffer;
static struct sigaction newSignalHandler, oldSignalHandler;

/*
int main(int argc, char *argv[]) {
	
	/*For testing purposes -------/
	unsigned char *testPattern = (char*)'A';
	unsigned int testPatLength = 1;
	struct patmatch *testLocations[10];
	unsigned int testLocLength = 10;
	/*----------------------------/

	int count = findpattern((char*)testPattern, testPatLength, (struct patmatch *)testLocations, testLocLength);
	return count;
}
*/

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
	long pageTotal = (0xffffffff) / pageSize;
	long currentPage = 0;
	int patternCount = 0;
	char data = ' ';
	
	char locationIndex = 0;
	int MemoryReadWriteType;
	int patternFoundCount = 0;	

	for(currentPage = 0; currentPage <= pageTotal; currentPage++)
	{
		long i;	
		for(i = 0; i < pageSize; i++)
		{
			MemoryReadWriteType = determineIfReadWriteAddressLocation(currentAddress);
			
			if(MemoryReadWriteType == -1)
			{
				currentAddress += pageSize;
				break;
			}
			
			data = *currentAddress;			

			currentAddress += 1;

			if(data == (int)pattern)
			{
				if(patternCount == 0)
					patternFoundAddress = currentAddress;
				patternCount++;
			}
			else
			{
				patternCount = 0;
			}
		
			if(patternCount == patlength)
			{
				//printf("Pattern found at: 0x%x\n", (int)patternFoundAddress );
				
				if(locationIndex < loclength)
				{
					(locations[locationIndex]).location = (int)patternFoundAddress;
					if(MemoryReadWriteType == MEM_RW)
						(locations[locationIndex]).mode = '0';//MEM_RW;
					else
						(locations[locationIndex]).mode = '1';//MEM_RO;
					locationIndex++;
				}
				
				patternFoundCount++;
			}
		}
	}
	//printf("last address read: 0x%x\n",(int)currentAddress);
	return patternFoundCount;
}

/*
A handler to help process signals.
*/
void handler(int signalNum) {
	siglongjmp(signalBuffer, 1);
}

/*
Scans the given address and determines if it is read, read/write or neither using signals

Returns the either -1,0 or 1.
-1 means address isn't read or write
0 means read only
1 means read and write.

address: char containing the address to scan.
*/
int determineIfReadWriteAddressLocation(char * address)
{
	//printf("start initiate_handler.\n");
	char readWriter;
	
	newSignalHandler.sa_handler = handler;
	sigemptyset(&newSignalHandler.sa_mask);
	newSignalHandler.sa_flags = 0;
	if (sigaction(SIGSEGV, &newSignalHandler, &oldSignalHandler) == -1)
		err(1, "Last sigaction not saved.\n");


	//Determine if address is readable.
	int SignalValue = sigsetjmp(signalBuffer, 1);
	if ( SignalValue == 0)
		readWriter = *address;
	else
	{
		//Cant read address.
		sigaction(SIGSEGV, &oldSignalHandler, NULL);
		return -1;
	}
	
	//Determine if address is writable.
	SignalValue = sigsetjmp(signalBuffer, 1);
	if ( SignalValue == 0)
		*address = readWriter;
	else
	{
		//Cant write to address.
		sigaction(SIGSEGV, &oldSignalHandler, NULL);
		return 0;
	}
	
	sigaction(SIGSEGV, &oldSignalHandler, NULL);
	return 1;
}

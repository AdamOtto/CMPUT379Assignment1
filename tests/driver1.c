#include "../findpattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	/*For testing purposes -------*/
	unsigned char *testPattern = (char*)'A';
	unsigned int testPatLength = 2;
	struct patmatch *testLocations[10];
	unsigned int testLocLength = 10;
	/*----------------------------*/

	int count = findpattern((char*)testPattern, testPatLength, (struct patmatch *)testLocations, testLocLength);
	return count;
}









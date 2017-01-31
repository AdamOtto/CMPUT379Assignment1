#include "../findpattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	
	unsigned int PAGE_SIZE = getpagesize();

	unsigned char *pattern = (char *) 'A';
	unsigned int patlength = 2;

	struct patmatch locations[10];
	unsigned int loclength = sizeof(locations) / sizeof(struct patmatch);
	
	
	printf("Test 1:\n");
	unsigned int number = findpattern(pattern, patlength, locations, loclength);
	
	//First test	
	printf("Pass 1\nTotal Matches: %d\n", number);
	int i;
	/*	
	for (i = 0; i < loclength; i++)
	{
		if(locations[i].location != NULL && locations[i].mode != NULL)
		{
			printf("%d\t%c",locations[i].location, locations[i].mode=='0'?"MEM_RW":"MEM_RO");
		}			
	}
	*/

	//This address may need to be changed later if its read only.
	/*
	char *address = 0xAAAAAAAA;
	for(i = 0; i < patlength; i++)
	{
		*address = 'A';
		address++;
	}
	*/

	unsigned char *pattern2 = (char *) 'A';
	unsigned int patlength2 = 2;

	struct patmatch locations2[10];
	unsigned int loclength2 = sizeof(locations) / sizeof(struct patmatch);	
	
	//Second test with changes.
	printf("Pass 2\n");
	number = findpattern(pattern2, patlength2, locations2, loclength2);
	printf("Total Matches: %d\n", number);

	/*
	for (i = 0; i < loclength; i++)
	{
		if(locations[i].location != NULL && locations[i].mode != NULL)
		{
			printf("%d\t%c\t%c",locations2[i].location, locations2[i].mode=='0'?"MEM_RW":"MEM_RO",
				(locations2[i].mode != NULL && locations[i] == NULL)?
					"N"
					:
					( locations2[i].mode == locations[i].mode ? "U":"C")
			);
		}			
	}
	*/
}









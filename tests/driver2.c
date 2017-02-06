#include "../findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, unsigned char *argv[]) {

    // Initialize variables for the first call to find pattern
    unsigned char * pattern = (char *)argv[1];
    unsigned int patlength = 2;
    struct patmatch locations[10];
    unsigned int loclength = 10;

    // First test
    printf("\nTest 2:\n");
    unsigned int number = findpattern((char *)(long)(*pattern), patlength, locations, loclength);

    //Display the patterns found	
    printf("Pass 1\nTotal Matches: %d\n", number);
    int i;
		
    for (i = 0; i < loclength; i++) {
        if (i == number) {
            break;
        }
        printf("0x%x\t",(locations[i]).location);
        if (locations[i].mode == '0') {
            printf("MEM_RO\n");
        } else {
            printf("MEM_RW\n");
        }			
    }

    // Adds an instance of the pattern on the stack by creating two new variables.
    char a1 = *argv[1];
    char a2 = *argv[1];

    // Initialize variables for test 2
    unsigned char *pattern2 = pattern;
    unsigned int patlength2 = 2;
    struct patmatch locations2[10];
    unsigned int loclength2 = 10;	

    // Second test with changes.
    printf("Pass 2\n");
    number = findpattern( (char *)(long) *pattern, patlength2, locations2, loclength2);
    printf("Total Matches: %d\n", number);
    int oldFindPatternIndex = 0;

    for (i = 0; i < loclength2; i++) {
        if (i == number) {
            break;
        }
        printf("0x%x\t", locations2[i].location);
		
        if (locations2[i].mode == '0') {
            printf("MEM_RO\t");
        } else {
            printf("MEM_RW\t");	
        }	

        if (locations2[i].location == locations[oldFindPatternIndex].location) {
            if (locations2[i].mode == locations[oldFindPatternIndex].mode) {
                printf("U\n");
            } else {
                printf("C\n");
            }
            oldFindPatternIndex++;
        } else {
            printf("N\n");
        }					
    }
    return 0;
}

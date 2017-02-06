#include "../findpattern.h"
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

void createPattern3(unsigned char pattern, unsigned int patlength);

int main(int argc, unsigned char *argv[]) {

    // Initialize variables for the first call to find pattern
    unsigned char * pattern = (char *) argv[1];
    unsigned int patlength = 2;
    struct patmatch locations[10];
    unsigned int loclength = 10;

    // Change the address space for the first test using mmap.
    struct stat sb;
    int fd = open("driver3test",O_RDWR);
    if (fd == -1) {
        printf("Could not open driver3File.\n");
    } else {
        if (fstat(fd, &sb) == -1) {
            printf("fstat Error.");
        }
    }
    // Map the address into memory.
    char* mapped = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        printf("Could not map file.\n");
    }
    // First test.
    printf("\nTest 3:\n");
    unsigned int number = findpattern( (char *)(long) *pattern, patlength, locations, loclength);
    munmap(mapped, sb.st_size);

    // Display the patterns found.	
    printf("Pass 1\nTotal Matches: %d\n", number);
    int i;
    for (i = 0; i < loclength; i++) {
        if (i == number) {
            break;
        }
        printf("0x%x\t", locations[i].location);
        if (locations[i].mode == '0') {
            printf("MEM_RO\n");
        } else {
            printf("MEM_RW\n");
        }
    }

    // Change the address space for the second test using mmap.
    mmap(0, sb.st_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        printf("Could not map file.\n");	
    }

    // Initialize variables for test 2.
    unsigned char *pattern2 = pattern;
    unsigned int patlength2 = 2;
    struct patmatch locations2[10];
    unsigned int loclength2 = 10;

    // Second test with changes.
    printf("Pass 2\n");
    number = findpattern((char *)(long) *pattern, patlength2, locations2, loclength2);
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
        }
        else {
            printf("N\n");					
        }
    }
    munmap(mapped, sb.st_size);
    close(fd);
    return 0;
}

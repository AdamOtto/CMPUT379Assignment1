#include "findpattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int PAGE_SIZE = getpagesize();

int main() {

    unsigned char *pattern = (char *) 'A';
    unsigned int patlength = 1;

    struct patmatch locations[10];
    unsigned int loclength = sizeof(locations) / sizeof(patmatch);

    unsigned int number = findpattern(pattern, patlength, locations, loclength)
    printf(number);

}

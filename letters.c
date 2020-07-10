#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>


#include "wordbank.h"

char __word__[9];
char __rodw__[9];

int main(int argv, char* argc[])
{
    int i, j, pick;

    srand(time(0));

    int i_word = rand() % 1000;

    for (i = 0; i < 9; ++i)
	__word__[i] = wordbank[i_word][i];
    
    for (i = 0; i < 9; ++i) {
	pick = rand() % (9 - i);
	__rodw__[i] = __word__[pick];
	for (j = pick; j < (9 - i); ++j) {
	    __word__[j] = __word__[j+1];
	}
    }

    for (i = 0; i < 9; ++i)
	__word__[i] = wordbank[i_word][i];

    printf("\n┌──────────────────────────────┐\n");
    printf("│      ");
    for (i = 0; i < 9; ++i)
	printf("%c ", __rodw__[i]);
    printf("      │\n");
    printf("└──────────────────────────────┘\n\n");

    int timer = 30;
    // this is stuff for the timer bar
    printf("[");
    for (i = 0; i < timer; ++i)
	printf(" ");
    printf("]");
    fflush(stdout);
    for (i = 0; i < timer; ++i) {
	sleep(1);
	for (j = 0; j < timer-i+1; ++j)
	    printf("\b");
	printf("=");
	for (j = 0; j < timer-i-1; ++j)
	    printf(" ");
	printf("]");
	fflush(stdout);
    }
    printf("\n\n");
    sleep(1);

    printf("Did you get it?\n");
    scanf("%s", __rodw__);

    int check = 1;
    for (i = 0; i < 9; ++i) {
	if (toupper(__rodw__[i]) == __word__[i]) {
	    continue;
	} else {
	    check = 0;
	    break;
	}
    }
    
    printf("Let's see what it was...\n\n");
    sleep(1);

    printf("┌──────────────────────────────┐\n");
    printf("│      ");
    for (i = 0; i < 9; ++i)
	printf("%c ", __word__[i]);
    printf("      │\n");
    printf("└──────────────────────────────┘\n\n");

    if (check)
	printf("Nice job!\n");
    else
	printf("You're a failure!\n");
    
    return 0;
}

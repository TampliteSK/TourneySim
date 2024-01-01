// misc.c

#include <stdio.h>
#include "defs.h"

// Initialises players
void initPlayers(_Player *playList, uint8_t *numPlays) {
    FILE *fptr = fopen("players.txt", "r");
    *numPlays = 0;
    while (!feof(fptr)) { // EOF only checks for character
      // %hu for short (unsigned)
      fscanf(fptr, "%s%hu", playList[*numPlays].name, &playList[*numPlays].rating);
      playList[*numPlays].score = 0;
      playList[*numPlays].games = 0;
      playList[*numPlays].scoreRate = 0;
      playList[*numPlays].perf = 0;
      playList[*numPlays].isSelected = 0;
      ++*numPlays;
    }
    fclose(fptr);
}

// Sorts players by scores
void sortPlayers(_Player *playList, uint8_t numPlayers) {
    // Bubble sort algorithm
    for (int i = 0; i < numPlayers - 1; i++) {
        for (int j = 0; j < numPlayers - i - 1; j++) {
            // Compare scores and swap if necessary
            if (playList[j].score < playList[j+1].score) {
                _Player temp = playList[j];
                playList[j] = playList[j+1];
                playList[j+1] = temp;
            }
        }
    }
}

// Prints players' data
void printData(_Player *playList, uint8_t numPlayers) {
    printf("\n\n");
    printf("╔═════════════════════╦════════╦═══════╦══════╗\n");
    printf("║       Players       ║ Rating ║ Score ║ Perf ║\n");
    for (int i = 0; i < numPlayers; ++i)
      printf("║%21s║%8d║%4.1lf/%-2d║%6.1lf║\n", playList[i].name, playList[i].rating, playList[i].score, playList[i].games, playList[i].perf);
    printf("╚═════════════════════╩════════╩═══════╩══════╝\n");
}
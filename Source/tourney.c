// tourney.c

#include <stdio.h>
#include <stdlib.h> // srand()
#include <time.h> // time()
#include "defs.h"

// Generates random pairs
void randPairs(_Player *players, uint8_t numPlays, _Pair *pairList, uint8_t numPairs) {

  srand(time(NULL)); // RNG seed

  // Reset the isSelected flag of all players to 0
  for (int i = 0; i < numPlays; i++) {
    players[i].isSelected = 0;
  }

  // Generate random pairs
  // printf("Randomly generated pairs:\n");
  for (int i = 0; i < numPairs; i++) {
      int index1 = 0, index2 = 0;

      // Randomly find the first unvisited player
      do {
          index1 = rand() % numPlays;
      } while (players[index1].isSelected);

      players[index1].isSelected = 1; // Mark player as visited

      // Randomly find the second unvisited player
      do {
          index2 = rand() % numPlays;
      } while (players[index2].isSelected);

      players[index2].isSelected = 1; // Mark player as visited

      // Create a pair with the selected players
      pairList[i].player_White = players[index1];
      pairList[i].player_Black = players[index2];

      // Pit them and update their data
      fightAndUpdate(&pairList[i]);
      // These statements are required or else players variables wont update
      players[index1] = pairList[i].player_White;
      players[index2] = pairList[i].player_Black;
    
      /*
      // Print the pair (debug)
      printf("%s vs %s\n", pairList[i].player_White.name, pairList[i].player_Black.name);
      */
  }

}

void generateRoundRobinSchedule(_Player *players, uint8_t numPlayers, _Pair *schedule) {

    uint8_t numPairs = numPlayers / 2;

    // Generate round-robin schedule
    for (int round = 1; round <= numPlayers - 1; round++) {
        // printf("\n\n=== Round %d ===\n", round);
        randPairs(players, numPlayers, schedule, numPairs);

        // Rotate players
        _Player lastPlayer = players[numPlayers - 1];
        for (int i = numPlayers - 1; i > 0; i--) {
            players[i] = players[i - 1];
        }
        players[0] = lastPlayer;
    }

}
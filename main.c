#include <stdio.h>
#include <stdlib.h> // malloc()
#include <math.h> // ceil()
#include <stdint.h> // uint8_t: 0~127， uint16_t: 0~32767
#include <time.h> // time()
#include "defs.h"

// Main program
int main(void) {
  int tournType = DOUBLE_ROUND_ROBIN;
  uint8_t numPlayers;
  
  // Allocating memory for "players"
  _Player *players;
  players = (_Player *)malloc( sizeof(_Player) * MAX_PLAYERS );
  if (players == NULL) {
    printf("Failed to allocate memory for \"players\".\n");
    return -1;
  }
  
  initPlayers(players, &numPlayers);

  /* File input debug
  for (int i = 0; i < numPlayers; ++i)
    printf("Player %d: %s (%hu)\n", i, players[i].name, players[i].rating);
  */
  
  // Round robin simulation
  if (tournType == DOUBLE_ROUND_ROBIN || tournType == ROUND_ROBIN) {
    
    uint8_t numPairs = numPlayers / 2;
    uint8_t numRounds = numPlayers - 1;
    uint8_t totalMatches = numPairs * numRounds;

    // Allocate memory for schedule
    _Pair *schedule = (_Pair *)malloc(totalMatches * sizeof(_Pair));
    if (schedule == NULL) {
      printf("Failed to allocate memory for \"schedule\".\n");
        return -1;
    }
        
    for (int sim = 0; sim < NUM_SIMULATIONS; ++sim) {
      
      for (int cycle = 0; cycle < tournType + 1; ++cycle) {

        generateRoundRobinSchedule(players, numPlayers, schedule);
        
      }
      
      sortPlayers(players, numPlayers);
      players[0].wins++;
      updatePerfs(players, numPlayers);
      
    }
    
  }

  scaleScores(players, numPlayers, tournType);
  sortPlayers(players, numPlayers);
  updateWinProbs(players, numPlayers);
  printData(players, numPlayers);
  
  return 0;
}
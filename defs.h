// defs.h

#include <stdio.h>
#include <stdint.h>

#define MAX_PLAYERS 100
// Will take over 30s for 50000+ simulations
#define NUM_SIMULATIONS 10

enum {ROUND_ROBIN, DOUBLE_ROUND_ROBIN};

typedef struct {
  char name[20];
  uint16_t rating; // FIDE, Chesscom, CCRL etc.
  double score;
  uint32_t games;
  double scoreRate;
  double perf;
  uint8_t isSelected;
} _Player;

typedef struct {
  _Player player_White;
  _Player player_Black;
} _Pair;

/**************
** Functions **
**************/

// formulas.c
extern double eloNormal(int16_t eloDiff);
extern double eloPerPawnAtElo(uint16_t elo);
extern void winDrawLoss(uint16_t rating1, uint16_t rating2, double *player1_winP, double *drawP, double *player2_winP);
extern void fightAndUpdate(_Pair *pair);
extern double calcPerf(_Player *playList, uint8_t numPlayers, uint16_t rating, double scoreRate);
extern void updatePerfs(_Player *playList, uint8_t numPlayers);
extern void scaleScores(_Player *playList, uint8_t numPlayers, int rrType);

// tourney.c
extern void randPairs(_Player *players, uint8_t numPlays, _Pair *pairList, uint8_t numPairs);
extern void generateRoundRobinSchedule(_Player *players, uint8_t numPlayers, _Pair *schedule);

// misc.c
extern void initPlayers(_Player *playList, uint8_t *numPlays);
extern void sortPlayers(_Player *playList, uint8_t numPlayers);
extern void printData(_Player *playList, uint8_t numPlayers);

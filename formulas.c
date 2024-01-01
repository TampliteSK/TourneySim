// formulas.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include "defs.h"

// The following functions are from François Labelle (wismuth.com)

// Writing these as macros somehow obtains a wrong value, ordering probably?
double eloNormal(int16_t eloDiff) {
  return erfc( -eloDiff / ( (2000.0/7) * sqrt(2) ) ) / 2;
}

double eloPerPawnAtElo(uint16_t elo) {
  return exp(elo/1020.0) * 26.59;
}

// Calculate win-draw-loss probabilities
void winDrawLoss(uint16_t rating1, uint16_t rating2, double *player1_winP, double *drawP, double *player2_winP) {
    // Where rating1 <= rating2
    int16_t eloDiff = rating1 - rating2;
    // printf("eloDiff = %hi\n", eloDiff);
    double avgElo = (rating1 + rating2) / 2.0;
    // printf("avgElo = %lf\n", avgElo);

    double expectedScore = eloNormal(eloDiff);
    // printf("expectedScore = %lf\n", expectedScore);
    double eloShift = eloPerPawnAtElo(avgElo) * 0.6;
    // printf("eloShift = %lf\n", eloShift);

    *player1_winP = eloNormal(eloDiff - eloShift);
    // expectedScore = player1_winP + drawP / 2
    *drawP = (expectedScore - *player1_winP) * 2;
    *player2_winP = 1 - *player1_winP - *drawP;
}

// Determine game result and update data accordingly
void fightAndUpdate(_Pair *pair) {

    uint16_t whiteRating = pair->player_White.rating;
    uint16_t blackRating = pair->player_Black.rating;

    double white_winP, drawP, black_winP;

    // Lichess' masters database gives White a first-move advantage of ~+31 elo
    if ( (whiteRating + 31) > blackRating) {
        winDrawLoss(blackRating, whiteRating + 31, &black_winP, &drawP, &white_winP);
    } else {
        winDrawLoss(whiteRating + 31, blackRating, &white_winP, &drawP, &black_winP);
    }

    // printf("White win: %.4lf%%\n", white_winP * 100);
    // printf("Draw: %.4lf%%\n", drawP * 100);
    // printf("Black win: %.4lf%%\n", black_winP * 100);

    srand(time(NULL)); // RNG seed
    double randResult = (double)rand() / RAND_MAX;
    // printf("\nrandResult = %lf\n", randResult);

    // Updating data of players depending on result
    // Separate 0.00 - 1.00 block to white_winP, draw and black_winP
    if (randResult <= white_winP) {
      // white win
      pair->player_White.score += 1;
      // printf("%s score: %.1lf\n", pair->player_White.name, pair->player_White.score);
    } else {
      if (randResult <= (white_winP + drawP)) {
        // draw
        pair->player_White.score += 0.5;
        pair->player_Black.score += 0.5;
        // printf("%s score: %.1lf\n", pair->player_White.name, pair->player_White.score);
        // printf("%s score: %.1lf\n", pair->player_Black.name, pair->player_Black.score);
      } else {
        // black win
        pair->player_Black.score += 1;
        // printf("%s score: %.1lf\n", pair->player_Black.name, pair->player_Black.score);
      }
    }
    pair->player_White.games++;
    pair->player_Black.games++;
    pair->player_White.scoreRate = pair->player_White.score / pair->player_White.games;
    pair->player_Black.scoreRate = pair->player_Black.score / pair->player_Black.games;

}

// Calculates performance
double calcPerf(_Player *playList, uint8_t numPlayers, uint16_t rating, double scoreRate) {

  double opposition = 0;
  for (int i = 0; i < numPlayers; ++i)
    opposition += playList[i].rating;
  opposition = (double)(opposition - rating) / (numPlayers - 1);

  double relativePerf = -400 * log10( 1/scoreRate - 1 );

  return opposition + relativePerf;

}

// Updates performances of players
void updatePerfs(_Player *playList, uint8_t numPlayers) {
  for (int i = 0; i < numPlayers; ++i)
    playList[i].perf = calcPerf(playList, numPlayers, playList[i].rating, playList[i].scoreRate);
}

// Scales down scores based on number of simulations (round robins)
void scaleScores(_Player *playList, uint8_t numPlayers, int rrType) {
    for (int i = 0; i < numPlayers; ++i) {
        playList[i].games = (numPlayers - 1) * (rrType + 1);
        playList[i].score = playList[i].games * playList[i].scoreRate;
    }
}

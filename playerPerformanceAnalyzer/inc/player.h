#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player
{
    int id;
    char name[50];
    char team[50];
    char role[20];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
} Player;

void printPlayer(const Player *player);

#endif

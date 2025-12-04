#include <stdio.h>

#include "player.h"

void printPlayer(const Player *player)
{
    if (player == NULL)
    {
        return;
    }
    printf("%-5d %-20s %-12s %-12s %6d %6.2f %6.2f %6d %6.2f %8.2f\n",
           player->id,
           player->name,
           player->team,
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->role,
           player->economyRate,
           player->performanceIndex,
           player->wickets);
}

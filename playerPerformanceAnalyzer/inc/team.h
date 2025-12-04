#ifndef TEAM_H
#define TEAM_H

#include "player.h"

typedef struct Team
{
    int teamId;
    char name[50];
    Player players[50];
    int totalPlayers;
    float avgBattingStrikeRate;
} Team;

void computeTeamStrikeRate(Team *team);
void printTeam(const Team *team);

#endif

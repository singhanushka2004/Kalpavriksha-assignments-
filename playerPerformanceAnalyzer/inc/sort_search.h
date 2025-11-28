#ifndef SORT_SEARCH_H
#define SORT_SEARCH_H

#include "player.h"
#include "team.h"

int searchTeamById(const Team teams[], int teamsCount, int teamId);
void sortTeamsByStrikeRate(Team teams[], int teamsCount);
void sortPlayersByPerformance(Player players[], int playersCount);

#endif
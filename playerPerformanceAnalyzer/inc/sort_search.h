#ifndef SORT_SEARCH_H
#define SORT_SEARCH_H

#include "player.h"
#include "team.h"

void sortTeamsByStrikeRate(Team teams[], int teamsCount);
void sortPlayersByPerformance(Player players[], int playersCount);
int searchTeamById(const Team teams[], int teamsCount, int teamId);

#endif

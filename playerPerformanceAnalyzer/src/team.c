#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "team.h"
#include "player.h"
#include "performance.h"

void computeTeamStrikeRate(Team *team)
{
    if (!team)
    {
        return;
    }
    int count = 0;
    float totalStrikeRate = 0.0f;
    for (int index = 0; index < team->totalPlayers; ++index)
    {
        const char *role = team->players[index].role;
        if (strcasecmp(role, "Batsman") == 0 || strcasecmp(role, "All-rounder") == 0)
        {
            totalStrikeRate += team->players[index].strikeRate;
            count++;
        }
    }
    team->avgBattingStrikeRate = (count > 0) ? (totalStrikeRate / count) : 0.0f;
}

void printTeam(const Team *team)
{
    if (!team)
    {
        return;
    }
    printf("\nPlayers of Team %s:\n", team->name);
    printf("===================================================================================\n");
    printf("%-6s %-20s %-12s %-6s %-6s %-6s %-6s %-5s %-8s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("-----------------------------------------------------------------------------------\n");
    for (int index = 0; index < team->totalPlayers; ++index)
    {
        printPlayer(&team->players[index]);
    }
    printf("===================================================================================\n");
}
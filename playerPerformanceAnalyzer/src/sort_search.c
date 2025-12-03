#include <stdlib.h> 

#include "search.h"
#include "sort.h"
#include "player.h"
#include "team.h"

int searchTeamById(const Team teams[], int teamsCount, int teamId)
{
    int low = 0;
    int high = teamsCount - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (teams[mid].teamId == teamId)
        {
            return mid;
        }

        if (teams[mid].teamId < teamId)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

static void mergeTeams(Team teams[], Team temp[], int left, int mid, int right)
{
    int i = left;
    int j = mid;
    int k = left;
    while (i < mid && j <= right)
    {
        if (teams[i].avgBattingStrikeRate >= teams[j].avgBattingStrikeRate)
        {
            temp[k] = teams[i];
            k++;
            i++;
        }
        else
        {
            temp[k] = teams[j];
            k++;
            j++;
        }
    }

    while (i < mid)
    {
        temp[k] = teams[i];
        k++;
        i++;
    }

    while (j <= right)
    {
        temp[k] = teams[j];
        k++;
        j++;
    }

    for (int index = left; index <= right; index++) 
    {
        teams[index] = temp[index];
    }
}

static void mergeSortTeamsRecursive(Team teams[], Team temp[], int left, int right) 
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSortTeamsRecursive(teams, temp, left, mid);
    mergeSortTeamsRecursive(teams, temp, mid + 1, right);
    mergeTeams(teams, temp, left, mid + 1, right);
}

void sortTeamsByStrikeRate(Team teams[], int teamsCount)
{
    if (teams == NULL || teamsCount <= 1)
    {
        return;
    }

    Team *temp = (Team *)malloc(sizeof(Team) * teamsCount);
    if (temp == NULL)
    {
        return;
    }
    mergeSortTeamsRecursive(teams, temp, 0, teamsCount - 1); 
    free(temp);
}

static void mergePlayers(Player players[], Player temp[], int left, int mid, int right)
{
    int i = left;
    int j = mid;
    int k = left;
    while (i < mid && j <= right)
    {
        if (players[i].performanceIndex >= players[j].performanceIndex)
        {
            temp[k] = players[i];
            k++;
            i++;
        }
        else
        {
            temp[k] = players[j];
            k++;
            j++;
        }
    }

    while (i < mid)
    {
        temp[k] = players[i];
        k++;
        i++;
    }

    while (j <= right)
    {
        temp[k] = players[j];
        k++;
        j++;
    }

    for (int index = left; index <= right; index++) 
    {
        players[index] = temp[index];
    }
}

static void mergeSortPlayersRecursive(Player players[], Player temp[], int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSortPlayersRecursive(players, temp, left, mid);
    mergeSortPlayersRecursive(players, temp, mid + 1, right);
    mergePlayers(players, temp, left, mid + 1, right);
}

void sortPlayersByPerformance(Player players[], int playersCount)
{
    if (players == NULL || playersCount <= 1)
    {
        return;
    }

    Player *temp = (Player *)malloc(sizeof(Player) * playersCount);
    if (temp == NULL)
    {
        return;
    }
    mergeSortPlayersRecursive(players, temp, 0, playersCount - 1);
    free(temp);
}

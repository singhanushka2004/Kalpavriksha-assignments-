#include <string.h>
#include <ctype.h>
#include "performance.h"

static int compareStringsInsensitive(const char *ch1, const char *ch2)
{
    if (ch1 == NULL || ch2 == NULL)
    {
        return 0;
    }

    int i = 0;
    while (1)
    {
        char charA = tolower(ch1[i]);
        char charB = tolower(ch2[i]);

        if (charA != charB)
        {
            return 0;
        }

        if (charA == '\0')
        {
            return 1;
        }
        i++;
    }
}

float computePerformanceIndex(const Player *player)
{
    if (player == NULL)
    {
        return 0.0f;
    }
    if (compareStringsInsensitive(player->role, "Batsman"))
    {
        return (player->battingAverage * player->strikeRate) * 0.01f;
    }
    else if (compareStringsInsensitive(player->role, "Bowler"))
    {
        return (player->wickets * 2.0) + (100.0 - player->economyRate);
    }
    else
    {
        float batPart = (player->battingAverage * player->strikeRate) / 100.0f;
        float bowlPart = (float)player->wickets * 2.0f;
        return batPart + bowlPart;
    }
}

void computePerformanceForAll(Player players[], int playersCount)
{
    if (players == NULL)
    {
        return;
    }
    for (int i = 0; i < playersCount; i++)  
    {
        players[i].performanceIndex = computePerformanceIndex(&players[i]);
    }
}
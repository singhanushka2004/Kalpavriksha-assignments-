#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "menu.h"
#include "player.h"
#include "players_data.h"
#include "performance.h"
#include "search.h"
#include "sort.h"
#include "team.h"

#define MAX_TEAMS 10

static Node *head = NULL;
static Team team[MAX_TEAMS];
static int teamsCount = 0;

extern const Player players[];
extern int playerCount;
extern int teamCount;
extern const char *teams[];

static void initializeTeams()
{
    teamsCount = teamCount;
    if (teamsCount > MAX_TEAMS)
    {
        teamsCount = MAX_TEAMS;
    }
    for (int i = 0; i < teamsCount; i++)
    {
        team[i].teamId = i + 1;
        strncpy(team[i].name, teams[i], sizeof(team[i].name) - 1);
        team[i].name[sizeof(team[i].name) - 1] = '\0';
        team[i].totalPlayers = 0;
        team[i].avgBattingStrikeRate = 0.0f;
    }
}

static void assignPlayerToTeam(const Player *player)
{
    if (player == NULL)
    {
        return;
    }
    for (int i = 0; i < teamsCount; i++)
    {
        if (strcmp(team[i].name, player->team) == 0)
        {
            if (team[i].totalPlayers < 50)
            {
                team[i].players[team[i].totalPlayers] = *player;
                team[i].totalPlayers++;
            }
            return;
        }
    }
}

static void loadInitialData()
{
    int i = 0;
    for (i = 0; i < playerCount; i++)
    {
        Player player;
        player.id = players[i].id;

        strncpy(player.name, players[i].name, sizeof(player.name) - 1);
        player.name[sizeof(player.name) - 1] = '\0';

        strncpy(player.team, players[i].team, sizeof(player.team) - 1);
        player.team[sizeof(player.team) - 1] = '\0';

        strncpy(player.role, players[i].role, sizeof(player.role) - 1);
        player.role[sizeof(player.role) - 1] = '\0';

        player.totalRuns = players[i].totalRuns;
        player.battingAverage = players[i].battingAverage;
        player.strikeRate = players[i].strikeRate;
        player.wickets = players[i].wickets;
        player.economyRate = players[i].economyRate;
        player.performanceIndex = computePerformanceIndex(&player);
        head = insertNode(head, player);
        assignPlayerToTeam(&player);
    }

    for (int idx = 0; idx < teamsCount; idx++)
    {
        computeTeamStrikeRate(&team[idx]);
    }
}

static void computeAllTeamsFromList()
{
    for (int idx = 0; idx < teamsCount; idx++)
    {
        team[idx].totalPlayers = 0;
    }
    Node *cur = head;
    while (cur != NULL)
    {
        assignPlayerToTeam(&(cur->data));
        cur = cur->next;
    }

    for (int idx = 0; idx < teamsCount; idx++)
    {
        computeTeamStrikeRate(&team[idx]);
    }
}

static const char *roleFromChoice(int index)
{
    switch (index)
    {
    case 1:
        return "Batsman";
    case 2:
        return "Bowler";
    default:
        return "All-rounder";
    }
}

static void addNewPlayer()
{
    Player player;
    printf("Enter Team ID to add player: ");
    int teamId;
    if (scanf("%d", &teamId) != 1)
    {
        printf("Invalid numeric input for Team ID.\n");
        return;
    }

    int idx = searchTeamById(team, teamsCount, teamId);
    if (idx < 0)
    {
        printf("Team with ID %d not found.\n", teamId);
        return;
    }

    printf("Enter Player Details:\n");
    printf("Player ID: ");
    if (scanf("%d", &player.id) != 1)
    {
        printf("Invalid ID input.\n");
        return;
    }

    printf("Name: ");
    scanf(" %[^\n]", player.name);
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role;

    if (scanf("%d", &role) != 1)
    {
        printf("Invalid role input.\n");
        return;
    }

    strncpy(player.role, roleFromChoice(role), sizeof(player.role) - 1);
    player.role[sizeof(player.role) - 1] = '\0';

    printf("Total Runs: ");
    if (scanf("%d", &player.totalRuns) != 1)
    {
        printf("Invalid Runs input.\n");
        return;
    }

    printf("Batting Average: ");
    if (scanf("%f", &player.battingAverage) != 1)
    {
        printf("Invalid Average input.\n");
        return;
    }

    printf("Strike Rate: ");
    if (scanf("%f", &player.strikeRate) != 1)
    {
        printf("Invalid SR input.\n");
        return;
    }

    printf("Wickets: ");
    if (scanf("%d", &player.wickets) != 1)
    {
        printf("Invalid Wickets input.\n");
        return;
    }

    printf("Economy Rate: ");
    if (scanf("%f", &player.economyRate) != 1)
    {
        printf("Invalid ER input.\n");
        return;
    }

    strncpy(player.team, team[idx].name, sizeof(player.team) - 1);
    player.team[sizeof(player.team) - 1] = '\0';
    player.performanceIndex = computePerformanceIndex(&player);
    head = insertNode(head, player);
    assignPlayerToTeam(&player);
    computeTeamStrikeRate(&team[idx]);
    printf("Player added successfully to Team %s!\n", team[idx].name);
}

static void displayTeamsSorted()
{
    for (int i = 0; i < teamsCount; i++)
    {
        team[i].totalPlayers = 0;
    }
    Node *cur = head;
    while (cur)
    {
        assignPlayerToTeam(&(cur->data));
        cur = cur->next;
    }
    for (int i = 0; i < teamsCount; i++)
    {
        computeTeamStrikeRate(&team[i]);
    }

    Team temp[MAX_TEAMS];
    for (int i = 0; i < teamsCount; i++)
    {
        temp[i] = team[i];
    }

    sortTeamsByStrikeRate(temp, teamsCount);
    printf("\nTeams Ranked by Average Batting Strike Rate (SR)\n");
    printf("=========================================================\n");
    printf("Key Team Designation      Avg Bat SR  Total Roster\n");
    printf("---------------------------------------------------------\n");

    for (int i = 0; i < teamsCount; i++)
    {
        printf("%-3d %-20s %10.2f %12d\n",
               temp[i].teamId,
               temp[i].name,
               temp[i].avgBattingStrikeRate,
               temp[i].totalPlayers);
    }
    printf("=========================================================\n");
}

static void displayTopKPlayers()
{
    printf("Enter Team ID: ");
    int teamId;
    if (scanf("%d", &teamId) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    int idx = searchTeamById(team, teamsCount, teamId);
    if (idx < 0)
    {
        printf("Team not found.\n");
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int roleIndex;
    if (scanf("%d", &roleIndex) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    const char *role = roleFromChoice(roleIndex);

    Player temp[60];
    int tempCount = 0;

    Node *cur = head;
    while (cur != NULL)
    {
        if ((strcmp(cur->data.team, team[idx].name) == 0) &&
            (strcasecmp(cur->data.role, role) == 0))
        {
            Player player = cur->data;
            player.performanceIndex = computePerformanceIndex(&player);
            temp[tempCount] = player;
            tempCount++;
        }
        cur = cur->next;
    }

    if (tempCount == 0)
    {
        printf("No players matching the role found in this team.\n");
        return;
    }
    printf("Enter number of players K (Rank Limit): ");
    int k;
    if (scanf("%d", &k) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    if (k <= 0)
    {
        printf("K must be a positive number.\n");
        return;
    }
    if (!(k <= tempCount))
    {
        k = tempCount;
    }

    sortPlayersByPerformance(temp, tempCount);

    printf("\nTop %d %s Players in Team %s (Highest Performance Index):\n", k, role, team[idx].name);
    printf("================================================================================\n");
    printf("Rank Name                     Role        Runs   Avg   SR   Wkts  ER   Perf.Score\n");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < k; i++)
    {
        printPlayer(&temp[i]);
    }

    printf("==================================================================\n");
}

static void displayPlayersByPerformance(void)
{
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int roleChoice;
    if (scanf("%d", &roleChoice) != 1)
    {
        printf("Invalid input.\n");
        return;
    }
    const char *role = roleFromChoice(roleChoice);

    Player tempArr[2000];
    int tempCount = 0;
    Node *cur = head;
    while (cur)
    {
        if (strcasecmp(cur->data.role, role) == 0)
        {
            Player player = cur->data;
            player.performanceIndex = computePerformanceIndex(&player);
            tempArr[tempCount] = player;
            tempCount++;
        }
        cur = cur->next;
    }

    if (tempCount == 0)
    {
        printf("No players found for the selected role: %s.\n", role);
        return;
    }

    sortPlayersByPerformance(tempArr, tempCount);
    printf("\nAll Players of role %s across all teams (Sorted by Performance Score):\n", role);
    printf("================================================================================\n");
    printf("Rank Name                     Team        Role        Runs   Avg   SR   Wkts  ER   Perf.Score\n");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < tempCount; i++)
    {
        printPlayer(&tempArr[i]);
    }
    printf("================================================================================\n");
}

static void displayTeamPlayers()
{
    printf("Enter Team ID: ");
    int teamId;
    if (scanf("%d", &teamId) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    int idx = searchTeamById(team, teamsCount, teamId);
    if (idx < 0)
    {
        printf("Team not found.\n");
        return;
    }
    printTeam(&team[idx]);
}

int main()
{
    initializeTeams();
    loadInitialData();
    int choice;
    while (1)
    {
        displayMenu();
        if (scanf("%d", &choice) != 1)
        {
            printf("Non-numeric input detected. Exiting...\n");
            int inputchar;
            while ((inputchar = getchar()) != '\n' && inputchar != EOF);
            return 0;
        }

        switch (choice)
        {
        case ADD_PLAYER:
            addNewPlayer();
            break;
        case DISPLAY_ALL_PLAYERS:
            displayTeamPlayers();
            break;
        case DISPLAY_ALL_TEAMS:
            displayTeamsSorted();
            break;
        case DISPLAY_TOP_K_PLAYERS:
            displayTopKPlayers();
            break;
        case DISPLAY_PLAYERS_BY_PERFORMANCE_INDEX:
            displayPlayersByPerformance();
            break;
        case EXIT:
            printf("Exiting the program...\n");
            destroyList(head);
            return 0;
        default:
            printf("Invalid menu choice selected.\n");
        }
    }
    return 0;
}
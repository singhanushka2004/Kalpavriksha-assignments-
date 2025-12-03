#ifndef MENU_H
#define MENU_H

#include "player.h"

typedef enum
{
    ADD_PLAYER = 1,
    DISPLAY_ALL_PLAYERS,
    DISPLAY_ALL_TEAMS,
    DISPLAY_TOP_K_PLAYERS,
    DISPLAY_PLAYERS_BY_PERFORMANCE_INDEX,
    EXIT
} MenuOptions;
void displayMenu();

typedef struct Node
{
    Player data;
    struct Node *next;
} Node;

Node *insertNode(Node *head, const Player player);
void destroyList(Node *head);
void printList(const Node *head);

#endif

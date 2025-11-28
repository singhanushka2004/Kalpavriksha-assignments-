#include <stdio.h>
#include <string.h>
#include "menu.h"

void displayMenu()
{
    printf("===============================================================================");
    printf("\n                ICC ODI Performance Analyzer                 \n");
    printf("===============================================================================\n");
    printf("%d. Add player to Team.\n", ADD_PLAYER);
    printf("%d. Display Players of a Specific Team.\n", DISPLAY_ALL_PLAYERS);
    printf("%d. Display Teams by Average Batting Strike Rate.\n", DISPLAY_ALL_TEAMS);
    printf("%d. Display Top K Players of a Specific Team by Role.\n", DISPLAY_TOP_K_PLAYERS);
    printf("%d. Display all Players of specific role Across All Teams by Performance Index.\n", DISPLAY_PLAYERS_BY_PERFORMANCE_INDEX);
    printf("%d. Exit.\n", EXIT);
    printf("===============================================================================\n");
    printf("Enter your choice: ");
}

Node *insertNode(Node *head, const Player player)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (NULL == newNode)
    {
        return head;
    }
    (*newNode).data = player;
    newNode->next = head;
    return newNode;
}

void destroyList(Node *head)
{
    Node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void printList(const Node *head)
{
    while (head)
    {
        printPlayer(&(head->data));
        head = head->next;
    }
}

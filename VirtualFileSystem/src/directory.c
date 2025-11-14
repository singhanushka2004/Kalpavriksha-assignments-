#include "directory.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FileNode *gRootNode;
extern FileNode *gCwdNode;

static void insertChildNode(FileNode *parentDir, FileNode *childNode)
{
    childNode->parent = parentDir;
    childNode->next = childNode->prev = NULL;
    if (parentDir->childHead == NULL)
    {
        parentDir->childHead = childNode;
        childNode->next = childNode->prev = childNode;
    }
    else
    {
        FileNode *head = parentDir->childHead;
        FileNode *tail = head->prev;
        tail->next = childNode;
        childNode->prev = tail;
        childNode->next = head;
        head->prev = childNode;
    }
}

static void unlinkChildNode(FileNode *parentDir, FileNode *childNode)
{
    if (parentDir == NULL || childNode == NULL)
        return;
    FileNode *head = parentDir->childHead;
    if (head == NULL)
        return;
    if (head == childNode && childNode->next == childNode)
    {
        parentDir->childHead = NULL;
        childNode->next = childNode->prev = NULL;
        return;
    }
    FileNode *walker = head;
    int found = 0;
    do
    {
        if (walker == childNode)
        {
            found = 1;
            break;
        }
        walker = walker->next;
    } while (walker != head);
    if (!found)
        return;
    childNode->prev->next = childNode->next;
    childNode->next->prev = childNode->prev;
    if (parentDir->childHead == childNode)
        parentDir->childHead = childNode->next;
    childNode->next = childNode->prev = NULL;
}

static void printFullPath(FileNode *directory)
{
    const size_t maxDepth = 1024;
    const char *pathStack[maxDepth];
    size_t depth = 0;
    FileNode *walker = directory;
    while (walker != NULL && walker->parent != NULL)
    {
        pathStack[depth++] = walker->name;
        walker = walker->parent;
        if (depth >= maxDepth)
            break;
    }
    if (depth == 0)
    {
        printf("/\n");
        return;
    }
    putchar('/');
    for (ssize_t idx = (ssize_t)depth - 1; idx >= 0; --idx)
    {
        printf("%s", pathStack[idx]);
        if (idx > 0)
            putchar('/');
    }
    putchar('\n');
}

void cmdMkdir(const char *dirname)
{
    if (dirname == NULL || dirname[0] == '\0')
    {
        printf("Invalid directory name.\n");
        return;
    }
    if (findChildByName(gCwdNode, dirname) != NULL)
    {
        printf("Name already exists in current directory.\n");
        return;
    }
    FileNode *newDir = allocateFileNode(dirname, 1);
    insertChildNode(gCwdNode, newDir);
    printf("Directory '%s' created successfully.\n", dirname);
}

void cmdCd(const char *dirname)
{
    if (dirname == NULL)
    {
        printf("Invalid directory.\n");
        return;
    }
    if (strcmp(dirname, "..") == 0)
    {
        if (gCwdNode->parent != NULL)
        {
            gCwdNode = gCwdNode->parent;
            if (gCwdNode == gRootNode)
                printf("Moved to /\n");
            else
            {
                printf("Moved to ");
                printFullPath(gCwdNode);
            }
        }
        else
        {
            printf("Already at root.\n");
        }
        return;
    }
    if (strcmp(dirname, "/") == 0)
    {
        gCwdNode = gRootNode;
        printf("Moved to /\n");
        return;
    }
    FileNode *target = findChildByName(gCwdNode, dirname);
    if (target == NULL || !target->isDirectory)
    {
        printf("Directory not found.\n");
        return;
    }
    gCwdNode = target;
    printf("Moved to ");
    printFullPath(gCwdNode);
}

void cmdLs(void)
{
    if (gCwdNode->childHead == NULL)
    {
        printf("(empty)\n");
        return;
    }
    FileNode *walker = gCwdNode->childHead;
    do
    {
        if (walker->isDirectory)
            printf("%s/\n", walker->name);
        else
            printf("%s\n", walker->name);
        walker = walker->next;
    } while (walker != gCwdNode->childHead);
}

void cmdPwd(void)
{
    printFullPath(gCwdNode);
}

void cmdRmdir(const char *dirname)
{
    if (dirname == NULL || dirname[0] == '\0')
    {
        printf("Invalid directory name.\n");
        return;
    }
    FileNode *target = findChildByName(gCwdNode, dirname);
    if (target == NULL || !target->isDirectory)
    {
        printf("Directory not found.\n");
        return;
    }
    if (target->childHead != NULL)
    {
        printf("Directory not empty. Remove files first.\n");
        return;
    }
    unlinkChildNode(gCwdNode, target);
    destroySingleNode(target);
    printf("Directory removed successfully.\n");
}

FileNode *vfsGetCwd(void)
{
    return gCwdNode;
}
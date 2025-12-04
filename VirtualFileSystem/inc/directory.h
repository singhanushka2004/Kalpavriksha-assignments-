#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdbool.h>

typedef struct FreeBlock
{
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct FileNode
{
    char *name;
    int isDirectory;

    struct FileNode *parent;
    struct FileNode *child;
    struct FileNode *next;
    struct FileNode *prev;

    int size;
    int blockCount;
    int *blockPointers;
} FileNode;

extern FileNode *root, *cwd;

void initializeDisk();
void initializeRootDirectory();
bool initializeFileNode(const char *name, const int isDirectory);
void freeFileTree(FileNode *node);
void exitProgram();

void makeDirectory(const char *name);
void removeDirectory(const char *name);
void changeDirectory(const char *name);
void showDirectories();
void showCurrentPath();

bool doesDirectoryExist(const char *name);

#endif

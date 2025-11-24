#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

extern char **virtualDisk;
extern FileNode *root;
extern FileNode *cwd;
extern FreeBlock *freeListHead;
extern FreeBlock *freeListTail;

typedef enum CommandType
{
    MKDIR,
    RMDIR,
    CD,
    PWD,
    CREATE,
    WRITE,
    READ,
    DELETE,
    LS,
    DF,
    EXIT,
    INVALID
} CommandType;

CommandType getCommandType(const char *input);
void showPrompt();
void runCli(void);

#endif
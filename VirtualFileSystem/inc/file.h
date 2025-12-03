#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "directory.h"

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

extern char **virtualDisk;
extern FreeBlock *freeListHead, *freeListTail;

void createFile(const char *name);
void writeFile(const char *name, const char *data);
void readFile(const char *name);
void deleteFile(const char *name);

void initializeVirtualBlock();
int allocateBlock();
void freeBlock(const int index);
int allocateBlocks(const int required, int *outArr);
void showDiskDetials();

#endif

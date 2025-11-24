#include "file.h"
#include "directory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int gTotalBlocks = NUM_BLOCKS; 
char **virtualDisk = NULL;

FreeBlock *freeListHead = NULL;
FreeBlock *freeListTail = NULL;

void initializeVirtualBlock() {
    virtualDisk = (char **)malloc(NUM_BLOCKS * sizeof(char *));
    if (!virtualDisk) { fprintf(stderr, "Fatal error: Failed to allocate virtual disk array.\n"); exit(EXIT_FAILURE); }
    for (int index = 0; index < NUM_BLOCKS; index++) {
        virtualDisk[index] = (char *)calloc(BLOCK_SIZE, sizeof(char));
        if (!virtualDisk[index]) { fprintf(stderr, "Fatal error: Failed to allocate block %d.\n", index); exit(EXIT_FAILURE); }
    }
}

int allocateBlock() {
    if (!freeListHead) return -1;
    
    FreeBlock *node = freeListHead;
    int idx = node->index;
    
    freeListHead = node->next;
    if (freeListHead) freeListHead->prev = NULL;
    else freeListTail = NULL;
    
    free(node);
    return idx;
}

void freeBlock(const int index) {
    FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
    if (!newBlock) { fprintf(stderr, "Fatal error: Failed to allocate free block node.\n"); exit(EXIT_FAILURE); }
    
    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = freeListTail;
    
    if (freeListTail) freeListTail->next = newBlock;
    else freeListHead = newBlock;
    
    freeListTail = newBlock;
}

int allocateBlocks(const int required, int *outArr) {
    for (int index = 0; index < required; index++) {
        int idx = allocateBlock();
        if (idx == -1) {
            for (int index2 = 0; index2 < index; index2++) {
                freeBlock(*(outArr + index2));
            }
            return -1;
        }
        *(outArr + index) = idx;
    }
    return 0;
}

void initializeDisk() {
    initializeVirtualBlock();
    
    for (int index = 0; index < NUM_BLOCKS; index++) {
        FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
        if (!newBlock) { fprintf(stderr, "Memory allocation failed during disk initialization.\n"); exit(EXIT_FAILURE); }
        
        newBlock->index = index;
        newBlock->next = NULL;
        newBlock->prev = freeListTail;
        if (!freeListHead) {
            freeListHead = newBlock;
        } else {
            freeListTail->next = newBlock;
        }
        freeListTail = newBlock;
    }
    
    initializeRootDirectory();
    
    printf("$ ./vfs\n");
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

void createFile(const char *name) {
    if (name == NULL || name[0] == '\0') {
        printf("Invalid file name.\n");
    } else if (initializeFileNode(name, 0)) {
        printf("File '%s' created Successfully\n", name);
    }
}

void writeFile(const char *name, const char *data) {
    FileNode *file = findChildByName(cwd, name);

    if (!file || file->isDirectory) {
        printf("File '%s' not found \n", name);
        return;
    }

    int dataLength = strlen(data);
    int requiredBlocks = (dataLength == 0) ? 0 : (dataLength + BLOCK_SIZE - 1) / BLOCK_SIZE;
    
    int *newBlocks = NULL;
    if (requiredBlocks > 0) {
        newBlocks = (int *)calloc(requiredBlocks, sizeof(int));
        if (!newBlocks) { printf("Memory error\n"); return; }
        
        if (allocateBlocks(requiredBlocks, newBlocks) == -1) {
            printf("Not enough free blocks to write '%s'\n", name);
            free(newBlocks);
            return;
        }
    }
    
    if (file->blockPointers) {
        for (int index = 0; index < file->blockCount; index++) {
            freeBlock(*(file->blockPointers + index));
        }
        free(file->blockPointers);
    }

    file->blockPointers = newBlocks;
    file->blockCount = requiredBlocks;
    file->size = dataLength;

    if (requiredBlocks > 0) {
        int pos = 0;
        for (int index = 0; index < requiredBlocks; index++) {
            int idx = *(newBlocks + index);
            size_t toCopy = (dataLength - pos > BLOCK_SIZE) ? BLOCK_SIZE : (dataLength - pos);
            memcpy(virtualDisk[idx], data + pos, toCopy);
            
            if (toCopy < BLOCK_SIZE) {
                memset(virtualDisk[idx] + toCopy, 0, BLOCK_SIZE - toCopy);
            }
            pos += toCopy;
        }
    }

    printf("Data written successfully (size=%d bytes) \n", dataLength);
}

void readFile(const char *name) {
    FileNode *file = findChildByName(cwd, name);

    if (!file || file->isDirectory) {
        printf("File '%s' not found\n", name);
    } else if (file->blockCount == 0 || !file->blockPointers || file->size == 0) {
        printf("File '%s' is empty \n", name);
    } else {
        int remaining = file->size;
        for (int index = 0; index < file->blockCount && remaining > 0; index++) {
            int blockIndex = *(file->blockPointers + index);
            size_t toRead = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
            
            fwrite(virtualDisk[blockIndex], 1, toRead, stdout);
            remaining -= toRead;
        }
        printf("\n");
    }
}

void deleteFile(const char *name) {
    FileNode *target = findChildByName(cwd, name);

    if (!target || target->isDirectory) {
        printf("File '%s' not found \n", name);
        return;
    }
    
    if (target->blockPointers) {
        for (int index = 0; index < target->blockCount; index++) {
            int blockIndex = *(target->blockPointers + index);
            freeBlock(blockIndex);
        }
        free(target->blockPointers);
        target->blockPointers = NULL;
    }
    
    unlinkChildNode(cwd, target);
    destroySingleNode(target);

    printf("File deleted successfully \n");
}

void showDiskDetials() {
    int freeCount = 0;
    FreeBlock *file = freeListHead;
    while (file) {
        freeCount++;
        file = file->next;
    }
    
    int used = NUM_BLOCKS - freeCount;
    double percent = ((double)used / NUM_BLOCKS) * 100.0;

    printf("Disk Status:\n");
    printf("Total Blocks : %d\n", NUM_BLOCKS);
    printf("Used  Blocks : %d\n", used);
    printf("Free  Blocks : %d\n", freeCount);
    printf("Disk  Usage: %.2f%%\n", percent);
}
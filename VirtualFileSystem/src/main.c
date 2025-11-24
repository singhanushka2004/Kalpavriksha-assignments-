#include "file.h"
#include "directory.h"
#include "commandParser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void performCleanup() {
    if (root) {
        freeFileTree(root);
        root = NULL;
        cwd = NULL;
    }
    
    FreeBlock *fb = freeListHead;
    while (fb) {
        FreeBlock *next = fb->next;
        free(fb);
        fb = next;
    }
    freeListHead = freeListTail = NULL;
    
    if (virtualDisk) {
        for (int index = 0; index < NUM_BLOCKS; index++) {
            if (virtualDisk[index]) {
                free(virtualDisk[index]);
            }
        }
        free(virtualDisk);
        virtualDisk = NULL;
    }
}

int main(int argc, char *argv[]) {
    int configuredBlocks = NUM_BLOCKS; 
    if (argc >= 2) {
        int parsed = atoi(argv[1]);
        if (parsed > 0) configuredBlocks = parsed;
    }
    
    initializeDisk(); 

    char input[4096];
    CommandType cmdType;

    while (1) {
        showPrompt();
        
        if (!fgets(input, sizeof(input), stdin)) {
            break; 
        }
        
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) {
            continue;
        }

        char *inputCopy = strdup(input);
        if (!inputCopy) { continue; }

        char *cmd = strtok(inputCopy, " ");
        char *arg1 = strtok(NULL, " ");
        
        char *arg2_start = NULL;
        if (cmd && strcmp(cmd, "write") == 0) {
            if (arg1) {
                arg2_start = strstr(input, arg1) + strlen(arg1);
                while (*arg2_start && isspace((unsigned char)*arg2_start)) {
                    arg2_start++;
                }
            }
        }
        
        if (!cmd) {
            free(inputCopy);
            continue;
        }

        cmdType = getCommandType(cmd);

        switch (cmdType) {
        case MKDIR:
            if (arg1) makeDirectory(arg1);
            else printf("Usage: mkdir <dirname>\n");
            break;

        case RMDIR:
            if (arg1) removeDirectory(arg1);
            else printf("Usage: rmdir <dirname>\n");
            break;

        case CD:
            if (arg1) changeDirectory(arg1);
            else printf("Usage: cd <dirname> or cd ..\n");
            break;

        case PWD:
            showCurrentPath();
            break;

        case CREATE:
            if (arg1) createFile(arg1);
            else printf("Usage: create <filename>\n");
            break;

        case WRITE:
            if (arg1 && arg2_start && *arg2_start) {
                if (*arg2_start == '\"' && arg2_start[strlen(arg2_start) - 1] == '\"') {
                    arg2_start[strlen(arg2_start) - 1] = 0;
                    arg2_start++;
                }
                writeFile(arg1, arg2_start);
            } else {
                printf("Usage: write <filename> \"content\"\n");
            }
            break;

        case READ:
            if (arg1) readFile(arg1);
            else printf("Usage: read <filename>\n");
            break;

        case DELETE:
            if (arg1) deleteFile(arg1);
            else printf("Usage: delete <filename>\n");
            break;

        case LS:
            showDirectories();
            break;

        case DF:
            showDiskDetials();
            break;

        case EXIT:
            performCleanup();
            printf("Exiting program...\n");
            free(inputCopy);
            return 0;

        default:
            printf("Unknown command. Try: mkdir, cd, create, write, read, delete, ls, rmdir, df, pwd, exit.\n");
            break;
        }
        printf("\n");
        free(inputCopy);
    }
    
    performCleanup();
    return 0;
}
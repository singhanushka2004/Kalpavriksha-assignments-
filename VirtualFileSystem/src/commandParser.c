#include "commandParser.h"
#include "directory.h" 
#include "file.h"      

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern FileNode *root;
extern FileNode *cwd;

CommandType getCommandType(const char *input) {
    if (strncmp(input, "mkdir", 5) == 0) return MKDIR;
    if (strncmp(input, "rmdir", 5) == 0) return RMDIR;
    if (strncmp(input, "cd", 2) == 0) return CD;
    if (strncmp(input, "pwd", 3) == 0) return PWD;
    if (strncmp(input, "create", 6) == 0) return CREATE;
    if (strncmp(input, "write", 5) == 0) return WRITE;
    if (strncmp(input, "read", 4) == 0) return READ;
    if (strncmp(input, "delete", 6) == 0) return DELETE;
    if (strncmp(input, "ls", 2) == 0) return LS;
    if (strncmp(input, "df", 2) == 0) return DF;
    if (strncmp(input, "exit", 4) == 0) return EXIT;
    return INVALID;
}

void showPrompt() {
    if (cwd == root) {
        printf("/> ");
    } else {
        char *path = (char *)calloc(sizeof(char), 256);
        if (path == NULL) { printf("%s > ", cwd->name); return; }
        
        FileNode *temp = cwd;
        
        while (temp && temp->parent) {
            char *tmp = (char *)calloc(sizeof(char), 64);
            char *newPath = (char *)calloc(sizeof(char), 256);

            if (tmp == NULL || newPath == NULL) {
                free(path); free(tmp); free(newPath);
                printf("%s > ", cwd->name);
                return;
            }

            strcpy(tmp, "/");
            strcat(tmp, temp->name);
            
            strcpy(newPath, tmp);
            strcat(newPath, path);
            
            free(path);
            path = newPath;
            free(tmp);

            temp = temp->parent;
        }
        
        printf("%s> ", path);
        free(path);
    }
}
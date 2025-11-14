#include "commandParser.h"
#include "directory.h" 
#include "file.h"      
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char* extractQuotedText(const char* source) {
    const char* firstQuote = strchr(source, '"');
    if (firstQuote == NULL) {
        const char* p = source;
        while (*p && isspace((unsigned char)*p)) ++p;
        char* copy = strdup(p);
        if (copy == NULL) return NULL;
        size_t lenCopy = strlen(copy);
        while (lenCopy > 0 && isspace((unsigned char)copy[lenCopy - 1])) { copy[lenCopy - 1] = '\0'; --lenCopy; }
        return copy;
    }
    const char* secondQuote = strchr(firstQuote + 1, '"');
    if (secondQuote == NULL) secondQuote = source + strlen(source);
    size_t length = (size_t)(secondQuote - (firstQuote + 1));
    char* out = (char*) malloc(length + 1);
    if (out == NULL) return NULL;
    memcpy(out, firstQuote + 1, length);
    out[length] = '\0';
    return out;
}

static void executeCommand(const char* inputLine) {
    char commandWord[128] = {0};
    if (sscanf(inputLine, "%127s", commandWord) <= 0) return;
    const char* afterCommand = inputLine + strlen(commandWord);
    while (*afterCommand && isspace((unsigned char)*afterCommand)) ++afterCommand;

    if (strcmp(commandWord, "exit") == 0) {
        printf("Memory released. Exiting program...\n");
        exit(0);
    } else if (strcmp(commandWord, "mkdir") == 0) {
        char nameArg[MAX_NAME_LEN + 1] = {0};
        if (sscanf(afterCommand, "%50s", nameArg) == 1) cmdMkdir(nameArg);
        else printf("Usage: mkdir <dirname>\n");
    } else if (strcmp(commandWord, "create") == 0) {
        char nameArg[MAX_NAME_LEN + 1] = {0};
        if (sscanf(afterCommand, "%50s", nameArg) == 1) cmdCreate(nameArg);
        else printf("Usage: create <filename>\n");
    } else if (strcmp(commandWord, "cd") == 0) {
        char nameArg[MAX_NAME_LEN + 1] = {0};
        if (sscanf(afterCommand, "%50s", nameArg) == 1) cmdCd(nameArg);
        else printf("Usage: cd <dirname>\n");
    } else if (strcmp(commandWord, "ls") == 0) {
        cmdLs();
    } else if (strcmp(commandWord, "pwd") == 0) {
        cmdPwd();
    } else if (strcmp(commandWord, "df") == 0) {
        cmdDf();
    } else if (strcmp(commandWord, "rmdir") == 0) {
        char nameArg[MAX_NAME_LEN + 1] = {0};
        if (sscanf(afterCommand, "%50s", nameArg) == 1) cmdRmdir(nameArg);
        else printf("Usage: rmdir <dirname>\n");
    } else if (strcmp(commandWord, "delete") == 0) {
        char nameArg[MAX_NAME_LEN + 1] = {0};
        if (sscanf(afterCommand, "%50s", nameArg) == 1) cmdDelete(nameArg);
        else printf("Usage: delete <filename>\n");
    } else if (strcmp(commandWord, "read") == 0) {
        char nameArg[MAX_NAME_LEN + 1] = {0};
        if (sscanf(afterCommand, "%50s", nameArg) == 1) cmdRead(nameArg);
        else printf("Usage: read <filename>\n");
    } else if (strcmp(commandWord, "write") == 0) {
        char nameArg[MAX_NAME_LEN + 1] = {0};
        int consumed = 0;
        if (sscanf(afterCommand, "%50s%n", nameArg, &consumed) >= 1) {
            char* rawContent = extractQuotedText(afterCommand + consumed);
            if (rawContent == NULL) printf("Usage: write <filename> \"content\"\n");
            else { cmdWrite(nameArg, rawContent); free(rawContent); }
        } else printf("Usage: write <filename> \"content\"\n");
    } else {
        printf("Unknown command: %s\n", commandWord);
    }
}

static void promptLoop(void) {
    char line[4096];
    while (1) {
        FileNode* cwd = vfsGetCwd();
        if (cwd == NULL || strcmp(cwd->name, "/") == 0) printf("/ > ");
        else printf("%s > ", cwd->name);
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        size_t lenLine = strlen(line);
        while (lenLine > 0 && (line[lenLine - 1] == '\n' || line[lenLine - 1] == '\r')) { line[lenLine - 1] = '\0'; --lenLine; }
        char* cursor = line;
        while (*cursor && isspace((unsigned char)*cursor)) ++cursor;
        if (*cursor == '\0') continue;
        executeCommand(cursor);
    }
}

void runCli(void) { 
    printf("Compact VFS - ready. Type 'exit' to quit.\n\n");
    promptLoop();
}
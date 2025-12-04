#include "directory.h"
#include "file.h" 

FileNode *root = NULL;
FileNode *cwd = NULL;

FileNode *findChildByName(FileNode *directoryNode, const char *nameToFind) {
    if (directoryNode == NULL || !directoryNode->child) return NULL;
    
    FileNode *walker = directoryNode->child;
    do {
        if (strcmp(walker->name, nameToFind) == 0)
            return walker;
        walker = walker->next;
    } while (walker != directoryNode->child);
    return NULL;
}

bool doesDirectoryExist(const char *name) {
    return findChildByName(cwd, name) != NULL;
}

static void printFullPath(FileNode *directory) {
    const size_t maxDepth = 1024;
    const char *pathStack[maxDepth];
    size_t depth = 0;
    FileNode *walker = directory;
    
    while (walker != NULL && walker->parent != NULL) {
        pathStack[depth++] = walker->name;
        walker = walker->parent;
        if (depth >= maxDepth) break;
    }
    
    if (depth == 0) {
        printf("/\n");
        return;
    }
    
    putchar('/');
    for (ssize_t idx = (ssize_t)depth - 1; idx >= 0; --idx) {
        printf("%s", pathStack[idx]);
        if (idx > 0) putchar('/');
    }
    putchar('\n');
}

static void insertChildNode(FileNode *parentDir, FileNode *childNode) {
    childNode->parent = parentDir;
    childNode->next = childNode->prev = NULL;
    
    if (parentDir->child == NULL) {
        parentDir->child = childNode;
        childNode->next = childNode->prev = childNode;
    } else {
        FileNode *head = parentDir->child;
        FileNode *tail = head->prev;
        
        tail->next = childNode;
        childNode->prev = tail;
        childNode->next = head;
        head->prev = childNode;
    }
}

static void unlinkChildNode(FileNode *parentDir, FileNode *childNode) {
    if (parentDir == NULL || childNode == NULL || parentDir->child == NULL) return;
    
    if (childNode->next == childNode) {
        parentDir->child = NULL;
    } else {
        childNode->prev->next = childNode->next;
        childNode->next->prev = childNode->prev;
        if (parentDir->child == childNode) {
            parentDir->child = childNode->next;
        }
    }
    childNode->next = childNode->prev = NULL;
}

static void destroySingleNode(FileNode *node) {
    if (node == NULL) return;
    if (node->blockPointers != NULL) free(node->blockPointers);
    if (node->name != NULL) free(node->name);
    free(node);
}

bool initializeFileNode(const char *name, const int isDirectory) {
    if (doesDirectoryExist(name)) return false;
    
    FileNode *newDir = (FileNode *)malloc(sizeof(FileNode));
    if (!newDir) { printf("Memory allocation failed for new node.\n"); return false; }
    
    newDir->name = (char *)malloc(strlen(name) + 1);
    if (!newDir->name) { free(newDir); printf("Memory allocation failed for node name.\n"); return false; }
    strcpy(newDir->name, name);
    
    newDir->isDirectory = isDirectory;
    newDir->child = NULL;
    newDir->blockCount = 0;
    newDir->blockPointers = NULL;
    newDir->size = 0;
    
    insertChildNode(cwd, newDir);
    
    return true;
}

void makeDirectory(const char *name) {
    if (name == NULL || name[0] == '\0') {
        printf("Invalid directory name.\n");
    } else if (initializeFileNode(name, 1)) {
        printf("Directory '%s' created successfully\n", name);
    }
}

void removeDirectory(const char *name) {
    FileNode *target = findChildByName(cwd, name);

    if (target == NULL || !target->isDirectory) {
        printf("Directory '%s' not found \n", name);
    } else if (target->child != NULL) {
        printf("Directory '%s' is not empty \n", name);
    } else {
        unlinkChildNode(cwd, target);
        destroySingleNode(target);
        printf("Directory '%s' removed successfully \n", name);
    }
}

void showDirectories() {
    FileNode *temp = cwd->child;
    if (!temp) {
        printf("(empty)\n");
    } else {
        FileNode *head = temp;
        do {
            printf("%s%s\n", temp->name, temp->isDirectory ? "/" : "");
            temp = temp->next;
        } while (temp != head);
        printf("\n");
    }
}

void changeDirectory(const char *name) {
    if (strcmp(name, "..") == 0) {
        if (cwd->parent != NULL) {
            cwd = cwd->parent;
            printf("Moved to %s\n", (cwd == root) ? "/" : cwd->name);
        } else {
            printf("Already at root.\n");
        }
    } else if (strcmp(name, "/") == 0) {
        cwd = root;
        printf("Moved to /\n");
    } else {
        FileNode *target = findChildByName(cwd, name);
        if (target != NULL && target->isDirectory) {
            cwd = target;
            printf("Moved to /%s\n", cwd->name);
        } else {
            printf("Directory '%s' not found \n", name);
        }
    }
}

void showCurrentPath() {
    if (!cwd) {
        printf("Error: current working directory is NULL \n");
        return;
    }
    printFullPath(cwd);
}

void freeFileTree(FileNode *node) {
    if (node) {
        if (node->child) {
            FileNode *child = node->child;
            FileNode *start = child;
            do {
                FileNode *next = child->next;
                freeFileTree(child);
                child = next;
            } while (child != start);
        }
        
        if (node->blockPointers) free(node->blockPointers);
        if (node->name) free(node->name);
        free(node);
    }
}

void initializeRootDirectory() {
    root = (FileNode *)malloc(sizeof(FileNode));
    if (!root) { printf("Memory allocation failed for root directory.\n"); exit(1); }
    
    root->name = (char *)malloc(2);
    strcpy(root->name, "/");
    
    root->isDirectory = 1;
    root->parent = NULL;
    root->next = root->prev = root;
    root->child = NULL;
    root->size = 0;
    root->blockCount = 0;
    root->blockPointers = NULL;
    
    cwd = root;
    printf("Root directory created: '/'\n");
}

void exitProgram() {
    printf("Virtual File System closed. All memory freed.\n");
}

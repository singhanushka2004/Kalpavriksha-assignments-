#ifndef VFS_DIRECTORY_H
#define VFS_DIRECTORY_H

#include "file.h"

void cmdMkdir(const char *dirname);
void cmdCd(const char *dirname);
void cmdLs(void);
void cmdPwd(void);
void cmdRmdir(const char *dirname);
FileNode *vfsGetCwd(void);

#endif
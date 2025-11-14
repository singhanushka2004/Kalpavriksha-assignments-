#include "file.h"          
#include "commandParser.h" 
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int configuredBlocks = 1024;
    if (argc >= 2) {
        int parsed = atoi(argv[1]);
        if (parsed > 0) configuredBlocks = parsed;
    }

    vfsInitialize(configuredBlocks);
    runCli();
    vfsShutdown();

    return 0;
}
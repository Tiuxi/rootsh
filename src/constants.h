#ifndef ROOTSH_CONSTANTS
#define ROOTSH_CONSTANTS

#define ROOTSH_MAX_ARG_LENGTH 100
#define ROOTSH_MAX_ERROR_LENGTH 256
#define PATHVAR "PATH"

// Syscall checks
#include <stdio.h>

#define ASSERT(op)           \
    if(op == -1) {          \
        fprintf(stderr, "Error: line %d, file \"%s\"\n", __LINE__, __FILE__);   \
        fflush(stderr);     \
        exit(EXIT_FAILURE); \
    }

#endif
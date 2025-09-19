#ifndef PLUSH_CONSTANTS
#define PLUSH_CONSTANTS

// globals
#define PLUSH_BASE_COMMAND_LENGTH 128
#define PLUSH_MAX_ARG_LENGTH 100
#define PLUSH_MAX_ERROR_LENGTH 256
#define VAR_ENVPATH "PATH"
#define DEFAULT_PATH "/bin:/usr/bin"
#define VAR_WORKINGDIR "PWD"
#define VAR_HOME "HOME"

// inputs globals
#define SOH 001 /* Start of Header      (ctrl + a)                                  */
#define ETX 003 /* End Of Text          (ctrl + c)                                  */
#define EOT 004 /* End Of Transmition   (ctrl + d)                                  */
#define NL  012 /* New line             (ctrl + j or "return" in cooked mode)       */
#define CR  015 /* Cariage return       (ctrl + m or "return" in raw mode)          */
#define DC2 022 /* Device Control 2     (ctrl + r)                                  */
#define NAK 025 /* Negative Acknowledge (ctrl + u)                                  */
#define SUB 032 /* Substitue            (ctrl + z)                                  */
#define ESC 033 /* Escape               (used in front of other chars)              */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Syscall checks
#include <stdio.h>

#define ASSERT(op)           \
    if((op) == FALSE) {          \
        fprintf(stderr, "Error: line %d, file \"%s\"\n", __LINE__, __FILE__);   \
        fflush(stderr);     \
        exit(EXIT_FAILURE); \
    }

#define max(n1, n2) ((n1 > n2) ? n1 : n2)

#endif
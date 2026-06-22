#ifndef PLUSH_HISTORY
#define PLUSH_HISTORY

#include "utils/constants.h"
#include "utils/error.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PATH_HISTDIR ".config/plush"
#define PATH_HISTFILE "plush_history"
#define MOD_HISTDIR 0755
#define MOD_HISTFILE 0664

/**
 * Check if the directory for the history exist
 */
void plushHistory_check_dir();

/**
 * Load the history file, or create it if doesn't exist.  
 * Also allocate and create the history variable
 */
void plushHistory_load_file();

/**
 * Free the history variable
 */
void plushHistory_destroy_history();

/**
 * Add a command to the history.
 * If the previous command is the same, it doesn't add it
 * 
 * @param command   The command to add to the history
 */
void plushHistory_add_command(const char* command);

/**
 * Save current history to the history file in the .config folder
 */
void plushHistory_save_to_file();

// history global variables
typedef struct s_hist {
    int fd;
    char** hist;
    int index;
} History;

extern History history; /* history global variable */
extern unsigned int HISTORY_SIZE; /* definition in history.c */
extern int HISTORY_currentCommandIndex;
extern bool isHistoryActivated;

#endif /* PLUSH_HISTORY */
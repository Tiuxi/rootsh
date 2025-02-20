#ifndef ROOTSH_EXECCOMMAND
#define ROOTSH_EXECCOMMAND

#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "constants.h"
#include "parseInput.h"
#include "list.h"
#include "error.h"


/**
 * Execute the command `command` with the rootsh shell, and print the result
 * into the standar output
 * 
 * @param command       The command to execute (as a string)
 */
void rootshExec_execute_command(char* command);

#endif
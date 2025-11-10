#ifndef PLUSH_EXECCOMMAND
#define PLUSH_EXECCOMMAND

#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "utils/constants.h"
#include "input/parseInput.h"
#include "utils/list.h"
#include "utils/error.h"
#include "exec/builtIn.h"


/**
 * Execute the command `command` with the plush shell, and print the result
 * into the standar output
 * 
 * @param command       The command to execute (as a string)
 */
void plushExec_execute_command(uchar* command);

#endif
#ifndef PARSEINPUT
#define PARSEINPUT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "constants.h"
#include "error.h"

#define ISREDIRECT(arg) (strncmp((char *)arg->v, ">", NSH_MAX_ARG_LENGTH) == 0) || (strncmp((char *)arg->v, "<", NSH_MAX_ARG_LENGTH) == 0) || (strncmp((char *)arg->v, ">>", NSH_MAX_ARG_LENGTH) == 0)

/**
 * Split a string into an array of string by separating at every spaces
 *
 * @short Split a string into an array
 * @param command       The string to split
 * @return An array of string without spaces
 */
List nshInput_splitInput(char* command);

/**
 * Check in the List "command" if there are redirection and they are correctly made, if not return error message in argument `error`
 * 
 * @param command       The list of argument to check
 * @param error         Error message if 0 is returned, `NULL` else
 * @return 1 if the command is correctly redirected, 0 else
 */
int nshInput_checkRedirect(List command, Error error);

/**
 * Check if the command sent is a file (start with `~`, `.` or `/`)
 * 
 * @param command       The command parsed into a list
 * @return 1 if the command is a file, 0 else
 */
int nshInput_isFile(List command);

#endif
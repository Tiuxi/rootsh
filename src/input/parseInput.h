#ifndef PLUSH_PARSEINPUT
#define PLUSH_PARSEINPUT

#include <string.h>
#include <stdlib.h>
#include "utils/list.h"
#include "utils/constants.h"
#include "utils/error.h"



    /************************ MACROS ************************/


/** 
 * REDIRECTIONS :
 * 
 * <   : stdin
 * >   : stdout
 * >+  : stdout (append mode)
 * >>  : stderr
 * >>+ : stderr (append mode)
 */

#define ISSTDIN(arg) \
    (strncmp((char *)arg->v, "<", strlen((char *)arg->v)) == 0)

#define ISSTDOUT(arg) \
    (strncmp((char *)arg->v, ">", strlen((char *)arg->v)) == 0) || \
    (strncmp((char *)arg->v, ">+", strlen((char *)arg->v)) == 0)

#define ISSTDERR(arg) \
    (strncmp((char *)arg->v, ">>", strlen((char *)arg->v)) == 0) || \
    (strncmp((char *)arg->v, ">>+", strlen((char *)arg->v)) == 0) \

#define ISREDIRECT(arg) \
    ISSTDERR(arg) || ISSTDOUT(arg) || ISSTDIN(arg)


/**
 * /  : root directory
 * .  : current directory
 * .. : parrent directory
 * ~  : home directory
 */
#define ISFILE(command) \
    (((char *)command->v)[0] == '/' || \
    ((char *)command->v)[0] == '~' || \
    ((char *)command->v)[0] == '.')



    /************************ FUNCTIONS ************************/


/**
 * Split a string into an list of every command by separating at every pipe.
 * Each command is a list of char separated by spaces
 *
 * @short Split a string into an array
 * @param command       The string to split
 * @return The list of commands
 */
List plushInput_splitInput(char *command);

/**
 * Check in the List "command" if there are redirection and they are correctly made, if not return error message in argument `error`
 * 
 * @param command       The list of argument to check
 * @param error         Error message if -1 is returned, `NULL` else
 * @return 0 if the command is correctly redirected, -1 else
 */
int plushInput_checkRedirect(List command, Error error);

#endif
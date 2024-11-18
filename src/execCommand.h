#ifndef EXECCOMMAND
#define EXECCOMMAND

#include <stdio.h>
#include "parseInput.h"
#include "list.h"
#include "error.h"

/**
 * Execute the command `command` with the nsh shell, and print the result
 * into the standar output
 * 
 * @param command       The command to execute (as a string)
 */
void nshExec_execute_command(char* command);

#endif
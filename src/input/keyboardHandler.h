#ifndef PLUSH_KEYBOARDHANDLER
#define PLUSH_KEYBOARDHANDLER

#include "utils/constants.h"
#include "hist/history.h"
#include "exec/execCommand.h"
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

extern struct termios previous_config; /* save previous terminal config */
extern char RUNNING;

/**
 * Disable terminal "raw mode". 
 * 
 * See plushKH_enable_raw_mode for more details
 * 
 * @see `plushKH_enable_raw_mode()`
 */
void plushKH_disable_raw_mode();

/**
 * Enable terminal "raw mode" :  
 * - Disable line buffering
 * - Disable echo of keypress
 */
void plushKH_enable_raw_mode();

/**
 * Read a single character from the STDIN stream.
 * 
 * @return The char read
 */
char plushKH_get_char();

/**
 * Main loop of the shell. Reads character while "running" is true, and prints them.  
 * Treats all the escape characters, and send command to executer on return press.
 * 
 * Assume terminal is already in raw mode, and doesn't disable it on exit
 * @see `plushKH_enable_raw_mode()`
 * @return `0` if no problems, else `1` 
 */
int plushKH_main_loop();

#endif /* PLUSH_KEYBOARDHANDLER */
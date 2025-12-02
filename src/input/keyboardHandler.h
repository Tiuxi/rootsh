#ifndef PLUSH_KEYBOARDHANDLER
#define PLUSH_KEYBOARDHANDLER

#include "utils/constants.h"
#include "hist/history.h"
#include "exec/execCommand.h"
#include "utils/signalHandler.h"
#include "output/theme.h"
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ioctl.h>

extern struct termios previous_config; /* save previous terminal config */
extern bool RUNNING;
extern int W_LINE;
extern int W_COLUMN;

/**
 * Disable terminal "raw mode". 
 * 
 * See plushKH_enable_raw_mode for more details
 * 
 * @see `plushKH_enable_raw_mode()`
 */
void plushKH_disable_raw_mode();

/**>
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
uchar plushKH_get_char();

/**
 * Main loop of the shell. Reads character while "running" is true, and prints them.  
 * Treats all the escape characters, and send command to executer on return press.
 * 
 * Assume terminal is already in raw mode, and doesn't disable it on exit
 * @see `plushKH_enable_raw_mode()`
 * @return `0` if no problems, else `1` 
 */
int plushKH_main_loop();

/**
 * Erase the current command typed on the terminal based on the terminal
 * number of line and row
 * 
 * @param cursorIndex   The current index of the cursor in the buffer
 */
void plushKH_erase_current_command(int cursorIndex);

/**
 * Check if the window size has changed, and if so update the global variables
 */
#define plushKH_check_window_resize()          \
    if (SIG_hasWindowChanged) {                \
        struct winsize ws;                     \
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); \
        W_COLUMN = ws.ws_col;                  \
        W_LINE = ws.ws_row;                    \
        SIG_hasWindowChanged = 0;              \
    }

#endif /* PLUSH_KEYBOARDHANDLER */
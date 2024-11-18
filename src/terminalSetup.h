#ifndef TERMCLASS
#define TERMCLASS

#include <gtk/gtk.h>
#include "appClass.h"
#include "terminalViewText.h"
#include "keyHandler.h"
#include "raler.h"

#define APPID "io.gitlab.Tiuxi.nssssh"

/**
 * Is called every time the the user press `return` or `enter` on the KeyPad.  
 * Append the text inside the buffer to the viewText, and then call the
 * function inside the shell
 * 
 * @param widget        The application where the command is sent
 * @param user_data     The data of the current application, with a buffer and a VT
 */
void nshTerminal_command_is_sent(GtkWidget* widget, gpointer user_data);

/**
 * Create the terminal window and call "setup_terminal"
 * 
 * @return  The terminal window as a GtkApplication
*/
GtkApplication* create_terminal();

#endif
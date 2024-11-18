#ifndef KEYHANDLER
#define KEYHANDLER

#include <gtk/gtk.h>
#include "terminalViewText.h"

void terminal_keypressed(GtkWidget* widget, GdkEventKey* event, gpointer data);

#endif
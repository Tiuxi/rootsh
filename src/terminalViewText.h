#ifndef TERMVIEWTEXT
#define TERMVIEWTEXT

#include <gtk/gtk.h>
#include "appClass.h"

void Vt_append_text(GtkApplication* app, char* text);
void Vt_print_hello(GtkWidget *widget, gpointer data);

#endif
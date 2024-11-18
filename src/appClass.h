#ifndef APPCLASS
#define APPCLASS

#include <gtk/gtk.h>

typedef struct _AppData {
    GtkApplication* terminal;
    GtkWidget* terminalText;
    GtkWidget* scrolledWindow;
    GtkWidget* terminalEntry;
    char* typingBuffer;
} AppData;

#define APPDATA(data) (AppData*)(data)

#endif
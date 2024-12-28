#ifndef ROOTSH_GUI
#define ROOTSH_GUI

#include <ncursesw/ncurses.h>

WINDOW* rootshGui_initCurses();

void rootshGui_closeCurses();

void rootshGui_refresh();

#endif
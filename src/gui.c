#include "gui.h"

WINDOW* scrollableWindow;

WINDOW *rootshGui_initCurses() {
    initscr();
    keypad(stdscr, TRUE);

    scrollableWindow = newwin(100,COLS,0,0);
    scrollok(scrollableWindow, TRUE);

    for (int i=0; i<100; i++) {
        wprintw(scrollableWindow, "%d\n", i);
    }

    wrefresh(scrollableWindow);
    return scrollableWindow;
}

void rootshGui_refresh() {
    wrefresh(scrollableWindow);
}

void rootshGui_closeCurses() {
    endwin();
}
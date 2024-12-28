#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "parseInput.h"
#include "list.h"
#include "error.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0); // Hide cursor

    // Get screen dimensions
    int screen_height = LINES;
    int screen_width = COLS;

    // Define border and initial text window dimensions
    int border_height = 3;                               // Top and bottom border height
    int border_width = 3;                                // Left and right border width
    int text_height = screen_height - 2 * border_height; // Initial text window height
    int text_width = screen_width - 2 * border_width;    // Text window width

    // Create the border windows (top, bottom, left, right)
    WINDOW *top_border = newwin(border_height, screen_width, 0, 0);                                // Top border
    WINDOW *bottom_border = newwin(border_height, screen_width, screen_height - border_height, 0); // Bottom border
    WINDOW *left_border = newwin(screen_height, border_width, 0, 0);                               // Left border
    WINDOW *right_border = newwin(screen_height, border_width, 0, screen_width - border_width);    // Right border

    // Create the central text window (initially occupying full height)
    WINDOW *text_window = newwin(text_height, text_width, border_height, border_width);

    // Enable scrolling in the text window
    scrollok(text_window, TRUE);

    // Print content into the text window and make it scrollable
    char line[256];
    for (int i = 0; i < 100; i++)
    {
        snprintf(line, sizeof(line), "This is line number %d", i + 1);
        wprintw(text_window, "%s\n", line);
        wrefresh(text_window); // Refresh the text window to show new content
        napms(100);            // Delay for demo purposes
    }

    // Resize the text window to occupy half of the terminal height (upon "open file" or similar action)
    text_height = (screen_height - 2 * border_height) / 2; // Resize to half the height
    wresize(text_window, text_height, text_width);         // Resize the text window
    mvwin(text_window, border_height, border_width);       // Reposition the text window

    // Create a new window for the bottom half
    WINDOW *bottom_window = newwin(screen_height - 2 * border_height - text_height, text_width, border_height + text_height, border_width);
    wrefresh(bottom_window); // Refresh the bottom window
    scrollok(bottom_window, TRUE);

    // Print some content into the new bottom window (for demo purposes)
    for (int i = 0; i < 50; i++)
    {
        snprintf(line, sizeof(line), "Bottom window, line number %d", i + 1);
        wprintw(bottom_window, "%s\n", line);
        wrefresh(bottom_window); // Refresh the bottom window to show new content
        napms(100);              // Delay for demo purposes
    }

    // Refresh the borders
    wrefresh(top_border);
    wrefresh(bottom_border);
    wrefresh(left_border);
    wrefresh(right_border);

    // Wait for user input before closing
    wgetch(text_window);

    // Clean up
    endwin();
}

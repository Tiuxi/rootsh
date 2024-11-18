#include <gtk/gtk.h>
#include "terminalSetup.h"
#include "terminalViewText.h"

#include <stdio.h>
#include "parseInput.h"
#include "list.h"
#include "error.h"

int main (int argc, char** argv) {

    // ####################  GUI  ####################
    if (argc == 1 || argv[1][0] == '0') {
        // create window
        GtkApplication *terminal = create_terminal();

        // run app
        int status = g_application_run(G_APPLICATION(terminal), argc, argv);

        // when app exited, free everything
        g_object_unref(terminal);

        return status;
    }

    // ####################  SHELL  ####################
    else if (argv[1][0] == '1') {
        // pass compilation
        (void) argc;
        (void) argv;

        int running = 1;
        char buffer[100];
        int index = 0;
        Error error = nshError_new_error();

        while (running) {
            char c = getchar();

            if (c==10) {
                buffer[index] = '\0';

                List entry = nshInput_splitInput(buffer);
                nshList_printListString(entry);
                if (!nshInput_checkRedirect(entry, error)) {
                    nshError_print_error(error);
                }

                if (((char*)(entry->v))[0] == '*' && ((char*)(entry->v))[0] == '*') {
                    running = 0;
                }

                nshList_destroyAll(entry);

                index = 0;
            }else {
                buffer[index] = c;
                index++;
            }
        }

        nshError_destroy_error(error);

        return 0;
    }

    return 0;
}

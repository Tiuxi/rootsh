#include <stdio.h>
#include <string.h>
#include "parseInput.h"
#include "list.h"
#include "error.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    int running = 1;
    char buffer[100];
    int index = 0;
    Error error = rootshError_new_error();

    while (running) {
        char c = getchar();

        if (c==10) {
            buffer[index] = '\0';
            List commands = rootshInput_splitInput(buffer);

            for (List command=commands; command!=NULL; command=command->next) {
                List entry = command->v;

                if (!strcmp(((char*)(entry->v)),"quit")) {
                    running = 0;
                    break;
                }

                rootshList_printListString(entry);
                if (!rootshInput_checkRedirect(entry, error)) {
                    rootshError_print_error(error);
                }

                index = 0;
            }

            rootshList_destroy2DListAll(commands);
        }else {
            buffer[index] = c;
            index++;
        }
    }

    rootshError_destroy_error(error);

    return 0;
}

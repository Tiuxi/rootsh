#include <stdio.h>
#include "constants.h"
#include "parseInput.h"
#include "error.h"
#include "execCommand.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    int running = 1;
    char buffer[ROOTSH_MAX_COMMAND_LENGTH];
    int index = 0;
    Error error = rootshError_new_error();

    while (running) {
        char c = getchar();

        if (c==10) {
            
            buffer[index] = '\0';
            rootshExec_execute_command(buffer);
            index=0;

        }else {
            buffer[index] = c;
            index++;
        }
    }

    rootshError_destroy_error(error);

    return 0;
}

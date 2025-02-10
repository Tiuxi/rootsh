#include <stdio.h>
#include <string.h>
#include "parseInput.h"
#include "list.h"
#include "error.h"
#include "execCommand.h"

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

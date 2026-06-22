#include <stdio.h>
#include "utils/constants.h"
#include "parse/parseInput.h"
#include "utils/error.h"
#include "exec/execCommand.h"
#include "hist/history.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    int running = 1;
    char buffer[PLUSH_BASE_COMMAND_LENGTH];
    int index = 0;

    // history
    plushHistory_check_dir();
    plushHistory_load_file();
    
    putchar('$'); putchar(' ');
    while (running) {
        char c = getchar();

        switch (c) {

        // "RETURN"
        case KEY_RETURN:

            // if command is empty, skip
            if (index!=0) {
                buffer[index] = '\0';
                if (strncmp(buffer, "exit", PLUSH_BASE_COMMAND_LENGTH) == 0)
                    running = FALSE;
                else {
                    plushHistory_add_command(buffer);
                    plushExec_execute_command(buffer);
                }
                index = 0;
            }
            putchar('$'); putchar(' ');

            break;
        
        // ctrl + d
        case EOF:
            running = FALSE;
            if(write(STDOUT_FILENO, "\nexiting\n", 10)){;}
            break;
        

        default:
            buffer[index] = c;
            index++;
            break;
        }
    }

    plushHistory_save_to_file();
    plushHistory_destroy_history();

    return 0;
}

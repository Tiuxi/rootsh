#include <stdio.h>
#include "hist/history.h"
#include "input/inputHandler.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    // history
    plushHistory_check_dir();
    plushHistory_load_file();
    
    PlushInput_main_loop();

    plushHistory_destroy_history();

    return 0;
}

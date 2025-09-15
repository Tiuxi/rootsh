#include <stdio.h>
#include <stdlib.h>
#include "utils/error.h"
#include "hist/history.h"
#include "input/keyboardHandler.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    // history
    plushHistory_check_dir();
    plushHistory_load_file();

    // save & free on exit
    // function are put in stack, so put destroy first and save after
    atexit(plushHistory_destroy_history);
    atexit(plushHistory_save_to_file);

    // raw mode
    plushKH_enable_raw_mode();
    atexit(plushKH_disable_raw_mode);

    // enter main loop
    int return_code = plushKH_main_loop();

    return return_code;
}

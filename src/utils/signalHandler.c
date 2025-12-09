#include "signalHandler.h"

volatile sig_atomic_t SIG_hasWindowChanged = 0;

void windowChanged_handler(int sig) {
    (void)sig;
    SIG_hasWindowChanged = 1;
    return;
}

void plushSig_set_handler() {
    
    // Window changed
    struct sigaction s_winch;
    s_winch.sa_handler = windowChanged_handler;
    s_winch.sa_flags = 0;
    sigemptyset(&s_winch.sa_mask);

    if (sigaction(SIGWINCH, &s_winch, NULL) == -1)
        plushError_print_new_warn("SIGWINCH : Signal handler couldn't be set");
}
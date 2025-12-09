#ifndef PLUSH_SIGNAL_HANDLER
#define PLUSH_SIGNAL_HANDLER

#include <signal.h>
#include <unistd.h>
#include "utils/error.h"

extern volatile sig_atomic_t SIG_hasWindowChanged;

void plushSig_set_handler();

#endif /* PLUSH_SIGNAL_HANDLER */
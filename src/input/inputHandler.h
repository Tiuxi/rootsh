#ifndef PLUSH_INPUTHANDLER
#define PLUSH_INPUTHANDLER

#include "utils/constants.h"
#include "input/linenoise.h"
#include "hist/history.h"
#include "exec/execCommand.h"

#include <sys/select.h>
#include <errno.h>

extern bool PlushInput_MainLoopRunning;

int PlushInput_main_loop();

#endif /* PLUSH_INPUT_HANDLER */
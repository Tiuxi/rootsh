#include "inputHandler.h"

bool PlushInput_MainLoopRunning = FALSE;

void load_hist_to_linenoise() {
    int index = (history.index + 1) % HISTORY_SIZE;

    while (index != history.index) {
        if (history.hist[index] != NULL)
            linenoiseHistoryAdd(history.hist[index]);

        index = (index + 1) % HISTORY_SIZE;
    }
}

int PlushInput_main_loop() {
    PlushInput_MainLoopRunning = TRUE;
    char* commandBuffer;

    linenoiseHistorySetMaxLen(HISTORY_SIZE);
    load_hist_to_linenoise();

    while (PlushInput_MainLoopRunning) {
        commandBuffer = linenoise("> ");

        if (commandBuffer != NULL && commandBuffer[0] != '\0') {
            plushHistory_add_command(commandBuffer);
            linenoiseHistoryAdd(commandBuffer);

            plushExec_execute_command(commandBuffer);
        }

        if (commandBuffer != NULL)
            linenoiseFree(commandBuffer);
        else {
            PlushInput_MainLoopRunning = FALSE;
        }
    }

    return 0;
}
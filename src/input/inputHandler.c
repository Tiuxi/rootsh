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

void PlushInput_get_input_loop() {
    struct linenoiseState lstate;
    char buffer[2048];
    char* command;

    while (PlushInput_MainLoopRunning) {

        linenoiseEditStart(&lstate, -1, -1, buffer, sizeof(buffer), "> ");
        bool isLineEditFinished = FALSE;

        while (!isLineEditFinished) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(lstate.ifd, &readfds);

            struct timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            
            int returnValue = select(lstate.ifd+1, &readfds, NULL, NULL, &tv);

            if (returnValue == -1) {
                plushError_print_new_error("select syscall failed");
                exit(1);
            }
            else if (returnValue != 0) { // char inputed
                command = linenoiseEditFeed(&lstate);

                // line editing is finished
                if (command != linenoiseEditMore) isLineEditFinished = TRUE;
            }
            else { // timeout call
                /*
                linenoiseHide(&lstate);

                linenoiseShow(&lstate);
                */
            }
        }

        linenoiseEditStop(&lstate);

        if (command == NULL) {
            switch (errno) {
                case EAGAIN: // ctrl+C
                    puts("\e[1A\e[2K\e[1A");
                    break;

                case ENOENT:  // ctrl+d
                    PlushInput_MainLoopRunning = FALSE;
                    break;
                
                default:
                    plushError_print_new_error("failed getting user input");
                    exit(1);
                    break;
            }
        } else if (command[0] != '\0'){
            plushHistory_add_command(command);
            linenoiseHistoryAdd(command);

            plushExec_execute_command(command);
        }

        if (command != NULL) linenoiseFree(command);
    }
}

int PlushInput_main_loop() {
    PlushInput_MainLoopRunning = TRUE;
    //char* commandBuffer;

    linenoiseHistorySetMaxLen(HISTORY_SIZE);
    load_hist_to_linenoise();

    /* while (PlushInput_MainLoopRunning) {
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
    } */
    PlushInput_get_input_loop();

    return 0;
}
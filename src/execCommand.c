#include "execCommand.h"

void nshExec_execute_command(char* command) {
    Error error = nshError_new_error();

    // parse the command
    List entry = nshInput_splitInput(command);

    // Check redirection
    if (!nshInput_checkRedirect(entry, error)) {
        nshError_print_error(error);
    }

    // Check files

    // Check command

    // Execute command
    nshList_printListString(entry);

    // free everything
    nshList_destroyAll(entry);
    nshError_destroy_error(error);

    return;
}
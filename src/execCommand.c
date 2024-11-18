#include "execCommand.h"

void rootshExec_execute_command(char* command) {
    Error error = rootshError_new_error();

    // parse the command
    List entry = rootshInput_splitInput(command);

    // Check redirection
    if (!rootshInput_checkRedirect(entry, error)) {
        rootshError_print_error(error);
    }

    // Check files

    // Check command

    // Execute command
    rootshList_printListString(entry);

    // free everything
    rootshList_destroyAll(entry);
    rootshError_destroy_error(error);

    return;
}
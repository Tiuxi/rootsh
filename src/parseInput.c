#include "parseInput.h"

List rootshInput_splitInput(char* command) {
    int commandLength = strlen(command);

    char* s = (char*)malloc(sizeof(char) * ROOTSH_MAX_ARG_LENGTH);
    List argList = rootshList_new(s); // list with current command arguments
    List commandList = rootshList_new(argList); // list with every command
    List n = argList; // list element of current argument

    int currentIndex = 0;
    for (int i=0; i<commandLength; i++) {

        // if pipe, separate in a new command
        if (currentIndex == 0 && command[i] == '|') {
            if (currentIndex == 0) {
                rootshList_pop(argList);
            }else {
                ((char *)(n->v))[currentIndex] = '\0';
                currentIndex = 0;
            }

            s = (char*)malloc(sizeof(char) * ROOTSH_MAX_ARG_LENGTH);
            argList = rootshList_new(s);
            commandList = rootshList_push(commandList, argList);
            n = argList;

            while (i < commandLength && (command[i] == '|' || command[i] == ' ')){
                i++;
            }
        }
        
        // slice by spaces
        if (command[i] == ' ') {
            ((char*)(n->v))[currentIndex] = '\0';
            currentIndex = 0;
            s = (char*)malloc(sizeof(char) * ROOTSH_MAX_ARG_LENGTH);
            argList = rootshList_push(argList, s);
            n = n->next;
        }

        // else put the char at the end of the string
        else {
            if (currentIndex < ROOTSH_MAX_ARG_LENGTH-1) {
                ((char *)(n->v))[currentIndex] = command[i]; // copy every char
                currentIndex++;
            }
        }
    }
    ((char*)(n->v))[currentIndex] = '\0';

    return commandList;
}

int rootshInput_checkRedirect(List command, Error error) {
    List tmp = command;

    // if redirection before command, raise error
    if (ISREDIRECT(tmp)) {
        rootshError_set_error_with_argument(error, "Redirection is made before command", tmp->v);
        return -1;
    }

    int redirections[3] = {0, 0, 0};

    for (tmp=command; tmp!=NULL; tmp=tmp->next) {

        // is a redirection
        if (ISREDIRECT(tmp)) {

            // if there is nothing after or another redirection, raise error
            if (tmp->next == NULL || ISREDIRECT(tmp->next)) {
                rootshError_set_error_with_argument(error, "No file specified for redirection", tmp->v);
                return -1;
            }

            // check type of redirection, and make sure there's not the same redirection more than once
            if (ISSTDIN(tmp)) {
                if (redirections[0] != 0) {
                    rootshError_set_error_with_argument(error, "Redirecting the stream STDIN more than once", tmp->v);
                    return -1;
                }
                redirections[0]++;
            } else if (ISSTDOUT(tmp)) {
                if (redirections[1] != 0) {
                    rootshError_set_error_with_argument(error, "Redirecting the stream STDOUT more than once", tmp->v);
                    return -1;
                }
                redirections[1]++;
            } else if (ISSTDERR(tmp)) {
                if (redirections[2] != 0) {
                    rootshError_set_error_with_argument(error, "Redirecting the stream STDERR more than once", tmp->v);
                    return -1;
                }
                redirections[2]++;
            }

        }
    }

    return 0;
}

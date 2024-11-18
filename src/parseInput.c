#include "parseInput.h"

List nshInput_splitInput(char* command) {
    int commandLength = strlen(command);

    char* s = (char*)malloc(sizeof(char) * NSH_MAX_ARG_LENGTH);
    List argList = nshList_push(NULL, s);
    int currentIndex = 0;

    List n = argList;
    for (int i=0; i<commandLength; i++) {
        
        // slice by spaces
        if (command[i] == ' ') {
            ((char*)(n->v))[currentIndex] = '\0';
            currentIndex = 0;
            s = (char*)malloc(sizeof(char) * NSH_MAX_ARG_LENGTH);
            argList = nshList_push(argList, s);
            n = n->next;
        }

        // else put the char at the end of the string
        else {
            if (currentIndex != NSH_MAX_ARG_LENGTH-1) {
                ((char *)(n->v))[currentIndex] = command[i]; // copy every char
                currentIndex++;
            }
        }
    }
    ((char*)(n->v))[currentIndex] = '\0';

    return argList;
}

int nshInput_checkRedirect(List command, Error error) {
    List tmp = command;

    // if redirection before command, raise error
    if (ISREDIRECT(tmp)) {
        nshError_set_error_with_argument(error, "Syntax error : redirection before command", tmp->v);
        return 0;
    }

    for (tmp=command; tmp!=NULL; tmp=tmp->next) {

        // is a redirection
        if (ISREDIRECT(tmp)) {

            // if there is nothing after, raise error
            if (tmp->next == NULL) {
                nshError_set_error_with_argument(error, "Syntax error : no file specified for redirection", tmp->v);
                return 0;
            }

            // if there is another redirection right after, raise error

            // if redirection already exist, raise error

        }
    }

    return 1;
}

int nshInput_isFile(List command) {
    return (((char *)command->v)[0] == '/' ||
            ((char *)command->v)[0] == '~' ||
            ((char *)command->v)[0] == '.');
}
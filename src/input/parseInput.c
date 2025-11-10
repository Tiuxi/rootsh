#include "input/parseInput.h"

List plushInput_splitInput(uchar* command) {
    int commandLength = strlen((char*)command);

    char* s = (char*)malloc(sizeof(char) * (commandLength+1));
    List argList = plushList_new(s); // list with current command arguments
    List commandList = plushList_new(argList); // list with every command
    List n = argList; // list element of current argument

    int currentIndex = 0;
    for (int i=0; i<commandLength; i++) {

        // if pipe, separate in a new command
        if (command[i] == '|') {
            if (currentIndex == 0 && plushList_size(argList) != 1) {
                argList = plushList_pop(argList);
            }else {
                ((char *)(n->v))[currentIndex] = '\0';
                currentIndex = 0;
            }

            s = (char*)malloc(sizeof(char) * commandLength);
            argList = plushList_new(s);
            commandList = plushList_push(commandList, argList);
            n = argList;

            i++;
            while (i < commandLength && command[i] == ' '){
                i++;
            }
            i--;
        }
        
        // slice by spaces
        else if (command[i] == ' ') {
            if (currentIndex == 0) {
                continue;
            } else {
                ((char*)(n->v))[currentIndex] = '\0';
                currentIndex = 0;
            }

            s = (char*)malloc(sizeof(char) * commandLength);
            argList = plushList_push(argList, s);
            n = n->next;
        }

        // if escape character, put next char in string
        else if (command[i] == '\\') {
            ((char *)(n->v))[currentIndex] = command[i+1];
            currentIndex++;
            i++;
        }

        // if double quotes, switch to input mode (no special character)
        else if (command[i] == '\"') {
            i++;
            while (command[i] != '\"') {
                // if end of string, throw error
                if (command[i] == '\0') {
                    plushError_print_new_error("\" not closed");
                    plushList_destroy2DListAll(commandList);
                    return NULL;
                } 
                // if escape character
                else if (command[i] == '\\') {
                    // if special character, escape it, else add "\"
                    switch (command[i+1]) {
                        case '\\':
                            ((char *)(n->v))[currentIndex] = '\\';
                            break;
                        case '\"':
                            ((char *)(n->v))[currentIndex] = '\"';
                            break;
                        
                        default:
                            ((char *)(n->v))[currentIndex] = '\\';
                            i--;
                            break;
                    }
                    currentIndex++;
                    i+=2;
                } else {
                    ((char *)(n->v))[currentIndex] = command[i]; 
                    currentIndex++;
                    i++;
                }
            }
        }

        // else put the char at the end of the string
        else {
            ((char *)(n->v))[currentIndex] = command[i]; // copy every char
            currentIndex++;
        }
    }
    ((char*)(n->v))[currentIndex] = '\0';

    return commandList;
}

int plushInput_checkRedirect(List command, Error error) {
    List tmp = command;

    // if redirection before command, raise error
    if (ISREDIRECT(tmp)) {
        plushError_set_error_with_argument(error, "Redirection is made before command", tmp->v);
        return -1;
    }

    int redirections[3] = {0, 0, 0};

    for (tmp=command; tmp!=NULL; tmp=tmp->next) {

        // is a redirection
        if (ISREDIRECT(tmp)) {

            // if there is nothing after or another redirection, raise error
            if (tmp->next == NULL || ISREDIRECT(tmp->next)) {
                plushError_set_error_with_argument(error, "No file specified for redirection", tmp->v);
                return -1;
            }

            // check type of redirection, and make sure there's not the same redirection more than once
            if (ISSTDIN(tmp)) {
                if (redirections[0] != 0) {
                    plushError_set_error_with_argument(error, "Redirecting the stream STDIN more than once", tmp->v);
                    return -1;
                }
                redirections[0]++;
            } else if (ISSTDOUT(tmp)) {
                if (redirections[1] != 0) {
                    plushError_set_error_with_argument(error, "Redirecting the stream STDOUT more than once", tmp->v);
                    return -1;
                }
                redirections[1]++;
            } else if (ISSTDERR(tmp)) {
                if (redirections[2] != 0) {
                    plushError_set_error_with_argument(error, "Redirecting the stream STDERR more than once", tmp->v);
                    return -1;
                }
                redirections[2]++;
            }

        }
    }

    return 0;
}

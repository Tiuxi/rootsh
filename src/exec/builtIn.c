#include "builtIn.h"

int plushBuiltin_check_builtin(List cmd) {
    char* command = cmd->v;
    int commandLength = strlen(command);

    if (!strncmp(command, "cd", commandLength)) {
        char* newPWD = NULL;

        // 1 argument ("cd"), return to home folder
        if (plushList_size(cmd) < 2) {
            newPWD = getenv(VAR_HOME);
            if (newPWD == NULL || newPWD[0] == '\0')
                plushError_print_new_warn("$HOME not initialized");
        }

        // 2 arguments ("cd", "dir")
        else if (plushList_size(cmd) == 2) {
            newPWD = (char*)cmd->next->v;
        } 
        
        // 3+ arguments, throw error
        else {
            plushError_print_new_error("cd : too many arguments");
        }
        
        if (newPWD != NULL && newPWD[0] != '\0') {
            if (chdir(newPWD)==-1) {
                switch (errno) {
                case ENOENT:
                    {
                        Error err = plushError_new_error();
                        
                        plushError_set_error_with_argument(err, "No directory named", newPWD);
                        plushError_print_error(err);
                        plushError_destroy_error(err);
                    }

                    break;
                
                default:
                    {
                        char err[26];
                        snprintf(err, 26, "Unkown error. errno : %d", errno);
                        plushError_print_new_error(err);
                    }
                    break;
                }
            };
        }
        return TRUE;
    }

    if (!strncmp(command, "history", commandLength)) {
        if (!isHistoryActivated) return TRUE;
        int index = (history.index+1) % HISTORY_SIZE;

        while (index != history.index) {
            if (history.hist[index] != NULL) {
                write(STDOUT_FILENO, history.hist[index], strlen(history.hist[index]));
                write(STDOUT_FILENO, "\n", 2);
            }

            index = (index+1) % HISTORY_SIZE;
        }

        return TRUE;
    }

    return FALSE;
}
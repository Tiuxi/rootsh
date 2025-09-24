#include "exec/execCommand.h"

/**
 * Get every executables directories from the environement variable "PATH"
 */
List getEnvironementsDir()
{
    char *pathstr = getenv(VAR_ENVPATH);
    if (pathstr == NULL || pathstr[0] == '\0') {
        plushError_print_new_warn("$PATH not initialized. Initializing to \"/bin:/usr/bin\"");
        if (setenv(VAR_ENVPATH, DEFAULT_PATH, 1) == -1) {
            plushError_print_new_error("Critical : could not set $PATH");
            ASSERT(FALSE);
        }
        pathstr = DEFAULT_PATH;
    }

    int lenPath = strlen(pathstr);

    char *currentDir = (char *)malloc(sizeof(char) * PATH_MAX);
    List paths = plushList_new(currentDir);

    int currentDirIndex = 0;
    for (int i = 0; i < lenPath; i++)
    {
        if (pathstr[i] == ':')
        {
            currentDir[currentDirIndex] = '\0';
            currentDir = (char *)malloc(sizeof(char) * PATH_MAX);
            plushList_push(paths, currentDir);
            currentDirIndex = 0;
        }
        else
        {
            currentDir[currentDirIndex] = pathstr[i];
            currentDirIndex++;
        }
    }

    return paths;
}

/**
 * Check in every directory of environement variable "PATH" if the command "command" is in it
 * 
 * @param command The name of the command to search
 * @param paths The list of every paths from the "PATH" env var (in the form of a `plushList`)
 * @return The full path to the command
 */
char* get_single_command_from_env_paths(char* command, List paths) {
    char* executablePath = (char*)malloc(sizeof(char) * PATH_MAX);
    int found = 0;

    DIR* dir;

    // open every "PATH" directories
    List currentDir = paths;
    while (currentDir != NULL && !found) {
        // open dir
        dir = opendir(currentDir->v);
        if (dir==NULL) {
            // if dir not found, skip to next dir
            if (errno == ENOENT) {
                currentDir = currentDir->next;
                continue;
            }

            // uknown error, stop
            else {
                printf("Errno : %d\n", errno);
                ASSERT(FALSE);
            }
        }

        struct dirent* file = readdir(dir);

        // check for every file in directory
        while (file != NULL && !found) {

            // check file name
            if (strncmp(file->d_name, command, NAME_MAX) == 0){
                found = 1;
                snprintf(executablePath, PATH_MAX, "%s/%s", (char*)currentDir->v, file->d_name);
            }

            // swap to next file in directory
            errno = 0;
            file = readdir(dir);
        }

        ASSERT(closedir(dir) != -1);

        currentDir = currentDir->next;
    }

    if (found==0) {
        free(executablePath);
        return NULL;
    }
    
    return executablePath;
}

// main function
void plushExec_execute_command(char* commandStr) {
    List paths = getEnvironementsDir();

    // parse the command
    List commands = plushInput_splitInput(commandStr);

    for (List command=commands; command!=NULL; command=command->next) {

        // check if no command given in input
        int size = plushList_size(command->v);
        if (size == 0 || (size == 1 && ((char*)((List)command->v)->v)[0] == '\0')) {
            plushError_print_new_error("No command given");

            // free everything
            plushList_destroy2DListAll(commands);
            plushList_destroyAll(paths);
            return;
        }
        List currentCommand = command->v;
        char* executable = NULL;

        // Check redirection
        Error errorRedirect = plushError_new_error();
        if (plushInput_checkRedirect(currentCommand, errorRedirect) == -1) {
            plushError_print_error(errorRedirect);
            plushError_destroy_error(errorRedirect);

            // free everything
            plushList_destroy2DListAll(commands);
            plushList_destroyAll(paths);
            return;
        }
        plushError_destroy_error(errorRedirect);

        // Check built-in
        if (plushBuiltin_check_builtin(currentCommand)) {
            // free everything
            continue;
        }

        // Check file
        if (ISFILE(currentCommand)) {
            executable = (char*)malloc(sizeof(char) * PLUSH_MAX_ARG_LENGTH);
            snprintf(executable, PLUSH_MAX_ARG_LENGTH, "%s", (char*)currentCommand->v);
        }

        // Check "PATH" executables
        else {
            executable = get_single_command_from_env_paths(currentCommand->v, paths);
        }

        if (executable==NULL) {
            // set error & print it
            Error err = plushError_new_error();
            plushError_set_error_with_argument(err, "Command not found", currentCommand->v);
            plushError_print_error(err);
            plushError_destroy_error(err);

            // free everything
            plushList_destroy2DListAll(commands);
            plushList_destroyAll(paths);
            return;
        }

        // execute command in a child process
        switch (fork()) {
            // fork error
            case -1:
                plushError_print_new_error("CRITICAL : fork error");
                return;
                break;
            
            // child process
            case 0:
                int nbArguments = plushList_size(currentCommand);
                char** arguments = (char**)malloc(sizeof(char*) * (nbArguments+1));

                // set argument list
                List tmp = currentCommand;
                for (int i=0; i<nbArguments; i++) {
                    arguments[i] = tmp->v;
                    tmp = tmp->next;
                }
                arguments[nbArguments] = (char*)NULL; // required for execv

                // execute command
                int exitStatus = execv(executable, arguments);

                // free value
                plushList_destroy2DListAll(commands);
                plushList_destroyAll(paths);
                free(executable);
                free(arguments);

                if (exitStatus == -1)
                    exit(errno);
                else
                    exit(0);
                break;
            
            // main process
            default:
                break;
        }

        // wait for the child process to execute
        int childStatus;
        ASSERT(wait(&childStatus) != -1);

        // free the path of the executable
        free(executable);
    }

    // free everything
    plushList_destroy2DListAll(commands);
    plushList_destroyAll(paths);

    return;
}

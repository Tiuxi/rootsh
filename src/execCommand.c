#include "execCommand.h"

/**
 * Get every executables directories from the environement variable "PATH"
 */
List getEnvironementsDir()
{
    char *pathstr = getenv(PATHVAR);
    if (pathstr == NULL)
        ASSERT(-1);

    int lenPath = strlen(pathstr);

    char *currentDir = (char *)malloc(sizeof(char) * PATH_MAX);
    List paths = rootshList_new(currentDir);

    int currentDirIndex = 0;
    for (int i = 0; i < lenPath; i++)
    {
        if (pathstr[i] == ':')
        {
            currentDir[currentDirIndex] = '\0';
            currentDir = (char *)malloc(sizeof(char) * PATH_MAX);
            rootshList_push(paths, currentDir);
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

void rootshExec_execute_command(char* command) {
    Error error = rootshError_new_error();
    List paths = getEnvironementsDir();

    // parse the command
    List commands = rootshInput_splitInput(command);

    // Check redirection
    for (List command=commands; command!=NULL; command=command->next) {
        List entry = command->v;

        rootshList_printListString(entry);
        //rootshList_printListString(entry);
        if (rootshInput_checkRedirect(entry, error) == -1) {
            rootshError_print_error(error);
        }
    }

    // Check files

    // Execute command
    rootshList_printListString(paths);

    // free everything
    rootshList_destroy2DListAll(commands);
    rootshError_destroy_error(error);

    return;
}

#include "history.h"

History history;
unsigned int HISTORY_SIZE = 1000;
bool isHistoryActivated = TRUE;
int HISTORY_currentCommandIndex = 1;

void plushHistory_check_dir() {

    const char* envHome = getenv(VAR_HOME);
    if (envHome == NULL || envHome[0] == '\0') {
        plushError_print_new_warn("$HOME not initialized. History will not be activated");
        isHistoryActivated = FALSE;
        return;
    }

    char* histPath = (char*)malloc(sizeof(char)*FILENAME_MAX);
    snprintf(histPath, FILENAME_MAX, "%s/%s", envHome, PATH_HISTDIR);

    DIR* histDir = opendir(histPath);

    if (histDir) { // directory exist
        closedir(histDir);
    } else if (errno == ENOENT) { // directory doesn't exist

        if(mkdir(histPath, MOD_HISTDIR) != 0) {
            plushError_print_new_error("Cannont create history directory");
            isHistoryActivated = FALSE;
        }

    } else { // unknown error
        plushError_print_new_error("Cannot open history directory");
        printf("%d\n", errno);
        isHistoryActivated = FALSE;
    }

    free(histPath);

    return;
}

void plushHistory_load_file() {
    if (!isHistoryActivated) return;

    const char* envHome = getenv(VAR_HOME);
    if (envHome == NULL || envHome[0] == '\0')
        return;
    char* histFilePath = (char*)malloc(sizeof(char)*FILENAME_MAX);
    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", envHome, PATH_HISTDIR, PATH_HISTFILE);

    history.fd = open(histFilePath, O_CREAT | O_RDONLY, MOD_HISTFILE);
    history.index = 0;
    history.hist = (char**)malloc(sizeof(char*) * HISTORY_SIZE);
    ASSERT(history.hist != NULL);
    for (unsigned int i=0; i<HISTORY_SIZE; i++) history.hist[i] = NULL;

    int commandLength = PLUSH_BASE_COMMAND_LENGTH;
    history.hist[history.index] = (char*)malloc(commandLength * sizeof(char));
    ASSERT(history.hist[history.index] != NULL);
    memset(history.hist[history.index], 0, commandLength);

    ssize_t bytes_reads, buff_size=64;
    char* buffer = (char*)malloc(sizeof(char) * buff_size);
    ASSERT(buffer != NULL);

    // read all file content
    int currentIndex = 0;
    while ((bytes_reads = read(history.fd, buffer, buff_size)) > 0){

        // copy into hist buffer
        for (int i=0; i<bytes_reads; i++) {
            // new command
            if (buffer[i] == '\n') {
                history.hist[history.index][currentIndex] = '\0';
                history.index = (history.index+1) % HISTORY_SIZE;
                printf("Loaded command %d : %s\n", history.index, history.hist[history.index-1]);

                // free if already allocated (circular buffer)
                if (history.hist[history.index] != NULL)
                    free(history.hist[history.index]);

                commandLength = PLUSH_BASE_COMMAND_LENGTH;
                history.hist[history.index] = (char*)malloc(commandLength * sizeof(char));
                ASSERT(history.hist[history.index] != NULL);
                memset(history.hist[history.index], 0, commandLength);
                currentIndex = 0;
            } else {
                if (currentIndex >= commandLength-1) {
                    commandLength *= 2;
                    history.hist[history.index] = realloc(history.hist[history.index], commandLength * sizeof(char));
                    ASSERT(history.hist[history.index] != NULL);
                }
                history.hist[history.index][currentIndex] = buffer[i];
                currentIndex++;
            }
        }
    }
    if (bytes_reads == -1) {
        plushError_print_new_warn("Error while reading the history file");
    }

    if (currentIndex != 0) {
        history.hist[history.index][currentIndex] = '\0';
        history.index = (history.index + 1) % HISTORY_SIZE;
        printf("Loaded command %d : %s\n", history.index, history.hist[history.index - 1]);

        if (history.hist[history.index] != NULL)
            free(history.hist[history.index]);

        commandLength = PLUSH_BASE_COMMAND_LENGTH;
        history.hist[history.index] = (char*)malloc(commandLength * sizeof(char));
        ASSERT(history.hist[history.index] != NULL);
        memset(history.hist[history.index], 0, commandLength);
        currentIndex = 0;
    }

    free(buffer);
    free(histFilePath);
    close(history.fd);

    HISTORY_currentCommandIndex = history.index;

    return;
}

void plushHistory_destroy_history() {
    if (!isHistoryActivated) return;
    
    for (unsigned int i=0; i<HISTORY_SIZE; i++) {
        if (history.hist[i] != NULL)
            free(history.hist[i]);
    }

    free(history.hist);

    return;
}

void plushHistory_add_command(const char* command) {
    if (!isHistoryActivated) return;
    size_t commandLen = strlen(command);

    // check if same command than before
    char* previousCommand = history.hist[(history.index - 1 + HISTORY_SIZE) % HISTORY_SIZE];
    if (previousCommand != NULL && 
        !strcmp(previousCommand, (char*)command)) {

        return;
    }

    if (commandLen >= PLUSH_BASE_COMMAND_LENGTH)
        history.hist[history.index] = realloc(history.hist[history.index], commandLen+1);
    memcpy(history.hist[history.index], command, commandLen);
    history.hist[history.index][commandLen] = '\0';

    int oldIndex = history.index;
    history.index = (history.index+1) % HISTORY_SIZE;
    if (history.hist[history.index] != NULL)
        free(history.hist[history.index]);
        
    history.hist[history.index] = (char*)malloc(PLUSH_BASE_COMMAND_LENGTH * sizeof(char));
    memset(history.hist[history.index], 0, PLUSH_BASE_COMMAND_LENGTH);

    // append to history file
    const char* envHome = getenv(VAR_HOME);
    if (envHome == NULL || envHome[0] == '\0') {
        plushError_print_new_warn("$HOME not set, could not save history");
        return;
    }

    char* histFilePath = (char*)malloc(sizeof(char) * FILENAME_MAX);
    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", envHome, PATH_HISTDIR, PATH_HISTFILE);

    history.fd = open(histFilePath, O_WRONLY | O_CREAT, MOD_HISTFILE);

    if (write(history.fd, history.hist[oldIndex], strlen(history.hist[oldIndex])) < 0
            || write(history.fd, "\n", 1) < 0) {
        plushError_print_new_warn("Cannot write to history file");
    }

    close(history.fd);
    free(histFilePath);

    return;
}

void plushHistory_save_to_file() {
    if (!isHistoryActivated) return;

    const char* envHome = getenv(VAR_HOME);
    if (envHome == NULL || envHome[0] == '\0') {
        plushError_print_new_warn("$HOME not set, could not save history");
        return;
    }

    char* histFilePath = (char*)malloc(sizeof(char) * FILENAME_MAX);
    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", envHome, PATH_HISTDIR, PATH_HISTFILE);
    
    history.fd = open(histFilePath, O_TRUNC | O_WRONLY | O_CREAT, MOD_HISTFILE);

    int index = (history.index + 1) % HISTORY_SIZE;

    while (index != history.index) {
        if (history.hist[index] != NULL) {
            if (write(history.fd, history.hist[index], strlen(history.hist[index])) < 0
            || write(history.fd, "\n", 1) < 0) {
                plushError_print_new_warn("Cannot write to history file");
                break;
            }
        }

        index = (index + 1) % HISTORY_SIZE;
    }

    close(history.fd);
    free(histFilePath);

    return;
}
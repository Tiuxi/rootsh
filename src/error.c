#include "error.h"

Error rootshError_new_error() {
    Error err = (Error)malloc(sizeof(char*));
    return err;
}

void rootshError_destroy_error(Error err) {
    free(err);
}

void rootshError_destroy_error_message(Error err) {
    free(*err);
}

void rootshError_set_error_message(Error err, const char *message) {
    char* tmp = (char*)malloc(sizeof(char)*ROOTSH_MAX_ERROR_LENGTH);
    snprintf(tmp, ROOTSH_MAX_ERROR_LENGTH, "%s\n", message);
    *err = tmp;
}

void rootshError_set_error_with_argument(Error err, const char* message, char* arg) {
    char* tmp = (char*)malloc(sizeof(char)*ROOTSH_MAX_ERROR_LENGTH);
    snprintf(tmp, ROOTSH_MAX_ERROR_LENGTH, "%s : \"%s\"\n", message, arg);
    *err = tmp;
}

void rootshError_print_error(Error err) {
    printf("%s", *err);
    rootshError_destroy_error_message(err);
}

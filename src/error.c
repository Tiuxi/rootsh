#include "error.h"

Error nshError_new_error() {
    Error err = (Error)malloc(sizeof(char*));
    return err;
}

void nshError_destroy_error(Error err) {
    free(err);
}

void nshError_destroy_error_message(Error err) {
    free(*err);
}

void nshError_set_error_message(Error err, const char *message) {
    char* tmp = (char*)malloc(sizeof(char)*NSH_MAX_ERROR_LENGTH);
    snprintf(tmp, NSH_MAX_ERROR_LENGTH, "%s\n", message);
    *err = tmp;
}

void nshError_set_error_with_argument(Error err, const char* message, char* arg) {
    char* tmp = (char*)malloc(sizeof(char)*NSH_MAX_ERROR_LENGTH);
    snprintf(tmp, NSH_MAX_ERROR_LENGTH, "%s : \"%s\"\n", message, arg);
    *err = tmp;
}

void nshError_print_error(Error err) {
    printf("%s", *err);
    nshError_destroy_error_message(err);
}

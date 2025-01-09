#ifndef ROOTSH_ERROR
#define ROOTSH_ERROR

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "color.h"

typedef char** Error;

Error rootshError_new_error();

void rootshError_destroy_error(Error err);

void rootshError_destroy_error_message(Error err);

void rootshError_set_error_message(Error err, const char *message);

void rootshError_set_error_with_argument(Error err, const char *message, char *arg);

void rootshError_print_error(Error err);

#endif
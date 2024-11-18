#ifndef NSH_ERROR
#define NSH_ERROR

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

typedef char** Error;

Error nshError_new_error();

void nshError_destroy_error(Error err);

void nshError_destroy_error_message(Error err);

void nshError_set_error_message(Error err, const char *message);

void nshError_set_error_with_argument(Error err, const char *message, char *arg);

void nshError_print_error(Error err);

#endif
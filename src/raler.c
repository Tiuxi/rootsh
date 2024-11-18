#include "raler.h"

void raler(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}
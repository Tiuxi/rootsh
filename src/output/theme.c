#include "theme.h"

char* userPrompt1 = "$ ";
int userPrompt1Size = 2;
char* userPrompt2 = "> ";
int userPrompt2Size = 2;

void plushTheme_print_prompt_1() {
    write(STDOUT_FILENO, userPrompt1, userPrompt1Size);
}

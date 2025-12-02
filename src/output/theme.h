#ifndef PLUSH_THEME
#define PLUSH_THEME

#include <unistd.h>

extern char* userPrompt1;
extern int userPrompt1Size;
extern char* userPrompt2;
extern int userPrompt2Size;

void plushTheme_print_prompt_1();

#endif /* PLUSH_THEME */
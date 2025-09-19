#include "input/keyboardHandler.h"

struct termios previous_config;
char RUNNING;

void plushKH_disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &previous_config);
    return;
}

void plushKH_enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &previous_config);
    
    struct termios raw = previous_config;
    // Input modes: no break, no CR to NL, no parity check, no strip char,
    // no start/stop output control.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    // Output modes: disable post processing
    // raw.c_oflag &= ~(OPOST);

    // Control modes: set 8 bit chars
    raw.c_cflag |= (CS8);

    // Local modes: echoing off, canonical off, no extended functions,
    // no signal chars (Ctrl+C, Ctrl+Z, etc.)
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // Control chars: return each byte, or timeout
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char plushKH_get_char() {
    char c;
    while (!(read(STDIN_FILENO, &c, 1)>0));
    return c;
}

int plushKH_main_loop() {
    RUNNING = TRUE;

    // input buffer + cursor localisation
    char buffer[PLUSH_BASE_COMMAND_LENGTH];
    int bufferIndex = 0;
    int cursorIndex = 0; (void)cursorIndex;

    // print user prompt
    write(STDOUT_FILENO, "$ ", 2);

    while (RUNNING) {
        char c = plushKH_get_char();

        switch (c) {
            // "RETURN"
            case NL:
            case CR:
                write(STDOUT_FILENO, "\n", 1);

                // if command is empty, skip
                if (bufferIndex != 0) {
                    buffer[bufferIndex] = '\0';
                    if (strncmp(buffer, "exit", PLUSH_BASE_COMMAND_LENGTH) == 0)
                        RUNNING = FALSE;
                    else {
                        plushHistory_add_command(buffer);
                        plushExec_execute_command(buffer);
                    }
                    bufferIndex = 0;
                }
                
                write(STDOUT_FILENO, "$ ", 2);

                break;

            // ctrl + d
            case EOF:
            case EOT:
                RUNNING = FALSE;
                if (write(STDOUT_FILENO, "\nexiting\n", 10)) {
                    ;
                }
                break;

            // ctrl + c
            case ETX:
                break;

            // ctrl + z
            case SUB:
                break;

            // escape sequence
            case ESC:
                c = plushKH_get_char();

                switch(c) {
                    default:
                        printf("Sequence : %d\n", c);
                }

                break;

            default:
                if (isprint(c)) {
                    buffer[bufferIndex] = c;
                    bufferIndex++;
                    write(STDOUT_FILENO, &c, 1);
                } else {
                    printf("%o\n", c); // DEBUG ONLY
                }
                break;
        }
    }

    return 0;
}
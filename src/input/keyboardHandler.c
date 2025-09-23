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

uchar plushKH_get_char() {
    uchar c;
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
        uchar c = plushKH_get_char();

        // printable char
        if (isprint(c)) {
            // cursor is not at the end
            if (cursorIndex != bufferIndex)
                for (uint i=bufferIndex+1; i>(uint)cursorIndex; i--)
                    buffer[i] = buffer[i - 1];  // push every other char

            buffer[cursorIndex] = c;
            bufferIndex++;
            cursorIndex++;
            write(STDOUT_FILENO, &c, 1);
        } 

        // other char (control char mainly)
        else {
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
                        bufferIndex = cursorIndex = 0;
                        memset(buffer, 0, sizeof buffer);
                    }

                    write(STDOUT_FILENO, "$ ", 2);

                    break;

                // ctrl + d
                case (uchar)EOF:
                case EOT:
                    RUNNING = FALSE;
                    write(STDOUT_FILENO, "\nexiting\n", 10);
                    break;

                // ctrl + c
                case ETX:
                    break;

                // ctrl + z
                case SUB:
                    break;

                // delete
                case DEL:
                    if (cursorIndex==0) break;
                    for (uint i=cursorIndex-1; i < (uint)bufferIndex; i++)
                        buffer[i] = buffer[i+1];  // push every other char
                    
                    cursorIndex--; bufferIndex--;

                    // clear and rewrite after cursor
                    write(STDOUT_FILENO, "\r\e[2K$ ", 7);
                    write(STDOUT_FILENO, buffer, bufferIndex);
                    
                    // move cursor back
                    char cursorOffset[15];
                    if (cursorIndex!=bufferIndex) {
                        snprintf(cursorOffset, 15, "\e[%dD", bufferIndex-cursorIndex);
                        write(STDOUT_FILENO, cursorOffset, strlen(cursorOffset));
                    }

                    break;

                // escape sequence
                case ESC:
                    c = plushKH_get_char();

                    // Control Sequence Introducer
                    if (c=='[') {
                        c = plushKH_get_char();
                        
                        // arg functions
                        if (c >= '0' && c <= '9') {
                            int arguments = 0;
                            
                            // get argument
                            while (c != '~') {
                                arguments = arguments * 10;
                                arguments += c - '0';
                                
                                c = plushKH_get_char();
                            }

                            switch (arguments)
                            {
                            case 3: // delete

                                if (cursorIndex == bufferIndex) break;
                                for (uint i = cursorIndex; i < (uint)bufferIndex; i++)
                                    buffer[i] = buffer[i + 1];  // push every other char

                                bufferIndex--;

                                // clear and rewrite after cursor
                                write(STDOUT_FILENO, "\r\e[2K$ ", 7);
                                write(STDOUT_FILENO, buffer, bufferIndex);

                                // move cursor back
                                char cursorOffset[15];
                                if (cursorIndex != bufferIndex) {
                                    snprintf(cursorOffset, 15, "\e[%dD", bufferIndex - cursorIndex);
                                    write(STDOUT_FILENO, cursorOffset, strlen(cursorOffset));
                                }

                                break;
                            
                            default:
                                break;
                            }

                        }

                        // no arg functions
                        else switch (c) {
                            case 'A': // UP
                                break;
                            case 'B': // DOWN
                                break;

                            case 'C': // RIGHT
                                if (cursorIndex<bufferIndex) {
                                    cursorIndex++;
                                    write(STDOUT_FILENO, "\e[1C", 4);
                                }
                                break;
                            case 'D': // LEFT
                                if (cursorIndex>0) {
                                    cursorIndex--;
                                    write(STDOUT_FILENO, "\e[1D", 4);
                                }
                                break;
                            
                            default:
                                // printf(" :: %d\n", c);
                        }
                    }
                    
                    // unknow sequence
                    else {
                        printf("Sequence : %d\n", c);
                    }

                    break;

                default:
                    printf("%d\n", c);  // DEBUG ONLY
                    break;
            }
        }

        
    }

    return 0;
}
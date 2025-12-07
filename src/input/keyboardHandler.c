#include "input/keyboardHandler.h"
#include "keyboardHandler.h"

struct termios previous_config;
int W_LINE = 1;
int W_COLUMN = 1;
bool RUNNING;

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
    while (!(read(STDIN_FILENO, &c, 1) > 0)) printf("%d\n", c);
    return c;
}

void plushKh_move_cursor(int oldCursorIndex, int newCursorIndex) {
    int oldLine = (int)((oldCursorIndex + userPrompt1Size) / W_COLUMN);
    int newLine = (int)((newCursorIndex + userPrompt1Size) / W_COLUMN);

    // go to correct line
    if (newLine > oldLine)
        printf("\e[%dB", newLine - oldLine);
    else if (oldLine > newLine)
        printf("\e[%dA", oldLine - newLine);
    
    fflush(stdout);
    
    // go to start of line, then to correct row
    write(STDOUT_FILENO, "\r", 1);
    if (((newCursorIndex + userPrompt1Size) % W_COLUMN) != 0) {
        printf("\e[%dC", (int)((newCursorIndex + userPrompt1Size) % W_COLUMN));
        fflush(stdout);
    }
}

/**
 * Handle all control character that is not an escape sequence
 * 
 * @param c                 The character read
 * @param _bufferPt         A pointer to the input buffer
 * @param _bufferIndexPt    A pointer to the buffer index
 * @param _cursorIndexPt    A pointer to the cursor index
 * @param bufferSize        The size of the input buffer
 */
void plushKH_control_char(uchar c, uchar** _bufferPt, int* _bufferIndexPt, int* _cursorIndexPt, int bufferSize) {
    uchar* buffer = *_bufferPt;
    int bufferIndex = *_bufferIndexPt;
    int cursorIndex = *_cursorIndexPt;

    switch (c) {
        // return & enter keys
        case NL:
        case CR:
            write(STDOUT_FILENO, "\n", 1);

            // if command is empty, skip
            if (bufferIndex != 0) {
                buffer[bufferIndex] = '\0';
                if (strncmp((char*)buffer, "exit", bufferSize) == 0)
                    RUNNING = FALSE;
                else {
                    plushHistory_add_command(buffer);
                    plushExec_execute_command(buffer);
                }
                bufferIndex = 0;
                cursorIndex = 0;
                memset(buffer, 0, bufferSize);
            }

            write(STDOUT_FILENO, "$ ", 2);
            currentHistoryIndex = history.index;

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

        // backspace
        case DEL:
            if (cursorIndex == 0) break;
            plushKH_erase_current_command(cursorIndex);

            for (uint i = cursorIndex - 1; i < (uint)bufferIndex; i++)
                buffer[i] = buffer[i + 1];  // push every other char
            
            cursorIndex--;
            bufferIndex--;

            // clear and rewrite after cursor
            plushTheme_print_prompt_1();
            write(STDOUT_FILENO, buffer, bufferIndex);

            // move cursor back
            plushKh_move_cursor(bufferIndex-1, cursorIndex);

            break;

        default:
            printf("%d\n", c);  // DEBUG ONLY
            break;
    }

    *_bufferPt = buffer;
    *_bufferIndexPt = bufferIndex;
    *_cursorIndexPt = cursorIndex;
}

/**
 * Get the arguments of an escape code function
 * 
 * @param arg           A pointer to a non-allocated int array
 * @param currentChar   A pointer to the current character that is being processed
 * 
 * @return The number of argument recognized
 */
int plushKH_get_arguments(int** arg, uchar* currentChar) {
    uchar c = *currentChar;
    bool argEnded = FALSE;
    int nbArg = 1;

    *arg = (int*)malloc(sizeof(int) * nbArg);
    ASSERT((*arg) != NULL);
    int* arguments = *arg;
    arguments[nbArg-1] = 0;

    while (!argEnded) {
        if (c == ';') {
            nbArg++;
            arguments = (int*)realloc(arguments, sizeof(int*) * nbArg);
            ASSERT(arguments != NULL);
            arguments[nbArg-1] = 0;
        } 
        
        else if (c >= '0' && c <= '9') {
            arguments[nbArg - 1] = arguments[nbArg - 1] * 10;
            arguments[nbArg - 1] += c - '0';
        }

        // range for end of control sequence : defined by ECMA 48
        else if (c >= 0x40 && c <= 0x7E) {
            argEnded = TRUE;
            break;
        }

        else {
            plushError_print_new_warn("Unrecognized char in escape sequence");
        }

        c = plushKH_get_char();
    }

    *currentChar = c;
    return nbArg;
}

/**
 * Handle the escape sequences
 *
 * @param _bufferPt         A pointer to the input buffer
 * @param _bufferIndexPt    A pointer to the buffer index
 * @param _cursorIndexPt    A pointer to the cursor index
 */
void plushKH_escape_seqence(uchar** _bufferPt, int* _bufferIndexPt, int* _cursorIndexPt) {
    uchar c = plushKH_get_char();
    uchar* buffer = *_bufferPt;
    int bufferIndex = *_bufferIndexPt;
    int cursorIndex = *_cursorIndexPt;
    int oldCursorIndex = cursorIndex;

    // check if the char is not the Control Sequence Introducer (CSI), return
    if (c != '[') {
        plushError_print_new_warn("Unknown escape sequence");
        printf("    - Sequence : %d\n", c);
        return;
    }

    // get the sequence
    c = plushKH_get_char();

    // ansi escape sequence with arguments
    if (c >= '0' && c <= '9') {
        int* args;
        int nbArgs = plushKH_get_arguments(&args, &c);
        (void)nbArgs;

        switch (args[0]) {
            // home
            case 1:
                // home key
                if (nbArgs == 1 && c == '~') {
                    printf("\nHome key pressed\n");
                }

                // key modifier
                else if (nbArgs == 2) {

                    // shift modifier
                    if (args[1] == 2) {
                        
                    }

                    // alt modifier
                    else if (args[1] == 3) {

                    }

                    // ctrl modifier
                    else if (args[1] == 5) {
                        switch (c) {
                        // RIGHT
                        case 'C':
                            // first, move after every spaces
                            while (cursorIndex < bufferIndex && buffer[cursorIndex] == ' ') {
                                cursorIndex++;
                            }

                            // then, move after the whole word
                            while (cursorIndex < bufferIndex && buffer[cursorIndex] != ' ') {
                                cursorIndex++;
                            }

                            plushKh_move_cursor(oldCursorIndex, cursorIndex);
                            break;

                        // LEFT
                        case 'D':
                            if (cursorIndex != 0) {
                                cursorIndex--;
                            }

                            // first, move after every spaces
                            while (cursorIndex > 0 && buffer[cursorIndex] == ' ') {
                                cursorIndex--;
                            }

                            // then, move after the whole word
                            while (cursorIndex > 0 && buffer[cursorIndex] != ' ') {
                                cursorIndex--;
                            }

                            // if we're not at the start, move to get to the first letter of the word
                            if (cursorIndex != 0) {
                                cursorIndex++;
                            }

                            plushKh_move_cursor(oldCursorIndex, cursorIndex);
                            break;
                        }
                    }

                    else {
                        plushError_print_new_warn("Unknown key modifer in escape sequence");
                    }
                }
                break;

            // insert
            case 2:
                printf("\nInsert key pressed\n");
                break;

            // delete
            case 3:

                if (cursorIndex == bufferIndex) break;

                plushKH_erase_current_command(cursorIndex);
                for (uint i = cursorIndex; i < (uint)bufferIndex; i++)
                    buffer[i] = buffer[i + 1];  // push every other char

                bufferIndex--;

                // clear and rewrite after cursor
                plushTheme_print_prompt_1();
                write(STDOUT_FILENO, buffer, bufferIndex);

                // move cursor back
                plushKh_move_cursor(0, cursorIndex);

                break;

            // end
            case 4:
                printf("\nEnd key pressed\n");
                break;

            // page up
            case 5:
                printf("\nPage up key pressed\n");
                break;

            // page down
            case 6:
                printf("\nPage down key pressed\n");
                break;

            default:
                printf("\nArgument %d \n", args[0]);
                break;
        }
    }
    
    // no arguments
    else switch (c) {
        // UP
        case 'A':
            if (currentHistoryIndex == (int)((history.index + 1) % HISTORY_SIZE)) break;
            if (!history.hist[(currentHistoryIndex + HISTORY_SIZE - 1) % HISTORY_SIZE]) break;

            // if new command, save it for now
            if (currentHistoryIndex == history.index) {
                memcpy(history.hist[history.index], buffer, bufferIndex);
            }
            plushKH_erase_current_command(cursorIndex);
            plushTheme_print_prompt_1();

            // clear buffer
            memset(buffer, 0, bufferIndex + 1);
            currentHistoryIndex = (currentHistoryIndex + HISTORY_SIZE - 1) % HISTORY_SIZE;
            memcpy(buffer, history.hist[currentHistoryIndex], strlen(history.hist[currentHistoryIndex]));
            cursorIndex = bufferIndex = strlen((char*)buffer);

            // re-write new buffer
            write(STDOUT_FILENO, buffer, bufferIndex);

            break;

        // DOWN
        case 'B':
            if (currentHistoryIndex == history.index) break;
            plushKH_erase_current_command(cursorIndex);
            plushTheme_print_prompt_1();

            // clear buffer
            memset(buffer, 0, bufferIndex + 1);
            currentHistoryIndex = (currentHistoryIndex + 1) % HISTORY_SIZE;
            memcpy(buffer, history.hist[currentHistoryIndex], strlen(history.hist[currentHistoryIndex]));
            cursorIndex = bufferIndex = strlen((char*)buffer);

            // re-write new buffer
            write(STDOUT_FILENO, buffer, bufferIndex);

            break;

        // RIGHT
        case 'C':
            if (cursorIndex < bufferIndex)
                cursorIndex++;

            plushKh_move_cursor(oldCursorIndex, cursorIndex);
            break;

        // LEFT
        case 'D':
            if (cursorIndex > 0)
                cursorIndex--;

            plushKh_move_cursor(oldCursorIndex, cursorIndex);
            break;

        // Home
        case 'H':
            plushKh_move_cursor(cursorIndex, 0);
            cursorIndex = 0;
            break;

        // End
        case 'F':
            plushKh_move_cursor(cursorIndex, bufferIndex);
            cursorIndex = bufferIndex;
            break;

        default:
            printf("Sequence %d - %c\n", c, c);
    }

    *_bufferPt = buffer;
    *_bufferIndexPt = bufferIndex;
    *_cursorIndexPt = cursorIndex;
}

int plushKH_main_loop() {
    RUNNING = TRUE;

    // input buffer + cursor localisation
    int commandLengthExtend = 0;
    uchar* buffer = (uchar*)malloc(PLUSH_BASE_COMMAND_LENGTH * sizeof(uchar));
    ASSERT(buffer != NULL);
    memset(buffer, 0, PLUSH_BASE_COMMAND_LENGTH);

    // force window size update
    SIG_hasWindowChanged = 1;
    plushKH_check_window_resize();

    int bufferIndex = 0;
    int cursorIndex = 0;
    (void)cursorIndex;

    write(STDOUT_FILENO, "$ ", 2);

    while (RUNNING) {
        uchar c = plushKH_get_char();

        // c is a printable char or an UTF-8 char
        if (isprint(c) || (c & 0x80) != 0) {
            // if allocated buffer is too small, reallocate
            if (bufferIndex >= (PLUSH_BASE_COMMAND_LENGTH << commandLengthExtend)-1) {
                commandLengthExtend++;
                buffer = realloc(buffer, PLUSH_BASE_COMMAND_LENGTH << commandLengthExtend);
                ASSERT(buffer != NULL);
            }

            // cursor is not at the end
            if (cursorIndex != bufferIndex)
                for (uint i = bufferIndex + 1; i > (uint)cursorIndex; i--)
                    buffer[i] = buffer[i - 1];  // push every other char

            buffer[cursorIndex] = c;
            bufferIndex++;
            cursorIndex++;

            if (cursorIndex != bufferIndex) {
                // clear and rewrite after cursor
                plushKH_erase_current_command(cursorIndex);
                plushTheme_print_prompt_1();
                write(STDOUT_FILENO, buffer, bufferIndex);

                // move cursor back
                plushKh_move_cursor(bufferIndex-1, cursorIndex);
            }
            else 
                write(STDOUT_FILENO, &c, 1);
        }

        // c is a control character that is not an escape sequence
        else if (c != ESC) {
            plushKH_control_char(c, &buffer, &bufferIndex, &cursorIndex, PLUSH_BASE_COMMAND_LENGTH << commandLengthExtend);
        }

        // c is an escape sequence
        else {
            plushKH_escape_seqence(&buffer, &bufferIndex, &cursorIndex);
        }
    }

    free(buffer);
    return 0;
}

void plushKH_erase_current_command(int cursorIndex) {
    int currentLine = (int)( (cursorIndex - 1 + userPrompt1Size) / W_COLUMN );
    
    // get to first line
    if (currentLine != 0) {
        printf("\e[%dA\r", currentLine);
        fflush(stdout);
    } else {
        write(STDOUT_FILENO, "\r", 1);
    }
    
    // erase all after
    write(STDOUT_FILENO, "\e[0J", 4);
}
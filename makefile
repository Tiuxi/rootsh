CC := gcc
CFLAGS := -g -Wall -Werror -Wextra

GUIFLAGS := `pkg-config --cflags gtk+-3.0`
GUILIBS := `pkg-config --libs gtk+-3.0`

SHELLPATH := src/
SHELLEXEC := shellMain

GUIPATH := src/
GUIEXEC := guiMain

.PHONY: default  
default: gui

includePath:
	pkg-config --cflags gtk+-3.0


####################  GUI  ####################

.PHONY: gui gui-run gui-valgrind gui-valgrind-full

gui: $(GUIEXEC)

gui-run: default
	@./$(GUIEXEC)

gui-valgrind: shell
	@valgrind ./$(GUIEXEC)

gui-valgrind-full: shell
	@valgrind --leak-check=full ./$(GUIEXEC)

#	 Get dependancies
GUIOBJECTS := $(patsubst %.c, %.o, $(wildcard $(GUIPATH)*.c))
GUIHEADERS := $(wildcard $(GUIPATH)*.h)

# 	 Compilation
#.PRECIOUS: $(GUIEXEC) $(GUIOBJECTS)
%.o: %.c $(GUIHEADERS)
	@$(CC) $(CFLAGS) $(GUIFLAGS) -c $< -o $@ $(GUILIBS)
$(GUIEXEC): $(GUIOBJECTS)
	@$(CC) $(GUIOBJECTS) $(CFLAGS) $(GUIFLAGS) -o $@ $(GUILIBS)



#################### SHELL ####################

.PHONY: shell shell-run shell-valgrind shell-valgrind-full

shell: $(SHELLEXEC)

shell-run: shell
	@./$(SHELLEXEC) 1

shell-valgrind: shell
	@valgrind ./$(SHELLEXEC)

shell-valgrind-full: shell
	@valgrind --leak-check=full ./$(SHELLEXEC)

#	 Get dependancies
SHELLOBJECTS := $(patsubst %.c, %.o, $(wildcard $(SHELLPATH)*.c))
SHELLHEADERS := $(wildcard $(SHELLPATH)*.h)

# 	 Compilation
#.PRECIOUS: $(SHELLEXEC) $(SHELLOBJECTS)
%.o: %.c $(SHELLHEADERS)
	@$(CC) $(CFLAGS) $(GUIFLAGS) -c $< -o $@ $(GUILIBS)
$(SHELLEXEC): $(SHELLOBJECTS)
	@$(CC) $(SHELLOBJECTS) $(CFLAGS) $(GUIFLAGS) -o $@ $(GUILIBS)



#################### BUILD & CLEAN ####################

.PHONY: clean clean-keep-build build

clean:
	rm -rf $(SHELLPATH)*.o $(SHELLEXEC) $(GUIPATH)*.o $(GUIEXEC) build/

clean-keep-build:
	rm -rf $(SHELLPATH)*.o $(SHELLEXEC) $(GUIPATH)*.o $(GUIEXEC)

build: shell
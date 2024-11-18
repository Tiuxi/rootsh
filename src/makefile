# make sure default action is compilation
.PHONY: default  
default : compile

CC ?= gcc
CFLAGS ?= -g -Wall -Werror -Wextra
LIBS ?= 

# 
ifndef EXEC
EXEC := ../rootsh.out
else
EXEC := ../$(EXEC)
endif

#################### Compile & run ####################

.PHONY: compile run valgrind valgrind-full

compile: $(EXEC)

run: compile
	@./$(EXEC)

valgrind: compile
	@valgrind ./$(EXEC)

valgrind-full: compile
	@valgrind --leak-check=full ./$(EXEC)

#	 Get dependancies
OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS := $(wildcard *.h)

# 	 Compilation
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@ $(LIBS)


#################### Build & Clean ####################

.PHONY: clean clean-keep-build build

clean:
	rm -rf *.o $(EXEC) build/

clean-keep-build:
	rm -rf *.o $(EXEC)

# TODO: make a build process
build: compile
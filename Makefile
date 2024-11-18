# compiler
CC := gcc

# compiler flags
CFLAGS := -g -Wall -Werror -Wextra

# executable name
EXEC := main

export CC CFLAGS LIBS EXEC


############### Targets ###############

.PHONY: default
default:
	@$(MAKE) -C src

.PHONY: clean
clean:
	rm -rf $(EXEC) *.out
	@$(MAKE) clean -C src

.PHONY: build
build:
	@echo "Not implemented yet"
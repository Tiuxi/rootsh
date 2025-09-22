# compiler
CC := gcc

# compiler flags
CFLAGS := -Wall -Wextra -O3

# executable name
EXEC := plush

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

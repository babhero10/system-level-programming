# Compiler and compile flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Directories
SRCDIR = unix_utilities
BINDIR = executables

# List all .c files in the source directory
SOURCES := $(wildcard $(SRCDIR)/*.c)

# Create a list of targets by replacing the source directory and removing the .c extension
TARGETS := $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%, $(SOURCES))

# Default target: build all executables
all: $(TARGETS)

# Rule to compile each .c file into an executable in the BINDIR
$(BINDIR)/%: $(SRCDIR)/%.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

# Ensure the executables directory exists
$(BINDIR):
	mkdir -p $(BINDIR)

# Clean up generated executables
clean:
	rm -rf $(BINDIR)/*

.PHONY: all clean


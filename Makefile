# Compiler and compile flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Directories
UTILS_SRCDIR = unix_utilities
SHELLS_SRCDIR = shells
UTILS_BINDIR = executables/utilies
SHELLS_BINDIR = executables/shells

# List all .c files in the utils and shells source directories
UTILS_SOURCES := $(wildcard $(UTILS_SRCDIR)/*.c)
SHELLS_SOURCES := $(wildcard $(SHELLS_SRCDIR)/*.c)

# Create a list of targets by replacing the source directory and removing the .c extension
UTILS_TARGETS := $(patsubst $(UTILS_SRCDIR)/%.c, $(UTILS_BINDIR)/%, $(UTILS_SOURCES))
SHELLS_TARGETS := $(patsubst $(SHELLS_SRCDIR)/%.c, $(SHELLS_BINDIR)/%, $(SHELLS_SOURCES))

# Default target: build all executables (both utils and shells)
all: $(UTILS_TARGETS) $(SHELLS_TARGETS)

# Rule to compile each .c file into an executable in the appropriate BINDIR
$(UTILS_BINDIR)/%: $(UTILS_SRCDIR)/%.c | $(UTILS_BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(SHELLS_BINDIR)/%: $(SHELLS_SRCDIR)/%.c | $(SHELLS_BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

# Ensure the executables directories exist
$(UTILS_BINDIR):
	mkdir -p $(UTILS_BINDIR)

$(SHELLS_BINDIR):
	mkdir -p $(SHELLS_BINDIR)

# Clean up generated executables
clean:
	rm -rf $(UTILS_BINDIR)/* $(SHELLS_BINDIR)/*

.PHONY: all clean


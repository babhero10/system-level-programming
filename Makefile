# Directories
SHELLS_DIR      := shells
EXECUTABLES_DIR := executables/shells
HELPER_SRC      := helper/source/*.c
UTIL_SRC        := unix_utilities/source/*.c
INCLUDES        := -I helper/include -I unix_utilities/include

# Get a list of all .c files in the shells directory.
SHELLS_SRC      := $(wildcard $(SHELLS_DIR)/*.c)
# Convert each source filename to an executable name in executables/shells.
EXECS           := $(patsubst $(SHELLS_DIR)/%.c, $(EXECUTABLES_DIR)/%, $(SHELLS_SRC))

# Default target.
all: $(EXECUTABLES_DIR) $(EXECS)

# Rule to build an executable from a .c file.
$(EXECUTABLES_DIR)/%: $(SHELLS_DIR)/%.c
	@echo "Building $@..."
	gcc -o $@ $< $(HELPER_SRC) $(UTIL_SRC) $(INCLUDES)

# Ensure the executables directory exists.
$(EXECUTABLES_DIR):
	mkdir -p $(EXECUTABLES_DIR)

# Clean up generated executables.
clean:
	rm -f $(EXECUTABLES_DIR)/*

.PHONY: all clean


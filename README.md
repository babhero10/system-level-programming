# UNIX Utilities

## Project Description
This project implements basic UNIX utility commands including pwd, echo, cp, and mv.

## Prerequisites
- GCC compiler
- Make utility
- Basic UNIX/Linux environment

## Compilation and Usage

### Building the Project
To compile all utilities, run:
```bash
make
```

### Cleaning the Project
To remove compiled executables and object files:
```bash
make clean
```

### Running Utilities
After compilation, you can run the utilities from the `executables/` directory:

- Print Working Directory utility:
  ```bash
  ./executables/pwd
  ```

- Echo utility:
  ```bash
  ./executables/echo [arguments]
  ```

- Copy utility:
  ```bash
  ./executables/cp [source] [destination]
  ```

- Move utility:
  ```bash
  ./executables/mv [source] [destination]
  ```

## Example Usage
```bash
# Compile the project
make

# Print current directory
./executables/pwd

# Display a message
./executables/echo "Hello, UNIX Utilities!"

# Copy a file
./executables/cp source.txt destination.txt

# Move a file
./executables/mv oldname.txt newname.txt
```

## Notes
- Ensure you have executable permissions for the utility files
- Run `chmod +x executables/*` if needed

# Low level programming

## Project Description
This project implements basic UNIX utility commands including pwd, echo, cp, and mv and also implements shells (femto and pico).

## Prerequisites
- GCC compiler
- Make utility
- Basic UNIX/Linux environment

## Compilation and Usage

### Building the Project
To compile all, run:

```bash
make
```

### Cleaning the Project
To remove compiled executables and object files:

```bash
make clean
```

### Running Utilities
Note: The utilities cannot be run directly on their own. They are integrated into the shells and must be used through them.

### Running Shells
After compilation, you can run the shells from the `executables/shells` directory:

#### Femto Shell
```bash
./executables/shells/femto_shell
```
The femto shell is a minimal implementation that can only run three built-in commands:
- `echo [arguments]` - Display text
- `clear` - Clear the terminal screen
- `exit` - Exit the shell

#### Pico Shell
```bash
./executables/shells/pico_shell
```
The pico shell is more advanced and can run most external commands thanks to the execvpe function, but it also includes these built-in utilities:
- `pwd` - Print current working directory
- `echo [arguments]` - Display text
- `cp [source] [destination]` - Copy files
- `mv [source] [destination]` - Move or rename files

## Notes
- Ensure you have executable permissions for all files
- Run `chmod +x executables/shells/*` if needed

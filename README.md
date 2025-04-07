# Low Level Programming
## Project Description
This project implements basic UNIX utility commands including pwd, echo, cp, and mv and also implements shells (femto, pico, nano amd micro).
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
#### Nano Shell
```bash
./executables/shells/nano_shell
```
The nano shell extends the pico shell functionality with variable handling capabilities:
- All pico shell built-in commands
- Local variable assignment: `var=value`
- Environment variable export: `export var_env=value`
- Variable expansion: `echo $var`
- Environment variable persistence for child processes
Examples:
```bash
# Set a local variable
user=john
# Set and export an environment variable
export PATH=/usr/bin:/bin
# Use variables in commands
echo "Hello, $user!"
# Use environment variables with external commands
export EDITOR=vim
$EDITOR file.txt
```
#### Micro Shell
```bash
./executables/shells/micro_shell
```
The micro shell extends the nano shell functionality with I/O redirection capabilities:
- All nano shell built-in commands and variable handling
- Input redirection: `command < input_file`
- Output redirection: `command > output_file`
- Error redirection: `command 2> error_file`
Examples:
```bash
# Redirect input from a file
sort < unsorted.txt
# Redirect output to a file
ls -la > directory_contents.txt
# Redirect error output to a file
find /root 2> errors.log
# Combine redirections
grep "pattern" < input.txt > results.txt 2> grep_errors.log
```
## Notes
- Ensure you have executable permissions for all files
- Run `chmod +x executables/shells/*` if needed
- Environment variables set in nano_shell are available to child processes
- Local variables are only accessible within the shell session
- I/O redirection in micro_shell allows for standard UNIX-like file operations

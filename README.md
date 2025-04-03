# Low level programming

## Project Description
This project implements basic UNIX utility commands including pwd, echo, cp, and mv and also implements shells (femto).

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
After compilation, you can run the utilities from the `executables/utilies` directory:

- Print Working Directory utility:
  ```bash
  ./executables/utilies/pwd
  ```

- Echo utility:
  ```bash
  ./executables/utilies/echo [arguments]
  ```

- Copy utility:
  ```bash
  ./executables/utilies/cp [source] [destination]
  ```

- Move utility:
  ```bash
  ./executables/utilies/mv [source] [destination]
  ```
  
### Running Shells
After compilation, you can run the shells from the `executables/shells` directory:

- Femto shell:
  ```bash
  ./executables/shells/femto_shell
  ```

## Notes
- Ensure you have executable permissions for all files
- Run `chmod +x executables/*` if needed

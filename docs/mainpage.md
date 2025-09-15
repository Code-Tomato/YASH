# YASH - Yet Another Shell

YASH is a simplified command line interpreter written in C that demonstrates core shell functionality including process management, redirection, job control, and signal handling.

## Features

### Core Functionality
- **Command Execution**: Execute external programs and built-in commands
- **Redirection**: Support for `<`, `>`, and `2>` (stdin, stdout, stderr)
- **Pipes**: Single pipe `|` support for command chaining
- **Environment**: Inherits and manages environment variables
- **PATH Resolution**: Finds executables via PATH environment variable

### Job Control
- **Background Jobs**: Run processes in background with `&`
- **Job Management**: Built-in commands `jobs`, `fg`, and `bg`
- **Job Tracking**: Supports up to 20 concurrent jobs
- **Process Groups**: Proper process group management

### Signal Handling
- **SIGINT**: Handles `Ctrl-C` for process interruption
- **SIGTSTP**: Handles `Ctrl-Z` for process suspension
- **SIGCHLD**: Manages child process termination

### User Interface
- **Prompt**: Clean `# ` prompt
- **Input**: Line-based command input
- **Exit**: Clean exit on `Ctrl-D` (EOF)

## Architecture

The shell is organized into several key modules:

- **main.c**: Entry point and main shell loop
- **parse.c**: Command parsing and tokenization
- **exec.c**: Command execution and process management
- **jobs.c**: Job control and background process management
- **signals.c**: Signal handling and process control

## Usage Examples

```bash
# Basic command execution
# ls -la

# Redirection
# echo "Hello World" > output.txt
# cat < input.txt

# Pipes
# ls -la | grep ".c"

# Background jobs
# sleep 10 &
# jobs
# fg

# Job control
# sleep 30 &
# jobs
# bg %1
# fg %1
```

## Building

```bash
# Build the project
make

# Generate documentation
make docs

# Clean build artifacts
make clean

# Clean documentation
make docs-clean
```

## Project Structure

```
YASH/
├── src/                    # Source files (.c)
├── include/                # Header files (.h)
├── build/                  # Build artifacts
│   ├── obj/               # Object files
│   └── bin/               # Executable
├── docs/                   # Documentation
│   ├── html/              # Generated HTML docs
│   ├── images/            # Documentation images
│   └── doxygen-awesome-css/ # Theme files
└── Makefile               # Build configuration
```

# Yet Another Shell (YASH)

YASH is a simplified command line interpreter written in C.
It supports basic process management, redirection, job control, and signals.

---

## Features
- **Redirection**: `<`, `>`, `2>` (stdin, stdout, stderr).
- **Pipes**: single `|` supported.
- **Signals**: handles `Ctrl-C` (SIGINT), `Ctrl-Z` (SIGTSTP), and `SIGCHLD`.
- **Job control**:
  - Run background jobs with `&`.
  - `jobs`, `fg`, and `bg` commands.
  - Tracks up to 20 jobs at once.
- **Misc**:
  - Inherits environment variables.
  - Finds executables via `PATH`.
  - Clean exit on `Ctrl-D`.
  - Prompt: `# `

---

## Build & Run

### Quick Start
```bash
make
./bin/yash
```

### Build Options
The project includes targets:

```bash
make              # Build the project (default)
make all          # Same as above
make clean        # Remove all compiled files
make rebuild      # Clean and build from scratch
make install      # Install to /usr/local/bin
make uninstall    # Remove from /usr/local/bin
make help         # Show all available targets
```

### Project Structure
```
YASH/
├── src/          # Source files (.c)
├── include/      # Header files (.h)
├── obj/          # Object files (.o) - created during build
├── bin/          # Executable - created during build
└── Makefile      # Build configuration
```

### Dependencies
```bash
# Ubuntu/Debian
sudo apt install libreadline-dev

# macOS (with Homebrew)
brew install readline

# Or build without readline support (basic functionality only)
```

---

## Example Usage

```bash
# echo hello > out.txt
# cat < out.txt | grep h
hello
# sleep 10 &
[1] + Running       sleep 10 &
# jobs
[1] + Running       sleep 10 &
# fg
sleep 10
# 
```
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
make docs         # Generate documentation
make docs-clean   # Remove generated documentation
make install      # Install to /usr/local/bin
make uninstall    # Remove from /usr/local/bin
make help         # Show all available targets
```

### Project Structure
```
YASH/
├── src/                    # Source files (.c)
├── include/                # Header files (.h)
├── build/                  # Build artifacts (created during build)
│   ├── obj/               # Object files (.o)
│   └── bin/               # Executable
├── docs/                   # Documentation
│   ├── html/              # Generated HTML docs (run `make docs`)
│   ├── images/            # Documentation images
│   ├── mainpage.md        # Main documentation page
│   └── doxygen-awesome-css/ # Doxygen theme
├── Makefile               # Build configuration
└── Doxyfile              # Documentation configuration
```

### Dependencies
```bash
# Ubuntu/Debian
sudo apt install libreadline-dev

# macOS (with Homebrew)
brew install readline

# Or build without readline support (basic functionality only)
```

### Documentation
The project includes comprehensive documentation generated with Doxygen and styled with the Doxygen Awesome theme:

**🌐 Live Documentation**: [View on GitHub Pages](https://yourusername.github.io/YASH/)

**Local Development:**
```bash
# Generate documentation
make docs

# View documentation
open docs/html/index.html
```

The documentation includes:
- **API Reference**: Complete function and structure documentation
- **Architecture Overview**: Module organization and relationships
- **Usage Examples**: Code examples and usage patterns
- **Modern Theme**: Clean, responsive design with dark mode support

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
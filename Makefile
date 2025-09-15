# Makefile for YASH project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include -g -O0
TEST_CFLAGS = $(CFLAGS) -I$(UNITYDIR)/src -I$(UNITYDIR)/extras/fixture/src
LDFLAGS = 

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = build/obj
BINDIR = build/bin
DOCDIR = docs
TESTDIR = tests
UNITYDIR = $(TESTDIR)/unity
TESTSRCDIR = $(TESTDIR)/test_src
TESTBINDIR = build/test

# Target executable name
TARGET = yash

# Source files (automatically find all .c files in src/)
SOURCES = $(wildcard $(SRCDIR)/*.c)
TEST_SOURCES = $(wildcard $(TESTSRCDIR)/*.c)

# Object files (replace .c with .o and add obj/ prefix)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTSRCDIR)/%.c=$(OBJDIR)/%.o)

# Unity source files
UNITY_SOURCES = $(UNITYDIR)/src/unity.c
UNITY_OBJECTS = $(UNITY_SOURCES:$(UNITYDIR)/src/%.c=$(OBJDIR)/%.o)

# Default target (what gets built when you just run 'make')
all: $(BINDIR)/$(TARGET)

# Generate compile_commands.json for clangd
compile_commands.json: clean
	@echo "Generating compile_commands.json for clangd..."
	@bear -- make all
	@echo "compile_commands.json generated successfully!"

# Build everything including tests
all-setup: clean all test
	@echo ""
	@echo "You can now:"
	@echo "  - Run tests: make test"
	@echo "  - Build project: make all"
	@echo "  - Format code: make format"
	@echo "  - Generate docs: make docs"

# Create the executable
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(TESTBINDIR):
	mkdir -p $(TESTBINDIR)

# Clean up compiled files
clean:
	rm -rf build

# Clean and rebuild
rebuild: clean all

# Test targets
test: $(TESTBINDIR)/test_runner
	@echo "Running all tests..."
	@$(TESTBINDIR)/test_runner

# Build test runner executable
$(TESTBINDIR)/test_runner: $(OBJDIR)/test_runner.o $(TEST_OBJECTS) $(UNITY_OBJECTS) $(OBJDIR)/parse.o | $(TESTBINDIR)
	$(CC) $(OBJDIR)/test_runner.o $(filter-out $(OBJDIR)/test_runner.o,$(TEST_OBJECTS)) $(UNITY_OBJECTS) $(OBJDIR)/parse.o -o $@ $(LDFLAGS)

# Compile test source files
$(OBJDIR)/test_%.o: $(TESTSRCDIR)/test_%.c | $(OBJDIR)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Compile Unity source files
$(OBJDIR)/unity.o: $(UNITYDIR)/src/unity.c | $(OBJDIR)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

# Install the executable (optional)
install: $(BINDIR)/$(TARGET)
	cp $(BINDIR)/$(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Documentation targets
docs: $(DOCDIR)/html/index.html

$(DOCDIR)/html/index.html: $(wildcard $(INCDIR)/*.h) $(wildcard $(SRCDIR)/*.c) Doxyfile
	doxygen Doxyfile

docs-clean:
	rm -rf $(DOCDIR)/html $(DOCDIR)/latex

# Format code
format:
	@echo "Formatting source files..."
	clang-format -i $(wildcard $(SRCDIR)/*.c) $(wildcard $(INCDIR)/*.h)
	@echo "Formatting complete!"


# Show help
help:
	@echo "Available targets:"
	@echo "  all-setup  - Build everything and run tests (RECOMMENDED)"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove all compiled files"
	@echo "  rebuild    - Clean and build"
	@echo "  test       - Run all unit tests"
	@echo "  format     - Format source code with clang-format"
	@echo "  docs       - Generate documentation"
	@echo "  docs-clean - Remove generated documentation"
	@echo "  compile_commands.json - Generate clangd configuration"
	@echo "  install    - Install to /usr/local/bin"
	@echo "  uninstall  - Remove from /usr/local/bin"
	@echo "  help       - Show this help message"

# Declare phony targets (targets that don't create files)
.PHONY: all all-setup clean rebuild test format docs docs-clean install uninstall help

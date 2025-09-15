# Makefile for YASH project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include -g -O0
LDFLAGS = 

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Target executable name
TARGET = yash

# Source files (automatically find all .c files in src/)
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Object files (replace .c with .o and add obj/ prefix)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target (what gets built when you just run 'make')
all: $(BINDIR)/$(TARGET)

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

# Clean up compiled files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Clean and rebuild
rebuild: clean all

# Install the executable (optional)
install: $(BINDIR)/$(TARGET)
	cp $(BINDIR)/$(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove all compiled files"
	@echo "  rebuild  - Clean and build"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  uninstall- Remove from /usr/local/bin"
	@echo "  help     - Show this help message"

# Declare phony targets (targets that don't create files)
.PHONY: all clean rebuild install uninstall help

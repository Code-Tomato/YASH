#!/bin/bash

# YASH Test Runner Script
# This script provides convenient commands for running tests locally

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to show help
show_help() {
    echo "YASH Test Runner"
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  test        Run all unit tests (default)"
    echo "  build       Build the project and tests"
    echo "  clean       Clean build artifacts"
    echo "  format      Format source code"
    echo "  coverage    Run tests with coverage analysis"
    echo "  valgrind    Run tests with valgrind memory checking"
    echo "  help        Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 test     # Run all tests"
    echo "  $0 coverage # Run tests with coverage"
    echo "  $0 valgrind # Run tests with memory checking"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to run tests
run_tests() {
    print_status "Building and running tests..."
    
    if ! make test; then
        print_error "Tests failed!"
        exit 1
    fi
    
    print_success "All tests passed!"
}

# Function to build project
build_project() {
    print_status "Building project..."
    make clean
    make all
    make test
    print_success "Build completed successfully!"
}

# Function to clean build artifacts
clean_project() {
    print_status "Cleaning build artifacts..."
    make clean
    print_success "Clean completed!"
}

# Function to format code
format_code() {
    print_status "Formatting source code..."
    make format
    print_success "Code formatting completed!"
}

# Function to run tests with coverage
run_coverage() {
    if ! command_exists gcov; then
        print_error "gcov not found. Please install gcc with coverage support."
        exit 1
    fi
    
    print_status "Running tests with coverage analysis..."
    
    # Clean and rebuild with coverage flags
    make clean
    CFLAGS="-Wall -Wextra -std=c99 -I./include -g -O0 --coverage" make all
    CFLAGS="-Wall -Wextra -std=c99 -I./include -g -O0 --coverage" make test
    
    # Run tests
    ./build/test/test_runner
    
    # Generate coverage report
    print_status "Generating coverage report..."
    gcov -r src/*.c
    lcov --capture --directory . --output-file coverage.info
    genhtml coverage.info --output-directory coverage_html
    
    print_success "Coverage report generated in coverage_html/"
    print_status "Open coverage_html/index.html in your browser to view the report"
}

# Function to run tests with valgrind
run_valgrind() {
    if ! command_exists valgrind; then
        print_error "valgrind not found. Please install valgrind."
        exit 1
    fi
    
    print_status "Running tests with valgrind memory checking..."
    
    # Build tests first
    make test
    
    # Run with valgrind
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
             --verbose --log-file=valgrind-out.txt ./build/test/test_runner
    
    if [ $? -eq 0 ]; then
        print_success "Valgrind check passed! No memory leaks found."
    else
        print_error "Valgrind found issues. Check valgrind-out.txt for details."
        exit 1
    fi
}

# Main script logic
case "${1:-test}" in
    "test")
        run_tests
        ;;
    "build")
        build_project
        ;;
    "clean")
        clean_project
        ;;
    "format")
        format_code
        ;;
    "coverage")
        run_coverage
        ;;
    "valgrind")
        run_valgrind
        ;;
    "help"|"-h"|"--help")
        show_help
        ;;
    *)
        print_error "Unknown command: $1"
        echo ""
        show_help
        exit 1
        ;;
esac

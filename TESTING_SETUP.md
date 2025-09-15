# YASH Testing Setup - Complete Guide

## Testing Framework Successfully Configured!

Your YASH shell project now has a comprehensive testing framework set up with automated CI/CD integration.

## What Was Created

### Directory Structure
```
YASH/
├── tests/
│   ├── unity/              # Unity testing framework
│   ├── test_src/           # Test source files
│   │   ├── test_parse.c    # Parsing tests
│   │   ├── test_yash.c     # Core data structure tests
│   │   └── test_runner.c   # Main test runner
│   └── README.md           # Testing documentation
├── .github/
│   └── workflows/
│       └── ci.yml          # GitHub Actions CI/CD
├── run_tests.sh            # Local test runner script
└── TESTING_SETUP.md        # This file
```

### Updated Files
- `Makefile` - Added test targets and Unity integration
- `tests/` - Complete testing framework

## How to Use

### Quick Start
```bash
# Run all tests
make test

# Or use the test runner script
./run_tests.sh test
```

### Available Commands

#### Using Make
```bash
make test        # Run all unit tests
make clean       # Clean build artifacts
make rebuild     # Clean and rebuild
make help        # Show all available targets
```

#### Using Test Runner Script
```bash
./run_tests.sh test        # Run all tests
./run_tests.sh build       # Build project and tests
./run_tests.sh coverage    # Run tests with coverage analysis
./run_tests.sh valgrind    # Run tests with memory leak detection
./run_tests.sh format      # Format source code
./run_tests.sh clean       # Clean build artifacts
./run_tests.sh help        # Show help
```

## Current Test Coverage

### Parse Module Tests (`test_parse.c`)
- Simple command tokenization (`ls -la`)
- Commands with multiple spaces
- Empty line handling
- Whitespace-only input
- Line parsing functionality
- Input length validation
- Empty input handling

### Core Data Structure Tests (`test_yash.c`)
- Command structure initialization
- Line structure initialization
- Token table validation
- Constants value verification

**Total: 11 tests, all passing!**

## Automated Testing (GitHub Actions)

Your project now has automated testing that runs on:
- **Every push** to main/develop branches
- **Every pull request** to main/develop branches

### CI Pipeline Features
1. **Multi-platform testing** (Ubuntu, macOS)
2. **Memory leak detection** with Valgrind
3. **Code quality checks** with cppcheck
4. **Security scanning** with CodeQL
5. **Documentation generation**
6. **Test result artifacts**

## Test Coverage Analysis

To generate a coverage report:
```bash
./run_tests.sh coverage
```

This will:
1. Build with coverage flags
2. Run all tests
3. Generate HTML report in `coverage_html/`
4. Show coverage statistics

## Memory Testing

To check for memory leaks:
```bash
./run_tests.sh valgrind
```

This will run Valgrind to detect:
- Memory leaks
- Use of uninitialized memory
- Invalid memory access
- Memory corruption

## Writing New Tests

### 1. Create Test File
Create a new file in `tests/test_src/test_<module>.c`:

```c
#include "unity.h"
#include "../../include/your_header.h"

void test_your_function(void) {
    // Test code here
    TEST_ASSERT_EQUAL(expected, actual);
}
```

### 2. Add to Test Runner
Add your test function to `tests/test_src/test_runner.c`:

```c
extern void test_your_function(void);

// In main():
RUN_TEST(test_your_function);
```

### 3. Run Tests
```bash
make test
```

## Testing Framework Details

### Unity Framework
- **Lightweight**: No external dependencies
- **Fast**: Quick compilation and execution
- **Reliable**: Used in embedded systems
- **Well-documented**: Extensive documentation available

### Test Macros Available
- `TEST_ASSERT_EQUAL(expected, actual)`
- `TEST_ASSERT_NOT_EQUAL(expected, actual)`
- `TEST_ASSERT_EQUAL_STRING(expected, actual)`
- `TEST_ASSERT_NULL(pointer)`
- `TEST_ASSERT_NOT_NULL(pointer)`
- `TEST_ASSERT_TRUE(condition)`
- `TEST_ASSERT_FALSE(condition)`

## Best Practices

1. **Write tests for all public functions**
2. **Test edge cases and error conditions**
3. **Keep tests simple and focused**
4. **Use descriptive test names**
5. **Clean up resources in tearDown()**
6. **Test both success and failure paths**

## Troubleshooting

### Common Issues
1. **Compilation errors**: Check header includes
2. **Link errors**: Verify Makefile dependencies
3. **Test failures**: Use debug output
4. **Memory issues**: Run with valgrind

### Getting Help
- Check `tests/README.md` for detailed documentation
- Review existing test files for examples
- Use `make help` for available targets
- Unity documentation: https://github.com/ThrowTheSwitch/Unity

## Next Steps

1. **Add more tests** as you develop new features
2. **Run tests frequently** during development
3. **Check CI results** on GitHub
4. **Generate coverage reports** to identify untested code
5. **Use memory testing** to catch leaks early

## Benefits You Now Have

- **Automated testing** on every code change  
- **Multi-platform validation** (Linux, macOS)  
- **Memory leak detection**  
- **Code quality checks**  
- **Security scanning**  
- **Coverage analysis**  
- **Easy local testing**  
- **Professional CI/CD pipeline**  

Your YASH shell project is now equipped with enterprise-grade testing infrastructure!

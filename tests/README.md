# YASH Testing Framework

This directory contains the testing framework and test cases for the YASH shell project.

## Directory Structure

```
tests/
├── unity/           # Unity testing framework source code
├── test_src/        # Test source files
│   ├── test_parse.c # Tests for parsing functionality
│   └── test_yash.c  # Tests for core data structures
└── README.md        # This file
```

## Testing Framework

We use **Unity** as our C unit testing framework. Unity is:
- Lightweight and fast
- No external dependencies
- Easy to integrate with CI/CD
- Well-suited for embedded/system programming

## Running Tests

### Quick Test Run
```bash
make test
```

### Using the Test Runner Script
```bash
# Run all tests
./run_tests.sh test

# Build project and tests
./run_tests.sh build

# Run tests with coverage analysis
./run_tests.sh coverage

# Run tests with memory leak detection
./run_tests.sh valgrind

# Format code
./run_tests.sh format

# Clean build artifacts
./run_tests.sh clean
```

## Writing Tests

### Test File Structure
Each test file should follow this pattern:

```c
#include "unity.h"
#include "../../include/your_header.h"

void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

void test_your_function(void) {
    // Your test code here
    TEST_ASSERT_EQUAL(expected, actual);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_your_function);
    return UNITY_END();
}
```

### Available Test Macros
- `TEST_ASSERT_EQUAL(expected, actual)` - Check equality
- `TEST_ASSERT_NOT_EQUAL(expected, actual)` - Check inequality
- `TEST_ASSERT_EQUAL_STRING(expected, actual)` - Check string equality
- `TEST_ASSERT_NULL(pointer)` - Check if pointer is NULL
- `TEST_ASSERT_NOT_NULL(pointer)` - Check if pointer is not NULL
- `TEST_ASSERT_TRUE(condition)` - Check if condition is true
- `TEST_ASSERT_FALSE(condition)` - Check if condition is false

## Automated Testing

### GitHub Actions
Tests are automatically run on every push and pull request via GitHub Actions. The CI pipeline includes:

1. **Multi-platform testing** (Ubuntu, macOS)
2. **Memory leak detection** with Valgrind
3. **Code quality checks** with cppcheck
4. **Security scanning** with CodeQL
5. **Documentation generation**

### Local Development
For local development, use the test runner script which provides:
- Colored output for better readability
- Coverage analysis
- Memory leak detection
- Code formatting
- Build automation

## Test Coverage

To generate a coverage report:
```bash
./run_tests.sh coverage
```

This will:
1. Build the project with coverage flags
2. Run all tests
3. Generate an HTML coverage report in `coverage_html/`
4. Open the report in your browser

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

## Best Practices

1. **Write tests for all public functions**
2. **Test edge cases and error conditions**
3. **Keep tests simple and focused**
4. **Use descriptive test names**
5. **Clean up resources in tearDown()**
6. **Test both success and failure paths**

## Adding New Tests

1. Create a new test file in `tests/test_src/`
2. Follow the naming convention: `test_<module>.c`
3. Include the appropriate header files
4. Write test functions with descriptive names
5. Add the test to the main() function
6. Run `make test` to verify your tests work

## Troubleshooting

### Common Issues

1. **Compilation errors**: Check that all required headers are included
2. **Link errors**: Ensure all source files are properly referenced in the Makefile
3. **Test failures**: Use debug output to understand what's failing
4. **Memory issues**: Run with valgrind to identify memory problems

### Getting Help

- Check the Unity documentation: https://github.com/ThrowTheSwitch/Unity
- Review existing test files for examples
- Use `make help` to see all available targets

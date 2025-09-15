#include "../../include/parse.h"
#include "../../include/yash.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// setUp and tearDown are defined in test_runner.c

// ============================================================================
// Input Validation Tests
// ============================================================================

void test_parse_null_input(void) {
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(NULL, &parsed_line);

   // This should fail because input is NULL
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_null_output(void) {
   char line[] = "ls";

   int result = parse_line(line, NULL);

   // This should fail because output is NULL
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_empty_input(void) {
   char line[] = "";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // Empty input should return -1 (invalid)
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_whitespace_only(void) {
   char line[] = "   \t  \t  ";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // Whitespace-only input should return -1 (invalid)
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_newline_only(void) {
   char line[] = "\n";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // Newline-only input should return 1 (empty line)
   TEST_ASSERT_EQUAL(1, result);
}

// ============================================================================
// Length Limit Tests
// ============================================================================

void test_parse_max_length(void) {
   // Create a line of exactly MAX_CMDLINE - 1 characters
   char line[MAX_CMDLINE];
   memset(line, 'a', MAX_CMDLINE - 1);
   line[MAX_CMDLINE - 1] = '\0';

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should succeed because it's exactly the maximum length
   TEST_ASSERT_EQUAL(0, result);
}

void test_parse_over_max_length(void) {
   // Create a line longer than MAX_CMDLINE
   char line[MAX_CMDLINE + 10];
   memset(line, 'a', MAX_CMDLINE + 9);
   line[MAX_CMDLINE + 9] = '\0';

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because it's too long
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_max_tokens(void) {
   // Create a line with exactly MAX_TOKENS tokens
   char line[MAX_CMDLINE];
   int pos = 0;
   for (int i = 0; i < MAX_TOKENS - 1; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, "token%d ", i);
   }
   snprintf(line + pos, MAX_CMDLINE - pos, "token%d", MAX_TOKENS - 1);

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should succeed because it's exactly the maximum number of tokens
   TEST_ASSERT_EQUAL(0, result);
}

void test_parse_over_max_tokens(void) {
   // Create a line with more than MAX_TOKENS tokens
   char line[MAX_CMDLINE];
   int pos = 0;
   for (int i = 0; i < MAX_TOKENS; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, "token%d ", i);
   }

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because it has too many tokens
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_max_token_length(void) {
   // Create a token of exactly MAX_TOKEN_LEN characters
   char long_token[MAX_TOKEN_LEN + 1];
   memset(long_token, 'a', MAX_TOKEN_LEN);
   long_token[MAX_TOKEN_LEN] = '\0';

   char line[MAX_CMDLINE];
   snprintf(line, MAX_CMDLINE, "ls %s", long_token);

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should succeed because the token is exactly the maximum length
   TEST_ASSERT_EQUAL(0, result);
}

void test_parse_over_max_token_length(void) {
   // Create a token longer than MAX_TOKEN_LEN
   char long_token[MAX_TOKEN_LEN + 10];
   memset(long_token, 'a', MAX_TOKEN_LEN + 9);
   long_token[MAX_TOKEN_LEN + 9] = '\0';

   char line[MAX_CMDLINE];
   snprintf(line, MAX_CMDLINE, "ls %s", long_token);

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because the token is too long
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Invalid Command Tests
// ============================================================================

void test_parse_invalid_starting_tokens(void) {
   // Test commands that start with invalid tokens
   char* invalid_commands[] = {"< ls", "> ls", "2> ls", "| ls", "& ls"};

   for (int i = 0; i < 5; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(invalid_commands[i], &parsed_line);

      // All of these should fail because they start with invalid tokens
      TEST_ASSERT_EQUAL(-1, result);
   }
}

void test_parse_malformed_redirections(void) {
   // Test malformed redirections
   char* malformed_commands[] = {"ls > > output.txt",
                                 "ls < < input.txt",
                                 "ls 2> 2> error.txt",
                                 "ls > > > output.txt",
                                 "ls < > input.txt",
                                 "ls 2> > error.txt"};

   for (int i = 0; i < 6; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(malformed_commands[i], &parsed_line);

      // All of these should fail because of malformed redirections
      TEST_ASSERT_EQUAL(-1, result);
   }
}

void test_parse_malformed_pipes(void) {
   // Test malformed pipes
   char* malformed_commands[] = {"ls | | grep test",
                                 "ls | | | grep test",
                                 "ls | grep | test",
                                 "| ls | grep test",
                                 "ls | grep test |"};

   for (int i = 0; i < 5; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(malformed_commands[i], &parsed_line);

      // All of these should fail because of malformed pipes
      TEST_ASSERT_EQUAL(-1, result);
   }
}

void test_parse_malformed_background(void) {
   // Test malformed background execution
   char* malformed_commands[] = {
       "ls & &", "ls & & &", "ls & grep test", "& ls", "ls & | grep test"};

   for (int i = 0; i < 5; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(malformed_commands[i], &parsed_line);

      // All of these should fail because of malformed background execution
      TEST_ASSERT_EQUAL(-1, result);
   }
}

// ============================================================================
// Special Character Tests
// ============================================================================

void test_parse_special_characters(void) {
   // Test commands with special characters
   char line[] = "ls -la | grep 'test file' > output-123_456.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should succeed because special characters are allowed in filenames
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls -la | grep 'test file' > output-123_456.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-la", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("'test", parsed_line.right.argv[1]);
   TEST_ASSERT_EQUAL_STRING("file'", parsed_line.right.argv[2]);
   TEST_ASSERT_EQUAL_STRING("output-123_456.txt", parsed_line.right.out_file);
}

void test_parse_quoted_strings(void) {
   // Test commands with quoted strings (should fail according to requirements)
   char line[] = "echo \"hello world\"";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because we don't handle quotes
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_escaped_characters(void) {
   // Test commands with escaped characters
   char line[] = "echo hello\\ world";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because we don't handle escapes
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Memory Safety Tests
// ============================================================================

void test_parse_memory_safety(void) {
   // Test that parsing doesn't cause memory issues
   char line[] = "ls -la | grep test > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);

   // Test that the original string is preserved
   TEST_ASSERT_EQUAL_STRING("ls -la | grep test > output.txt", parsed_line.original);

   // Test that pointers are valid
   TEST_ASSERT_NOT_NULL(parsed_line.left.argv[0]);
   TEST_ASSERT_NOT_NULL(parsed_line.right.argv[0]);
   TEST_ASSERT_NOT_NULL(parsed_line.right.out_file);
}

void test_parse_buffer_overflow_protection(void) {
   // Test that parsing protects against buffer overflows
   char line[MAX_CMDLINE + 100];
   memset(line, 'a', MAX_CMDLINE + 99);
   line[MAX_CMDLINE + 99] = '\0';

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because the line is too long
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Error Recovery Tests
// ============================================================================

void test_parse_error_recovery(void) {
   // Test that parsing can recover from errors
   char* test_commands[] = {
       "ls -la",          // Valid command
       "invalid_command", // Invalid command (should still parse)
       "ls | grep test",  // Valid pipe
       "ls > output.txt"  // Valid redirection
   };

   for (int i = 0; i < 4; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(test_commands[i], &parsed_line);

      // All of these should parse successfully
      TEST_ASSERT_EQUAL(0, result);
   }
}

void test_parse_partial_commands(void) {
   // Test partial commands that might be typed
   char* partial_commands[] = {"ls",
                               "ls -",
                               "ls -l",
                               "ls -la",
                               "ls |",
                               "ls | grep",
                               "ls | grep test",
                               "ls >",
                               "ls > output",
                               "ls > output.txt"};

   for (int i = 0; i < 10; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(partial_commands[i], &parsed_line);

      // Some should succeed, some should fail
      if (i == 4 || i == 7) { // "ls |" and "ls >"
         TEST_ASSERT_EQUAL(-1, result);
      } else {
         TEST_ASSERT_EQUAL(0, result);
      }
   }
}

// ============================================================================
// Performance Tests
// ============================================================================

void test_parse_performance_simple(void) {
   // Test parsing performance for simple commands
   char line[] = "ls -la";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   clock_t start = clock();

   for (int i = 0; i < 1000; i++) {
      int result = parse_line(line, &parsed_line);
      TEST_ASSERT_EQUAL(0, result);
   }

   clock_t end = clock();
   double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

   // Parsing should be very fast
   TEST_ASSERT_TRUE(cpu_time_used < 0.1); // Less than 100ms for 1000 iterations
}

void test_parse_performance_complex(void) {
   // Test parsing performance for complex commands
   char line[] = "find . -name '*.c' -type f | grep -v test | wc -l > count.txt 2> errors.log";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   clock_t start = clock();

   for (int i = 0; i < 100; i++) {
      int result = parse_line(line, &parsed_line);
      TEST_ASSERT_EQUAL(0, result);
   }

   clock_t end = clock();
   double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

   // Parsing should be reasonably fast even for complex commands
   TEST_ASSERT_TRUE(cpu_time_used < 0.1); // Less than 100ms for 100 iterations
}

// ============================================================================
// Integration Tests
// ============================================================================

void test_parse_integration_all_features(void) {
   // Test parsing with all features combined
   char line[] = "cat < input.txt | grep -i pattern | wc -l > count.txt 2> errors.log";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input.txt | grep -i pattern | wc -l > count.txt 2> errors.log",
                            parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (cat)
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-i", parsed_line.right.argv[1]);
   TEST_ASSERT_EQUAL_STRING("pattern", parsed_line.right.argv[2]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_EQUAL_STRING("count.txt", parsed_line.right.out_file);
   TEST_ASSERT_EQUAL_STRING("errors.log", parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_integration_edge_cases(void) {
   // Test parsing with edge cases
   char* edge_cases[] = {
       "a",             // Single character command
       "a b",           // Two single character arguments
       "a | b",         // Single character pipe
       "a > b",         // Single character redirection
       "a &",           // Single character background
       "a < b > c 2> d" // Single character with all redirections
   };

   for (int i = 0; i < 6; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(edge_cases[i], &parsed_line);

      // All of these should succeed
      TEST_ASSERT_EQUAL(0, result);
   }
}

// Test functions are called from test_runner.c

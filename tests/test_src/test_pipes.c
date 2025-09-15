#include "../../include/parse.h"
#include "../../include/yash.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

// setUp and tearDown are defined in test_runner.c

// ============================================================================
// Basic Pipe Parsing Tests
// ============================================================================

void test_parse_simple_pipe(void) {
   char line[] = "ls | grep test";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls | grep test", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (ls)
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_with_arguments(void) {
   char line[] = "cat file.txt | grep -i pattern | wc -l";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat file.txt | grep -i pattern | wc -l", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (cat)
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("file.txt", parsed_line.left.argv[1]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-i", parsed_line.right.argv[1]);
   TEST_ASSERT_EQUAL_STRING("pattern", parsed_line.right.argv[2]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_with_redirections(void) {
   char line[] = "cat < input.txt | grep test > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input.txt | grep test > output.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (cat)
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_both_commands_redirections(void) {
   char line[] = "cat < input1.txt | grep test < input2.txt > output.txt 2> error.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input1.txt | grep test < input2.txt > output.txt 2> error.txt",
                            parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (cat)
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input1.txt", parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_EQUAL_STRING("input2.txt", parsed_line.right.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.right.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

// ============================================================================
// Pipe Edge Cases and Error Conditions
// ============================================================================

void test_parse_pipe_missing_left_command(void) {
   char line[] = "| grep test";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there's no command before the pipe
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_pipe_missing_right_command(void) {
   char line[] = "ls |";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there's no command after the pipe
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_pipe_missing_both_commands(void) {
   char line[] = "|";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there are no commands
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_pipe_with_background(void) {
   char line[] = "ls | grep test &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because pipes and background execution are mutually exclusive
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_pipe_background_left_command(void) {
   char line[] = "ls & | grep test";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because pipes and background execution are mutually exclusive
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_pipe_background_right_command(void) {
   char line[] = "ls | grep test &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because pipes and background execution are mutually exclusive
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Pipe with Complex Commands
// ============================================================================

void test_parse_pipe_complex_left_command(void) {
   char line[] = "find . -name '*.c' -type f | grep -v test";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("find . -name '*.c' -type f | grep -v test", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (find)
   TEST_ASSERT_EQUAL_STRING("find", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING(".", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("-name", parsed_line.left.argv[2]);
   TEST_ASSERT_EQUAL_STRING("'*.c'", parsed_line.left.argv[3]);
   TEST_ASSERT_EQUAL_STRING("-type", parsed_line.left.argv[4]);
   TEST_ASSERT_EQUAL_STRING("f", parsed_line.left.argv[5]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-v", parsed_line.right.argv[1]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[2]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_complex_right_command(void) {
   char line[] = "ls | grep -E '^[a-z]' | sort | uniq";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls | grep -E '^[a-z]' | sort | uniq", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (ls)
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-E", parsed_line.right.argv[1]);
   TEST_ASSERT_EQUAL_STRING("'^[a-z]'", parsed_line.right.argv[2]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

// ============================================================================
// Pipe with Redirections
// ============================================================================

void test_parse_pipe_input_redirection_left(void) {
   char line[] = "cat < input.txt | grep test";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input.txt | grep test", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (cat)
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_output_redirection_right(void) {
   char line[] = "ls | grep test > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls | grep test > output.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (ls)
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_error_redirection_right(void) {
   char line[] = "ls | grep test 2> error.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls | grep test 2> error.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (ls)
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_all_redirections(void) {
   char line[] = "cat < input1.txt | grep test < input2.txt > output.txt 2> error.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input1.txt | grep test < input2.txt > output.txt 2> error.txt",
                            parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (cat)
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input1.txt", parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_EQUAL_STRING("input2.txt", parsed_line.right.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.right.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

// ============================================================================
// Pipe Validation Tests
// ============================================================================

void test_parse_pipe_invalid_characters(void) {
   char line[] = "ls | grep test with spaces";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because "test with spaces" is not a valid argument
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_pipe_malformed_redirection(void) {
   char line[] = "ls | grep test > > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because of malformed redirection
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_pipe_duplicate_redirection(void) {
   char line[] = "ls | grep test > output1.txt > output2.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because of duplicate output redirection
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Pipe Special Cases
// ============================================================================

void test_parse_pipe_empty_arguments(void) {
   char line[] = "ls | grep";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls | grep", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (ls)
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_single_character_commands(void) {
   char line[] = "a | b";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("a | b", parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (a)
   TEST_ASSERT_EQUAL_STRING("a", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (b)
   TEST_ASSERT_EQUAL_STRING("b", parsed_line.right.argv[0]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_NULL(parsed_line.right.out_file);
   TEST_ASSERT_NULL(parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

void test_parse_pipe_max_arguments(void) {
   // Create a command with maximum arguments
   char line[MAX_CMDLINE];
   int pos = 0;
   pos += snprintf(line + pos, MAX_CMDLINE - pos, "cmd1");
   for (int i = 0; i < MAX_ARGS - 1; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, " arg%d", i);
   }
   pos += snprintf(line + pos, MAX_CMDLINE - pos, " | cmd2");
   for (int i = 0; i < MAX_ARGS - 1; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, " arg%d", i);
   }

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command should have MAX_ARGS arguments
   TEST_ASSERT_EQUAL_STRING("cmd1", parsed_line.left.argv[0]);
   for (int i = 0; i < MAX_ARGS - 1; i++) {
      char expected_arg[32];
      snprintf(expected_arg, sizeof(expected_arg), "arg%d", i);
      TEST_ASSERT_EQUAL_STRING(expected_arg, parsed_line.left.argv[i + 1]);
   }

   // Right command should have MAX_ARGS arguments
   TEST_ASSERT_EQUAL_STRING("cmd2", parsed_line.right.argv[0]);
   for (int i = 0; i < MAX_ARGS - 1; i++) {
      char expected_arg[32];
      snprintf(expected_arg, sizeof(expected_arg), "arg%d", i);
      TEST_ASSERT_EQUAL_STRING(expected_arg, parsed_line.right.argv[i + 1]);
   }
}

// Test functions are called from test_runner.c

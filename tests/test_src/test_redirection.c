#include "../../include/parse.h"
#include "../../include/yash.h"
#include "unity.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// setUp and tearDown are defined in test_runner.c

// ============================================================================
// File Redirection Parsing Tests
// ============================================================================

void test_parse_input_redirection(void) {
   char line[] = "cat < input.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_output_redirection(void) {
   char line[] = "ls > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls > output.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_error_redirection(void) {
   char line[] = "ls 2> error.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls 2> error.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_multiple_redirections(void) {
   char line[] = "cat < input.txt > output.txt 2> error.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input.txt > output.txt 2> error.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_redirection_with_arguments(void) {
   char line[] = "grep -i pattern < input.txt > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("grep -i pattern < input.txt > output.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-i", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("pattern", parsed_line.left.argv[2]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

// ============================================================================
// File Redirection Edge Cases
// ============================================================================

void test_parse_redirection_missing_filename(void) {
   char line[] = "ls >";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there's no filename after >
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_missing_input_file(void) {
   char line[] = "cat <";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there's no filename after <
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_missing_error_file(void) {
   char line[] = "ls 2>";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there's no filename after 2>
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_invalid_order(void) {
   char line[] = "> output.txt ls";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because redirection comes before command
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_duplicate_output(void) {
   char line[] = "ls > output1.txt > output2.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because multiple output redirections are not allowed
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_duplicate_input(void) {
   char line[] = "cat < input1.txt < input2.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because multiple input redirections are not allowed
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_duplicate_error(void) {
   char line[] = "ls 2> error1.txt 2> error2.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because multiple error redirections are not allowed
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// File Redirection with Background Execution
// ============================================================================

void test_parse_redirection_background(void) {
   char line[] = "ls > output.txt &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls > output.txt &", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

void test_parse_multiple_redirections_background(void) {
   char line[] = "cat < input.txt > output.txt 2> error.txt &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input.txt > output.txt 2> error.txt &", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

// ============================================================================
// File Redirection with Pipes
// ============================================================================

void test_parse_redirection_pipe_with_redirections(void) {
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

void test_parse_redirection_pipe_both_commands_redirections(void) {
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
// File Redirection Validation Tests
// ============================================================================

void test_parse_redirection_invalid_characters(void) {
   char line[] = "ls > output with spaces.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because filename has spaces
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_empty_filename(void) {
   char line[] = "ls > ";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because filename is empty
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_filename_too_long(void) {
   // Create a filename that's too long
   char long_filename[MAX_TOKEN_LEN + 10];
   memset(long_filename, 'a', MAX_TOKEN_LEN + 9);
   long_filename[MAX_TOKEN_LEN + 9] = '\0';

   char line[MAX_CMDLINE];
   snprintf(line, MAX_CMDLINE, "ls > %s", long_filename);

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because filename is too long
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// File Redirection Special Cases
// ============================================================================

void test_parse_redirection_special_characters(void) {
   char line[] = "ls > output-123_456.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls > output-123_456.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("output-123_456.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_redirection_quoted_filename(void) {
   char line[] = "ls > \"output file.txt\"";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // According to the requirements, we don't need to handle quotes
   // So this should fail because of the space in the filename
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_absolute_path(void) {
   char line[] = "ls > /tmp/output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls > /tmp/output.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("/tmp/output.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_redirection_relative_path(void) {
   char line[] = "ls > ./output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls > ./output.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("./output.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

// ============================================================================
// File Redirection Error Handling
// ============================================================================

void test_parse_redirection_malformed_input(void) {
   char line[] = "ls > > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because of malformed redirection
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_malformed_error(void) {
   char line[] = "ls 2> 2> error.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because of malformed redirection
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_redirection_mixed_redirections(void) {
   char line[] = "ls > output.txt 2> error.txt < input.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls > output.txt 2> error.txt < input.txt", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("ls", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

// Test functions are called from test_runner.c

#include "../../include/parse.h"
#include "../../include/yash.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

// setUp and tearDown are defined in test_runner.c

// ============================================================================
// Background Execution Parsing Tests
// ============================================================================

// These tests are now in test_parse.c to avoid duplication

void test_parse_background_with_multiple_redirections(void) {
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

void test_parse_background_with_arguments(void) {
   char line[] = "find . -name '*.c' -type f > results.txt &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("find . -name '*.c' -type f > results.txt &", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("find", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING(".", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("-name", parsed_line.left.argv[2]);
   TEST_ASSERT_EQUAL_STRING("'*.c'", parsed_line.left.argv[3]);
   TEST_ASSERT_EQUAL_STRING("-type", parsed_line.left.argv[4]);
   TEST_ASSERT_EQUAL_STRING("f", parsed_line.left.argv[5]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("results.txt", parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

// ============================================================================
// Job Control Command Tests
// ============================================================================

void test_parse_jobs_command(void) {
   char line[] = "jobs";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("jobs", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("jobs", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_fg_command(void) {
   char line[] = "fg";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("fg", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("fg", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

void test_parse_bg_command(void) {
   char line[] = "bg";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("bg", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("bg", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);
}

// ============================================================================
// Job Control Edge Cases
// ============================================================================

void test_parse_background_missing_command(void) {
   char line[] = "&";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there's no command before &
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_background_with_pipe(void) {
   char line[] = "ls | grep test &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because pipes and background execution are mutually exclusive
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_background_pipe_left(void) {
   char line[] = "ls & | grep test";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because pipes and background execution are mutually exclusive
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_background_pipe_right(void) {
   char line[] = "ls | grep test &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because pipes and background execution are mutually exclusive
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_background_duplicate_ampersand(void) {
   char line[] = "ls & &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because there are multiple & symbols
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_background_ampersand_not_last(void) {
   char line[] = "ls & grep test";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because & is not the last token
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Job Control Command Validation
// ============================================================================

void test_parse_jobs_with_arguments(void) {
   char line[] = "jobs arg1 arg2";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat jobs as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("jobs", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("arg1", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("arg2", parsed_line.left.argv[2]);
}

void test_parse_fg_with_arguments(void) {
   char line[] = "fg arg1 arg2";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat fg as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("fg", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("arg1", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("arg2", parsed_line.left.argv[2]);
}

void test_parse_bg_with_arguments(void) {
   char line[] = "bg arg1 arg2";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat bg as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("bg", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("arg1", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("arg2", parsed_line.left.argv[2]);
}

void test_parse_jobs_with_redirection(void) {
   char line[] = "jobs > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat jobs as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("jobs", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
}

void test_parse_fg_with_redirection(void) {
   char line[] = "fg > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat fg as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("fg", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
}

void test_parse_bg_with_redirection(void) {
   char line[] = "bg > output.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat bg as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("bg", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
}

void test_parse_jobs_with_background(void) {
   char line[] = "jobs &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat jobs as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("jobs", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

void test_parse_fg_with_background(void) {
   char line[] = "fg &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat fg as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("fg", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

void test_parse_bg_with_background(void) {
   char line[] = "bg &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // For now, treat bg as a regular command (will be built-in later)
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("bg", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

// ============================================================================
// Job Control Special Cases
// ============================================================================

void test_parse_background_max_arguments(void) {
   // Create a background command with maximum arguments
   char line[MAX_CMDLINE];
   int pos = 0;
   pos += snprintf(line + pos, MAX_CMDLINE - pos, "cmd");
   for (int i = 0; i < MAX_ARGS - 1; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, " arg%d", i);
   }
   pos += snprintf(line + pos, MAX_CMDLINE - pos, " &");

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because it exceeds MAX_ARGS
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_background_single_character_command(void) {
   char line[] = "a &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("a &", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("a", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

void test_parse_background_empty_arguments(void) {
   char line[] = "cmd &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cmd &", parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("cmd", parsed_line.left.argv[0]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

// ============================================================================
// Job Control Complex Scenarios
// ============================================================================

void test_parse_background_complex_command(void) {
   char line[] = "find /usr -name '*.h' -type f | grep -v test > headers.txt 2> errors.log &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because pipes and background execution are mutually exclusive
   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_jobs_background_with_all_redirections(void) {
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

void test_parse_background_with_long_command(void) {
   char line[] = "grep -r -i 'pattern' /usr/include --include='*.h' --exclude='*.c' > results.txt "
                 "2> errors.log &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("grep -r -i 'pattern' /usr/include --include='*.h' --exclude='*.c' > "
                            "results.txt 2> errors.log &",
                            parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-r", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("-i", parsed_line.left.argv[2]);
   TEST_ASSERT_EQUAL_STRING("'pattern'", parsed_line.left.argv[3]);
   TEST_ASSERT_EQUAL_STRING("/usr/include", parsed_line.left.argv[4]);
   TEST_ASSERT_EQUAL_STRING("--include='*.h'", parsed_line.left.argv[5]);
   TEST_ASSERT_EQUAL_STRING("--exclude='*.c'", parsed_line.left.argv[6]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("results.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL_STRING("errors.log", parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

// Test functions are called from test_runner.c

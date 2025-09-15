#include "../../include/parse.h"
#include "../../include/yash.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

// setUp and tearDown are defined in test_runner.c

// ============================================================================
// Basic Tokenization Tests
// ============================================================================

void test_tokenize_simple_command(void) {
   char line[] = "ls -la";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(2, num_tokens);
   TEST_ASSERT_EQUAL_STRING("ls", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("-la", tokens[1]);
}

void test_tokenize_command_with_spaces(void) {
   char line[] = "  echo   hello   world  ";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(3, num_tokens);
   TEST_ASSERT_EQUAL_STRING("echo", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("hello", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("world", tokens[2]);
}

void test_tokenize_empty_line(void) {
   char line[] = "";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(0, num_tokens);
}

void test_tokenize_whitespace_only(void) {
   char line[] = "   \t  \t  ";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(0, num_tokens);
}

// ============================================================================
// Redirection Token Tests
// ============================================================================

void test_tokenize_input_redirection(void) {
   char line[] = "cat < input.txt";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(3, num_tokens);
   TEST_ASSERT_EQUAL_STRING("cat", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("<", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("input.txt", tokens[2]);
}

void test_tokenize_output_redirection(void) {
   char line[] = "ls > output.txt";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(3, num_tokens);
   TEST_ASSERT_EQUAL_STRING("ls", tokens[0]);
   TEST_ASSERT_EQUAL_STRING(">", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("output.txt", tokens[2]);
}

void test_tokenize_error_redirection(void) {
   char line[] = "ls 2> error.txt";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(3, num_tokens);
   TEST_ASSERT_EQUAL_STRING("ls", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("2>", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("error.txt", tokens[2]);
}

void test_tokenize_multiple_redirections(void) {
   char line[] = "cat < input.txt > output.txt 2> error.txt";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(7, num_tokens);
   TEST_ASSERT_EQUAL_STRING("cat", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("<", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("input.txt", tokens[2]);
   TEST_ASSERT_EQUAL_STRING(">", tokens[3]);
   TEST_ASSERT_EQUAL_STRING("output.txt", tokens[4]);
   TEST_ASSERT_EQUAL_STRING("2>", tokens[5]);
   TEST_ASSERT_EQUAL_STRING("error.txt", tokens[6]);
}

// ============================================================================
// Pipe Token Tests
// ============================================================================

void test_tokenize_simple_pipe(void) {
   char line[] = "ls | grep test";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(4, num_tokens);
   TEST_ASSERT_EQUAL_STRING("ls", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("|", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("grep", tokens[2]);
   TEST_ASSERT_EQUAL_STRING("test", tokens[3]);
}

void test_tokenize_pipe_with_redirections(void) {
   char line[] = "cat < input.txt | grep test > output.txt";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(8, num_tokens);
   TEST_ASSERT_EQUAL_STRING("cat", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("<", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("input.txt", tokens[2]);
   TEST_ASSERT_EQUAL_STRING("|", tokens[3]);
   TEST_ASSERT_EQUAL_STRING("grep", tokens[4]);
   TEST_ASSERT_EQUAL_STRING("test", tokens[5]);
   TEST_ASSERT_EQUAL_STRING(">", tokens[6]);
   TEST_ASSERT_EQUAL_STRING("output.txt", tokens[7]);
}

// ============================================================================
// Background Execution Token Tests
// ============================================================================

void test_tokenize_background_command(void) {
   char line[] = "sleep 5 &";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(3, num_tokens);
   TEST_ASSERT_EQUAL_STRING("sleep", tokens[0]);
   TEST_ASSERT_EQUAL_STRING("5", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("&", tokens[2]);
}

void test_tokenize_background_with_redirection(void) {
   char line[] = "ls > output.txt &";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(4, num_tokens);
   TEST_ASSERT_EQUAL_STRING("ls", tokens[0]);
   TEST_ASSERT_EQUAL_STRING(">", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("output.txt", tokens[2]);
   TEST_ASSERT_EQUAL_STRING("&", tokens[3]);
}

// ============================================================================
// Edge Cases and Error Conditions
// ============================================================================

void test_tokenize_max_tokens(void) {
   // Create a line with exactly MAX_TOKENS tokens
   char line[MAX_CMDLINE];
   int pos = 0;
   for (int i = 0; i < MAX_TOKENS - 1; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, "token%d ", i);
   }
   snprintf(line + pos, MAX_CMDLINE - pos, "token%d", MAX_TOKENS - 1);

   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(MAX_TOKENS, num_tokens);
}

void test_tokenize_too_many_tokens(void) {
   // Create a line with more than MAX_TOKENS tokens
   char line[MAX_CMDLINE];
   int pos = 0;
   for (int i = 0; i < MAX_TOKENS; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, "token%d ", i);
   }

   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(-1, result);
}

void test_tokenize_max_token_length(void) {
   // Create a token of exactly MAX_TOKEN_LEN characters
   char long_token[MAX_TOKEN_LEN + 1];
   memset(long_token, 'a', MAX_TOKEN_LEN);
   long_token[MAX_TOKEN_LEN] = '\0';

   char line[MAX_CMDLINE];
   snprintf(line, MAX_CMDLINE, "ls %s", long_token);

   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(2, num_tokens);
   TEST_ASSERT_EQUAL_STRING("ls", tokens[0]);
   TEST_ASSERT_EQUAL_STRING(long_token, tokens[1]);
}

void test_tokenize_null_parameters(void) {
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   // Test with NULL line
   int result = tokenize_line(NULL, tokens, &num_tokens);
   TEST_ASSERT_EQUAL(-1, result);

   // Test with NULL tokens array
   result = tokenize_line("test", NULL, &num_tokens);
   TEST_ASSERT_EQUAL(-1, result);

   // Test with NULL num_tokens pointer
   result = tokenize_line("test", tokens, NULL);
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Complex Command Tests
// ============================================================================

void test_tokenize_complex_command(void) {
   char line[] = "find . -name '*.c' | grep -v test | wc -l > count.txt 2> errors.log &";
   char* tokens[MAX_TOKENS];
   int num_tokens = 0;

   int result = tokenize_line(line, tokens, &num_tokens);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(16, num_tokens);
   TEST_ASSERT_EQUAL_STRING("find", tokens[0]);
   TEST_ASSERT_EQUAL_STRING(".", tokens[1]);
   TEST_ASSERT_EQUAL_STRING("-name", tokens[2]);
   TEST_ASSERT_EQUAL_STRING("'*.c'", tokens[3]);
   TEST_ASSERT_EQUAL_STRING("|", tokens[4]);
   TEST_ASSERT_EQUAL_STRING("grep", tokens[5]);
   TEST_ASSERT_EQUAL_STRING("-v", tokens[6]);
   TEST_ASSERT_EQUAL_STRING("test", tokens[7]);
   TEST_ASSERT_EQUAL_STRING("|", tokens[8]);
   TEST_ASSERT_EQUAL_STRING("wc", tokens[9]);
   TEST_ASSERT_EQUAL_STRING("-l", tokens[10]);
   TEST_ASSERT_EQUAL_STRING(">", tokens[11]);
   TEST_ASSERT_EQUAL_STRING("count.txt", tokens[12]);
   TEST_ASSERT_EQUAL_STRING("2>", tokens[13]);
   TEST_ASSERT_EQUAL_STRING("errors.log", tokens[14]);
   TEST_ASSERT_EQUAL_STRING("&", tokens[15]);
}

// ============================================================================
// Parse Line Tests
// ============================================================================

void test_parse_line_simple_command(void) {
   char line[] = "ls -la";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("ls -la", parsed_line.original);
}

void test_parse_line_too_long(void) {
   // Create a line that's too long (over MAX_CMDLINE)
   char line[MAX_CMDLINE + 10];
   memset(line, 'a', MAX_CMDLINE + 9);
   line[MAX_CMDLINE + 9] = '\0';

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(-1, result);
}

void test_parse_line_empty(void) {
   char line[] = "";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(1, result);
}

void test_parse_line_whitespace_only(void) {
   char line[] = "   \t  \t  ";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(1, result);
}

// ============================================================================
// Token Table Tests
// ============================================================================

void test_token_table_completeness(void) {
   // Test that all expected tokens are in the table
   TEST_ASSERT_EQUAL(5, token_table_size);

   // Test each token type
   TEST_ASSERT_EQUAL_STRING("<", token_table[0].lexeme);
   TEST_ASSERT_EQUAL(TK_REDIR_IN, token_table[0].kind);
   TEST_ASSERT_EQUAL(0, token_table[0].fd);

   TEST_ASSERT_EQUAL_STRING(">", token_table[1].lexeme);
   TEST_ASSERT_EQUAL(TK_REDIR_OUT, token_table[1].kind);
   TEST_ASSERT_EQUAL(1, token_table[1].fd);

   TEST_ASSERT_EQUAL_STRING("2>", token_table[2].lexeme);
   TEST_ASSERT_EQUAL(TK_REDIR_ERR, token_table[2].kind);
   TEST_ASSERT_EQUAL(2, token_table[2].fd);

   TEST_ASSERT_EQUAL_STRING("|", token_table[3].lexeme);
   TEST_ASSERT_EQUAL(TK_PIPE, token_table[3].kind);
   TEST_ASSERT_EQUAL(-1, token_table[3].fd);

   TEST_ASSERT_EQUAL_STRING("&", token_table[4].lexeme);
   TEST_ASSERT_EQUAL(TK_AMP, token_table[4].kind);
   TEST_ASSERT_EQUAL(-1, token_table[4].fd);
}

// Test functions are called from test_runner.c

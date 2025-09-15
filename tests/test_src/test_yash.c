#include "../../include/yash.h"
#include "unity.h"
#include <string.h>

// setUp and tearDown are defined in test_runner.c

void test_command_initialization(void) {
   Command cmd;
   memset(&cmd, 0, sizeof(cmd));

   // Test that command is properly initialized
   TEST_ASSERT_NULL(cmd.argv[0]);
   TEST_ASSERT_NULL(cmd.in_file);
   TEST_ASSERT_NULL(cmd.out_file);
   TEST_ASSERT_NULL(cmd.err_file);
   TEST_ASSERT_EQUAL(0, cmd.background);
}

void test_line_initialization(void) {
   Line line;
   memset(&line, 0, sizeof(line));

   // Test that line is properly initialized
   TEST_ASSERT_EQUAL(0, line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("", line.original);
}

void test_token_kind_enum_values(void) {
   // Test that token kind enum has expected values
   TEST_ASSERT_EQUAL(0, TK_WORD);
   TEST_ASSERT_EQUAL(1, TK_REDIR_IN);
   TEST_ASSERT_EQUAL(2, TK_REDIR_OUT);
   TEST_ASSERT_EQUAL(3, TK_REDIR_ERR);
   TEST_ASSERT_EQUAL(4, TK_PIPE);
   TEST_ASSERT_EQUAL(5, TK_AMP);
}

void test_constants_values(void) {
   // Test that constants have expected values
   TEST_ASSERT_EQUAL(2001, MAX_CMDLINE);
   TEST_ASSERT_EQUAL(2000, MAX_TOKENS);
   TEST_ASSERT_EQUAL(30, MAX_TOKEN_LEN);
   TEST_ASSERT_EQUAL(64, MAX_ARGS);
   TEST_ASSERT_EQUAL(20, MAX_JOBS);
}

// Test functions are called from test_runner.c

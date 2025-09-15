#include "unity.h"
#include "../../include/yash.h"
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

void test_token_table_constants(void) {
    // Test that token table has expected entries
    TEST_ASSERT_EQUAL(5, token_table_size);
    
    // Test specific token entries
    TEST_ASSERT_EQUAL(TK_REDIR_IN, token_table[0].kind);
    TEST_ASSERT_EQUAL_STRING("<", token_table[0].lexeme);
    TEST_ASSERT_EQUAL(0, token_table[0].fd);
    
    TEST_ASSERT_EQUAL(TK_REDIR_OUT, token_table[1].kind);
    TEST_ASSERT_EQUAL_STRING(">", token_table[1].lexeme);
    TEST_ASSERT_EQUAL(1, token_table[1].fd);
    
    TEST_ASSERT_EQUAL(TK_REDIR_ERR, token_table[2].kind);
    TEST_ASSERT_EQUAL_STRING("2>", token_table[2].lexeme);
    TEST_ASSERT_EQUAL(2, token_table[2].fd);
    
    TEST_ASSERT_EQUAL(TK_PIPE, token_table[3].kind);
    TEST_ASSERT_EQUAL_STRING("|", token_table[3].lexeme);
    TEST_ASSERT_EQUAL(-1, token_table[3].fd);
    
    TEST_ASSERT_EQUAL(TK_AMP, token_table[4].kind);
    TEST_ASSERT_EQUAL_STRING("&", token_table[4].lexeme);
    TEST_ASSERT_EQUAL(-1, token_table[4].fd);
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

#pragma once

#include "yash.h"

/**
 * @brief Parse a line of input and store the result in line_out
 *
 * @param line Assume the string is properly null-terminated, no newline characters and no more than
 * 2000 characters
 * @param line_out
 * @return int
 */
int parse_line(char* line, Line* line_out);

/**
 * @brief Tokenize a line of input and store the result in tokens
 *
 * @param line Assume the string is properly null-terminated, no newline characters and no more than
 * 2000 characters
 * @param tokens
 * @param num_tokens
 * @return int
 */
int tokenize_line(char* line, char* tokens[], int* num_tokens);

/**
 * @brief Match a token in the token table
 *
 * @param ptr
 * @return static const Token*
 */
static const Token* match_token(const char* ptr);
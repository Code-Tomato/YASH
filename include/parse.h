/**
 * @file parse.h
 * @brief Parse functions for the YASH shell
 * @author Nathan Lemma
 * @date 09-16-2025
 * @details This header file contains the parse functions for the YASH shell.
 */

#pragma once

// ============================================================================
// Includes
// ============================================================================

#include "yash.h"

// ============================================================================
// Public Functions
// ============================================================================

/**
 * @brief Parse a line of input and store the result in line_out
 *
 * @param line Assume the string is properly null-terminated, no newline characters and no more than
 * 2000 characters (i.e. no more than MAX_CMDLINE - 1 characters)
 * @param line_out
 * @return 0 on success, -1 on invalid
 */
int parse_line(char* line, Line* line_out);

/**
 * @brief Tokenize a line of input and store the result in tokens
 * @note Caller must pass a mutable buffer (i.e. not a string literal)
 *
 * @param line Assume the string is properly null-terminated, no newline characters and no more than
 * 2000 characters (i.e. no more than MAX_CMDLINE - 1 characters)
 * @param tokens
 * @param num_tokens
 * @return 0 on success, -1 on invalid
 */
int tokenize_line(char* line, char* tokens[], int* num_tokens);
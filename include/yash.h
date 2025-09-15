/**
 * @file yash.h
 * @brief Core data structures and constants
 * @author Nathan Lemma
 * @date 09-15-2025
 * @details This header file contains the core data structures and constants used throughout
 *          the YASH shell implementation. It defines the Command and Line structures that
 *          represent parsed shell commands and input lines, along with various limits
 *          and configuration constants.
 */

#pragma once

// ============================================================================
// Configuration Constants
// ============================================================================

/** @brief Maximum command line length including null terminator */
#define MAX_CMDLINE 2001

/** @brief Upper bound on number of tokens from max line length */
#define MAX_TOKENS 2000

/** @brief Maximum length of a single token */
#define MAX_TOKEN_LEN 30

/** @brief Reasonable cap on arguments per command */
#define MAX_ARGS 64

/** @brief Maximum number of jobs to track */
#define MAX_JOBS 20

/** @brief File creation mode */
#define FILE_CREATE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH

// ============================================================================
// Enums
// ============================================================================

/** @brief Token kinds */
typedef enum {
   TK_WORD,      ///< regular text (cmd/arg/filename)
   TK_REDIR_IN,  ///< "<"
   TK_REDIR_OUT, ///< ">"
   TK_REDIR_ERR, ///< "2>"
   TK_PIPE,      ///< "|"
   TK_AMP,       ///< "&"
   TK_EOL        ///< End of line
} TokenKind;

// ============================================================================
// Data Structures
// ============================================================================

/**
 * @brief Represents one command with arguments and redirections.
 *
 * Invariants:
 * - argv is always NULL-terminated.
 * - argv[0] must exist for a valid command (cannot be empty).
 * - At most one of each redirection (in_file, out_file, err_file) may be set.
 * - Redirection fields are either a filename string or NULL.
 * - background == 1 is only valid if the containing Line.is_pipeline == 0.
 * - argv pointers reference the tokenized input buffer, which must outlive
 *   parsing and execution.
 */
typedef struct Command {
   char* argv[MAX_ARGS];
   char* in_file;
   char* out_file;
   char* err_file;
   int background;
} Command;

/**
 * @brief Represents a full line of user input.
 *
 * Invariants:
 * - If is_pipeline == 0: left is valid, right is unused.
 * - If is_pipeline == 1: both left and right must have argv[0].
 * - Background execution (&) is invalid when is_pipeline == 1.
 * - original always contains the raw command line string as typed,
 *   including & if present.
 */
typedef struct Line {
   int is_pipeline;
   Command left;
   Command right;
   char original[MAX_CMDLINE];
} Line;

/**
 * @brief Represents a single token from the input line.
 *
 * Invariants:
 * - lexeme is always NULL-terminated and points to the start of the token in the input line.
 * - fd is only valid for redirections (TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_ERR).
 * - fd is -1 for TK_WORD.
 */
typedef struct Token {
   const char* lexeme; ///< pointer into your in-place buffer, for TK_WORD
   TokenKind kind;     ///< kind of token
   int fd;             ///< for redirs: -1 for all kinds, 0 for "<", 1 for ">", 2 for "2>"
} Token;

/** @brief Table of tokens */
static const Token token_table[] = {
    {"<", TK_REDIR_IN, 0},
    {">", TK_REDIR_OUT, 1},
    {"2>", TK_REDIR_ERR, 2},
    {"|", TK_PIPE, -1},
    {"&", TK_AMP, -1},
};

/** @brief Size of the token table */
static const int token_table_size = sizeof(token_table) / sizeof(token_table[0]);
/**
 * @file yash.h
 * @brief Core data structures and constants
 * @author Nathan Lemma
 * @date 09-15-2025
 * @details This file contains the core data structures and constants used throughout the YASH shell
 * implementation.
 */

#pragma once

// ============================================================================
// Includes
// ============================================================================

#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

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
#define FILE_CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)

// ============================================================================
// Globals
// ============================================================================

extern volatile sig_atomic_t child_status_changed;
extern pid_t foreground_pgid;

// ============================================================================
// Enums
// ============================================================================

/** @brief Token kinds */
typedef enum {
   TK_WORD,      ///< Identifies = `Regular text`
   TK_REDIR_IN,  ///< Identifies = `<`
   TK_REDIR_OUT, ///< Identifies = `>`
   TK_REDIR_ERR, ///< Identifies = `2>`
   TK_PIPE,      ///< Identifies = `|`
   TK_AMP,       ///< Identifies = `&`
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
   char* argv[MAX_ARGS]; ///< Null-terminated array of arguments
   char* in_file;        ///< Filename for input redirection
   char* out_file;       ///< Filename for output redirection
   char* err_file;       ///< Filename for error redirection
   int background;       ///< Background execution flag
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
   int is_pipeline;            ///< Flag indicating if the line is a pipeline
   Command left;               ///< Left command
   Command right;              ///< Right command
   char original[MAX_CMDLINE]; ///< Original command line string
} Line;

// ============================================================================
// Helper Functions
// ============================================================================

/**
 * @brief Initialize a Command structure to NULL/0 values
 * @param cmd Pointer to Command structure to initialize
 */
void init_command(Command* cmd);

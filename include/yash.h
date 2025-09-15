/**
 * @file yash.h
 * @brief Main header file for YASH (Yet Another Shell) - Core data structures and constants
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
#define MAX_CMDLINE 2001   // 2000 chars + null terminator

/** @brief Upper bound on number of tokens from max line length */
#define MAX_TOKENS  2000   // upper bound from max line length

/** @brief Reasonable cap on arguments per command */
#define MAX_ARGS    64     // reasonable cap on arguments per command

// ============================================================================
// Data Structures
// ============================================================================

/**
 * @brief Represents a single command with its arguments and I/O redirection.
 * @details This structure encapsulates all the information needed to execute
 *          a single command, including its arguments, file descriptors for
 *          I/O redirection, and execution mode (foreground/background).
 * 
 * @note The argv array is null-terminated and should be allocated dynamically
 *       when parsing user input.
 */
typedef struct Command {
   char *argv[MAX_ARGS];  ///< Array of command-line arguments (null-terminated)
   int infile;            ///< File descriptor for input redirection (stdin if -1)
   int outfile;           ///< File descriptor for output redirection (stdout if -1)
   int errfile;           ///< File descriptor for error redirection (stderr if -1)
   int background;        ///< Flag: 1 if command should run in background, 0 otherwise
} Command;

/**
 * @brief Represents a single line of input from the user.
 * @details This structure holds all the information needed to execute a single
 *          line of input from the user. It can represent either a simple command
 *          or a pipeline of two commands connected by a pipe operator.
 * 
 * @note The left command is always set, while the right command is only set
 *       when ispipeline is true (indicating a pipe operation).
 */
typedef struct Line {
   int ispipeline;        /**< Pipeline flag: 1 for pipeline, 0 for single command */
   Command left;          /**< Left command in a pipeline, MUST BE SET */
   Command right;         /**< Right command in a pipeline, conditionally set */
   char *original;        /**< Original input string from the user */
} Line;

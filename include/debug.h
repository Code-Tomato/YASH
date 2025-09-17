/**
 * @file debug.h
 * @author Nathan Lemma
 * @brief Debug configuration for the YASH shell
 * @date 09-16-2025
 * @details This header file contains debug configuration and macros for the YASH shell.
 */

#pragma once

// ============================================================================
// Includes
// ============================================================================

#include <stdio.h>

// ============================================================================
// Debug Configuration
// ============================================================================

#define DEBUG 1

// ============================================================================
// Debug Macros
// ============================================================================

#ifdef DEBUG
// General debug printing
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)

// Parse-specific debug
#define DEBUG_PARSE(fmt, ...) fprintf(stderr, "[PARSE] " fmt "\n", ##__VA_ARGS__)

// Execution debug
#define DEBUG_EXEC(fmt, ...) fprintf(stderr, "[EXEC] " fmt "\n", ##__VA_ARGS__)

// Job control debug
#define DEBUG_JOBS(fmt, ...) fprintf(stderr, "[JOBS] " fmt "\n", ##__VA_ARGS__)

// Signal handling debug
#define DEBUG_SIGNALS(fmt, ...) fprintf(stderr, "[SIGNALS] " fmt "\n", ##__VA_ARGS__)

// Command structure debug
#define DEBUG_COMMAND(cmd)                                                                         \
   do {                                                                                            \
      DEBUG_PARSE("┌─ Parsed Command Structure");                                                  \
      DEBUG_PARSE("│  Command: %s", (cmd)->argv[0] ? (cmd)->argv[0] : "NULL");                     \
      DEBUG_PARSE("│  Background: %s", (cmd)->background ? "YES" : "NO");                          \
      DEBUG_PARSE("│  Input file: %s", (cmd)->in_file ? (cmd)->in_file : "stdin");                 \
      DEBUG_PARSE("│  Output file: %s", (cmd)->out_file ? (cmd)->out_file : "stdout");             \
      DEBUG_PARSE("│  Error file: %s", (cmd)->err_file ? (cmd)->err_file : "stderr");              \
      fprintf(stderr, "[PARSE] │  Arguments: ");                                                   \
      for (int i = 0; i < MAX_ARGS && (cmd)->argv[i]; i++) {                                       \
         fprintf(stderr, "\"%s\" ", (cmd)->argv[i]);                                               \
      }                                                                                            \
      fprintf(stderr, "\n");                                                                       \
      DEBUG_PARSE("└─ End Command Structure");                                                     \
   } while (0)

// Process debug
#define DEBUG_PROCESS(pid, status) DEBUG_EXEC("Process PID %d, status: %d", pid, status)

// Job debug
#define DEBUG_JOB(job_id, pid, status) DEBUG_JOBS("Job %d: PID %d, status: %d", job_id, pid, status)

// Signal debug
#define DEBUG_SIGNAL(sig, action) DEBUG_SIGNALS("Signal %d: %s", sig, action)

#else
// No-op versions when DEBUG is disabled
#define DEBUG_PRINT(fmt, ...)
#define DEBUG_PARSE(fmt, ...)
#define DEBUG_EXEC(fmt, ...)
#define DEBUG_JOBS(fmt, ...)
#define DEBUG_SIGNALS(fmt, ...)
#define DEBUG_COMMAND(cmd)
#define DEBUG_PROCESS(pid, status)
#define DEBUG_JOB(job_id, pid, status)
#define DEBUG_SIGNAL(sig, action)
#endif

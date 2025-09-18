/**
 * @file signals.h
 * @author Nathan Lemma
 * @brief Signal handling functions for the YASH shell
 * @date 09-16-2025
 * @details This header file contains the signal handling functions for the YASH shell.
 */

#pragma once

// ============================================================================
// Public Functions
// ============================================================================

/**
 * @brief Setup signal handlers for SIGINT, SIGTSTP, and SIGCHLD
 *
 * @param void
 * @return void
 */
void setup_signal_handlers(void);

/**
 * @brief SIGCHLD handler
 *
 * @param sig
 * @return void
 */
void sigchld_handler(int sig);

/**
 * @brief SIGINT handler (Ctrl-C)
 *
 * @param sig
 * @return void
 */
void sigint_handler(int sig);

/**
 * @brief SIGTSTP handler (Ctrl-Z)
 *
 * @param sig
 * @return void
 */
void sigtstp_handler(int sig);

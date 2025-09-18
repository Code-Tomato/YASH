/**
 * @file signals.c
 * @author Nathan Lemma
 * @brief Signal handling functions for the YASH shell
 * @date 09-16-2025
 * @details This file contains the signal handling functions for the YASH shell.
 */

// ============================================================================
// Includes
// ============================================================================

#include "../include/signals.h"
#include "../include/debug.h"
#include "../include/yash.h"

// ============================================================================
// Globals
// ============================================================================

volatile sig_atomic_t child_status_changed = 0;
pid_t foreground_pgid = 0;

// ============================================================================
// Public Functions
// ============================================================================

void sigchld_handler(int sig) {
   (void)sig; // Suppress unused parameter warning
   child_status_changed = 1;
}

void sigint_handler(int sig) {
   (void)sig; // Suppress unused parameter warning
   DEBUG_PRINT("SIGINT handler called, foreground_pgid = %d", foreground_pgid);
   if (foreground_pgid > 0) {
      DEBUG_PRINT("Sending SIGINT to process group %d", -foreground_pgid);
      kill(-foreground_pgid, SIGINT);
   } else {
      DEBUG_PRINT("No foreground process group, ignoring SIGINT");
   }
}

void sigtstp_handler(int sig) {
   (void)sig; // Suppress unused parameter warning
   DEBUG_PRINT("SIGTSTP handler called, foreground_pgid = %d", foreground_pgid);
   if (foreground_pgid > 0) {
      DEBUG_PRINT("Sending SIGTSTP to process group %d", -foreground_pgid);
      kill(-foreground_pgid, SIGTSTP);
   } else {
      DEBUG_PRINT("No foreground process group, ignoring SIGTSTP");
   }
}

void setup_signal_handlers(void) {
   struct sigaction sa;

   // Set up SIGCHLD handler
   sa.sa_handler = sigchld_handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART; // Restart system calls if interrupted
   sigaction(SIGCHLD, &sa, NULL);

   // Set up SIGINT handler (Ctrl-C)
   sa.sa_handler = sigint_handler;
   sigaction(SIGINT, &sa, NULL);

   // Set up SIGTSTP handler (Ctrl-Z)
   sa.sa_handler = sigtstp_handler;
   sigaction(SIGTSTP, &sa, NULL);
}

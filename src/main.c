/**
 * @file main.c
 * @brief Main function for the YASH shell
 * @author Nathan Lemma
 * @date 09-16-2025
 * @details This file contains the main function for the YASH shell.
 */

// ============================================================================
// Includes
// ============================================================================

#include "../include/debug.h"
#include "../include/exec.h"
#include "../include/jobs.h"
#include "../include/parse.h"
#include "../include/signals.h"
#include "../include/yash.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// ============================================================================
// Main Function
// ============================================================================

int main() {

   setup_signal_handlers();
   jobs_init();

   DEBUG_PRINT("YASH shell starting");

   while (1) {
      // Check for any background job state changes first
      if (child_status_changed) {
         child_status_changed = 0;
         pid_t pid;
         int status;
         while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
            pid_t pg = getpgid(pid);
            pid_t key = (pg == -1) ? pid : pg; // fallback to PID if getpgid fails
            if (WIFSTOPPED(status)) {
               jobs_mark(key, JOB_STOPPED);
            } else if (WIFCONTINUED(status)) {
               jobs_mark(key, JOB_RUNNING);
            } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
               jobs_mark(key, JOB_DONE);
            }
            DEBUG_PRINT("Background child %d changed state", pid);
         }
      }

      // Reap done jobs and print "Done" messages before prompt
      jobs_reap_done_and_print();

      printf("# ");
      fflush(stdout);

      char buffer[MAX_CMDLINE];
      if (!fgets(buffer, sizeof(buffer), stdin)) {
         DEBUG_PRINT("EOF received, exiting shell");
         break;
      }

      size_t len = strlen(buffer);
      if (len == MAX_CMDLINE - 1 && buffer[len - 1] != '\n') {
         int c;
         while ((c = getchar()) != EOF && c != '\n');
         DEBUG_PRINT("Command too long\n");
         continue;
      }

      // Remove the newline character
      if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';

      // If the buffer is empty, reprompt
      if (buffer[0] == '\0') continue;

      Line line;
      memset(&line, 0, sizeof(line));

      int result = parse_line(buffer, &line);
      if (result == 0) {
         DEBUG_PRINT("Parsing successful, executing command");
         int exec_result = execute_line(&line);
         if (exec_result == -1) {
            // Internal error (pipe/fork/etc). Log only, no user newline here.
            DEBUG_PRINT("Execution internal error");
         }
         // Check if any child processes changed state
         if (child_status_changed) {
            child_status_changed = 0; // Reset the flag
            pid_t pid;
            int status;
            // Wait for any child that changed state (died, stopped, continued)
            while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
               // Child process changed state - update job table
               pid_t pg = getpgid(pid);
               pid_t key = (pg == -1) ? pid : pg; // fallback to PID if getpgid fails
               if (WIFSTOPPED(status)) {
                  jobs_mark(key, JOB_STOPPED);
               } else if (WIFCONTINUED(status)) {
                  jobs_mark(key, JOB_RUNNING);
               } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
                  jobs_mark(key, JOB_DONE);
               }
               DEBUG_PRINT("Child %d changed state", pid);
            }
         }
      } else if (result == -1) {
         DEBUG_PRINT("Parsing failed, invalid command");
         fflush(stdout);
      }
   }
   return 0;
}

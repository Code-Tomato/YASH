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
#include "../include/parse.h"
#include "../include/yash.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

// ============================================================================
// Main Function
// ============================================================================

int main() {
   DEBUG_PRINT("YASH shell starting");

   while (1) {
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
      } else if (result == -1) {
         DEBUG_PRINT("Parsing failed, invalid command");
         putchar('\n');
         fflush(stdout);
      }
   }
   return 0;
}

#include "../include/parse.h"
#include "../include/yash.h"
#include <stdio.h>
#include <string.h>

// int main() {
//    while (1) {
//       printf("# ");
//       fflush(stdout);

//       char buffer[MAX_CMDLINE];
//       if (!fgets(buffer, sizeof(buffer), stdin)) {
//          break;
//       }

//       size_t len = strlen(buffer);
//       if (len == MAX_CMDLINE - 1 && buffer[len - 1] != '\n') {
//          int c;
//          while ((c = getchar()) != EOF && c != '\n');
//          // printf("Command too long, try again\n");
//          continue;
//       }

//       // Remove the newline character
//       if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';

//       // If the buffer is empty, reprompt
//       if (buffer[0] == '\0') continue;

//       Line line;
//       memset(&line, 0, sizeof(line));

//       int result = parse_line(buffer, &line);
//       if (result == 0) {
//          // TODO: Sprint 2
//       } else if (result == -1) {
//          putchar('\n');
//          fflush(stdout);
//       }
//    }
//    return 0;
// }

// Temporary main function for testing parse_line
int main() {
   printf("=== Testing parse_line function ===\n\n");

   // Test cases
   char* test_cases[] = {
       "ls   -la",
       "echo hello world",
       "cat file.txt > output.txt",
       "grep pattern < input.txt",
       "ls 2> error.log",
       "ls | grep txt",
       "ls -la &",
       "cat file.txt | grep pattern > result.txt",
       "echo hello 2> /dev/null",
       "ls -la | grep txt | wc -l",
       "",    // empty line
       "   ", // whitespace only
       "01234567890123456789012345678901234567890123456789012345678901234567890123456789",
       NULL};

   for (int i = 0; test_cases[i] != NULL; i++) {
      printf("Test %d: \"%s\"\n", i + 1, test_cases[i]);

      Line line;
      memset(&line, 0, sizeof(line));

      // Create a mutable copy of the input string
      char mutable_buffer[MAX_CMDLINE];
      strncpy(mutable_buffer, test_cases[i], MAX_CMDLINE - 1);
      mutable_buffer[MAX_CMDLINE - 1] = '\0';

      int result = parse_line(mutable_buffer, &line);

      printf("  Return value: %d\n", result);

      if (result == 0) {
         printf("  Parsing successful!\n");
         printf("  is_pipeline: %d\n", line.is_pipeline);
         printf("  original: \"%s\"\n", line.original);

         // Print left command
         printf("  Left command:\n");
         printf("    background: %d\n", line.left.background);
         printf("    in_file: %s\n", line.left.in_file ? line.left.in_file : "NULL");
         printf("    out_file: %s\n", line.left.out_file ? line.left.out_file : "NULL");
         printf("    err_file: %s\n", line.left.err_file ? line.left.err_file : "NULL");
         printf("    argv: ");
         for (int j = 0; j < MAX_ARGS && line.left.argv[j] != NULL; j++) {
            printf("\"%s\" ", line.left.argv[j]);
         }
         printf("\n");

         // Print right command if it's a pipeline
         if (line.is_pipeline) {
            printf("  Right command:\n");
            printf("    background: %d\n", line.right.background);
            printf("    in_file: %s\n", line.right.in_file ? line.right.in_file : "NULL");
            printf("    out_file: %s\n", line.right.out_file ? line.right.out_file : "NULL");
            printf("    err_file: %s\n", line.right.err_file ? line.right.err_file : "NULL");
            printf("    argv: ");
            for (int j = 0; j < MAX_ARGS && line.right.argv[j] != NULL; j++) {
               printf("\"%s\" ", line.right.argv[j]);
            }
            printf("\n");
         }
      } else {
         printf("  Parsing failed or incomplete line\n");
      }

      printf("\n");
   }

   printf("=== Interactive testing ===\n");
   printf("Enter commands to test (or 'quit' to exit):\n");

   char buffer[MAX_CMDLINE];
   while (1) {
      printf("> ");
      fflush(stdout);

      if (!fgets(buffer, sizeof(buffer), stdin)) {
         break;
      }

      // Remove newline
      size_t len = strlen(buffer);
      if (len > 0 && buffer[len - 1] == '\n') {
         buffer[len - 1] = '\0';
      }

      // Check for quit command
      if (strcmp(buffer, "quit") == 0) {
         break;
      }

      // Skip empty lines
      if (buffer[0] == '\0') {
         continue;
      }

      Line line;
      memset(&line, 0, sizeof(line));

      // Create a mutable copy of the input string
      char mutable_buffer[MAX_CMDLINE];
      strncpy(mutable_buffer, buffer, MAX_CMDLINE - 1);
      mutable_buffer[MAX_CMDLINE - 1] = '\0';

      int result = parse_line(mutable_buffer, &line);
      printf("Result: %d\n", result);

      if (result == 0) {
         printf("Pipeline: %d\n", line.is_pipeline);
         printf("Original: \"%s\"\n", line.original);

         printf("Left: ");
         for (int i = 0; i < MAX_ARGS && line.left.argv[i] != NULL; i++) {
            printf("\"%s\" ", line.left.argv[i]);
         }
         printf("(bg: %d)\n", line.left.background);

         if (line.is_pipeline) {
            printf("Right: ");
            for (int i = 0; i < MAX_ARGS && line.right.argv[i] != NULL; i++) {
               printf("\"%s\" ", line.right.argv[i]);
            }
            printf("(bg: %d)\n", line.right.background);
         }
      }
      printf("\n");
   }

   return 0;
}

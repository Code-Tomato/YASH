#include "../include/parse.h"
#include "../include/yash.h"
#include <stdio.h>
#include <string.h>

int main() {
   while (1) {
      printf("# ");
      fflush(stdout);

      char buffer[MAX_CMDLINE];
      if (!fgets(buffer, sizeof(buffer), stdin)) {
         break;
      }

      size_t len = strlen(buffer);
      if (len == MAX_CMDLINE - 1 && buffer[len - 1] != '\n') {
         int c;
         while ((c = getchar()) != EOF && c != '\n');
         // printf("Command too long, try again\n");
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
         // TODO: Sprint 2
      } else if (result == -1) {
         putchar('\n');
         fflush(stdout);
      }
   }
   return 0;
}

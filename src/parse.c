#include "../include/parse.h"
#include <stdio.h>
#include <string.h>

int parse_line(char* line, Line* line_out) {

   if (strnlen(line, MAX_CMDLINE) == MAX_CMDLINE) {
      // Means the input filled the buffer without room for '\0' or is more than 2000 characters
      return -1;
   }

   snprintf(line_out->original, MAX_CMDLINE, "%s", line);

   char* tokens[MAX_TOKENS];
   int num_tokens = 0;
   tokenize_line(line, tokens, &num_tokens);

   if (num_tokens == 0) return 1;

   for (int i = 0; i < num_tokens; i++) {
      printf("token[%d] = \"%s\"\n", i, tokens[i]);
   }
   return 0;
}

int tokenize_line(char* line, char* tokens[], int* num_tokens) {
   if (!line || !tokens || !num_tokens) return -1;

   int n = 0;
   char* ptr = line;

   while (*ptr) {
      // Skip leading whitespace
      while (*ptr == ' ' || *ptr == '\t') {
         *ptr = '\0';
         ptr++;
      }

      if (*ptr == '\0') break;

      if (n >= MAX_TOKENS) return -1;
      tokens[n++] = ptr; // Start of the token

      // Run to the end of the token
      while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;

      if (*ptr == ' ' || *ptr == '\t') {
         *ptr = '\0';
         ptr++;
      }
   }
   *num_tokens = n;
   return 0;
}

static const Token* match_token(const char* ptr) {
   for (int i = 0; i < token_table_size; i++) {
      if (strcmp(ptr, token_table[i].lexeme) == 0) {
         return &token_table[i];
      }
   }
   return NULL;
}
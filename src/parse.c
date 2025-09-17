/**
 * @file parse.c
 * @brief Parse functions for the YASH shell
 * @author Nathan Lemma
 * @date 09-16-2025
 * @details This file contains the parse functions for the YASH shell.
 */

// ============================================================================
// Includes
// ============================================================================

#include "../include/parse.h"
#include "../include/debug.h"
#include "../include/yash.h"
#include <stdio.h>
#include <string.h>

// ============================================================================
// Data Structures
// ============================================================================

// ============================================================================
// Static Functions
// ============================================================================

/**
 * @brief Get the kind of a token
 *
 * @param t Token string
 * @return TokenKind enum
 */
static inline TokenKind kind_of(const char* t) {
   if (t[0] == '2' && t[1] == '>' && t[2] == '\0') return TK_REDIR_ERR;
   if (t[0] == '<' && t[1] == '\0') return TK_REDIR_IN;
   if (t[0] == '>' && t[1] == '\0') return TK_REDIR_OUT;
   if (t[0] == '|' && t[1] == '\0') return TK_PIPE;
   if (t[0] == '&' && t[1] == '\0') return TK_AMP;
   return TK_WORD;
}

/**
 * @brief Analyze the structure of a line of tokens
 *
 * @param tokens
 * @param n Length
 * @param pipe_idx (-1 if none)
 * @param has_amp (0/1)
 * @return 0 on success, -1 on invalid
 */
static int analyze_structure(char* tokens[], int n, int* pipe_idx, int* has_amp) {
   if (n == 0 || kind_of(tokens[0]) != TK_WORD) return -1;

   *pipe_idx = -1;
   *has_amp = 0;
   for (int i = 1; i < n; i++) {
      switch (kind_of(tokens[i])) {
      case TK_PIPE:
         if (*pipe_idx != -1 || *has_amp) return -1;
         *pipe_idx = i;
         break;
      case TK_AMP:
         if (i != n - 1 || *has_amp) return -1;
         *has_amp = 1;
         break;
      default:
         break;
      }
   }
   if (*pipe_idx != -1 && *has_amp) return -1;
   return 0;
}

/**
 * @brief Fill a Command from tokens[lo..hi)
 *
 * @param cmd
 * @param tokens
 * @param lo
 * @param hi
 * @return 0 on success, -1 on invalid
 */
static int fill_command(Command* cmd, char* tokens[], int lo, int hi) {

   int args_closed = 0;

   cmd->in_file = NULL;
   cmd->out_file = NULL;
   cmd->err_file = NULL;
   cmd->background = 0;
   for (int j = 0; j < MAX_ARGS; j++) {
      cmd->argv[j] = NULL;
   }

   int k = 0;
   for (int i = lo; i < hi; i++) {
      switch (kind_of(tokens[i])) {
      case TK_WORD:
         if (args_closed == 1) return -1;
         if (k >= MAX_ARGS - 1) return -1;
         cmd->argv[k++] = tokens[i];
         break;
      case TK_REDIR_IN:
         args_closed = 1;
         if (i + 1 >= hi || cmd->in_file || kind_of(tokens[i + 1]) != TK_WORD) return -1;
         cmd->in_file = tokens[i + 1];
         i++;
         break;
      case TK_REDIR_OUT:
         args_closed = 1;
         if (i + 1 >= hi || cmd->out_file || kind_of(tokens[i + 1]) != TK_WORD) return -1;
         cmd->out_file = tokens[i + 1];
         i++;
         break;
      case TK_REDIR_ERR:
         args_closed = 1;
         if (i + 1 >= hi || cmd->err_file || kind_of(tokens[i + 1]) != TK_WORD) return -1;
         cmd->err_file = tokens[i + 1];
         i++;
         break;
      default:
         return -1;
      }
   }
   if (cmd->argv[0] == NULL) return -1;
   cmd->argv[k] = NULL;

   return 0;
}

// ============================================================================
// Public Functions
// ============================================================================

int parse_line(char* line, Line* line_out) {
   // Check for NULL input
   if (!line || !line_out) {
      return -1;
   }

   DEBUG_PARSE("Parsing line: \"%s\"", line);

   // Parse the line
   if (strnlen(line, MAX_CMDLINE) == MAX_CMDLINE) {
      // Means the input filled the buffer without room for '\0' or is more than 2000 characters
      DEBUG_PARSE("Line too long (>= %d characters)", MAX_CMDLINE);
      return -1;
   }

   snprintf(line_out->original, MAX_CMDLINE, "%s", line);

   char* tokens[MAX_TOKENS];
   int num_tokens = 0;
   int result = tokenize_line(line, tokens, &num_tokens);
   if (result == -1) {
      DEBUG_PARSE("Tokenization failed");
      return -1;
   }

   if (num_tokens == 0) {
      DEBUG_PARSE("No tokens found");
      return -1;
   }

   DEBUG_PARSE("┌─ Tokenization Results (%d tokens)", num_tokens);
   for (int i = 0; i < num_tokens; i++) {
      DEBUG_PARSE("│  [%d] \"%s\"", i, tokens[i]);
   }
   DEBUG_PARSE("└─ End Tokenization");

   // Match the tokens
   int pipe_idx = -1;
   int has_amp = 0;
   if (analyze_structure(tokens, num_tokens, &pipe_idx, &has_amp) == -1) {
      DEBUG_PARSE("Invalid command structure");
      return -1;
   }

   line_out->is_pipeline = (pipe_idx != -1);
   DEBUG_PARSE("Command type: %s", line_out->is_pipeline ? "pipeline" : "simple");

   if (pipe_idx == -1) {
      int hi = has_amp ? num_tokens - 1 : num_tokens;
      if (fill_command(&line_out->left, tokens, 0, hi) == -1) {
         DEBUG_PARSE("Failed to fill simple command");
         return -1;
      }
      line_out->left.background = has_amp ? 1 : 0;
      DEBUG_PARSE("├─ Simple Command:");
      DEBUG_COMMAND(&line_out->left);
   } else {
      int left_lo = 0, left_hi = pipe_idx;
      int right_lo = pipe_idx + 1;
      int right_hi = num_tokens;
      if (left_hi <= left_lo || right_hi <= right_lo) {
         DEBUG_PARSE("Invalid pipeline structure");
         return -1;
      }
      if (fill_command(&line_out->left, tokens, left_lo, left_hi) == -1) {
         DEBUG_PARSE("Failed to fill left command in pipeline");
         return -1;
      }
      if (fill_command(&line_out->right, tokens, right_lo, right_hi) == -1) {
         DEBUG_PARSE("Failed to fill right command in pipeline");
         return -1;
      }
      line_out->left.background = 0;  // Pipelines can't be background
      line_out->right.background = 0; // Pipelines can't be background
      DEBUG_PARSE("├─ Left Command (Pipeline):");
      DEBUG_COMMAND(&line_out->left);
      DEBUG_PARSE("├─ Right Command (Pipeline):");
      DEBUG_COMMAND(&line_out->right);
   }

   DEBUG_PARSE("Parsing completed successfully");
   return 0;
}

int tokenize_line(char* line, char* tokens[], int* num_tokens) {
   if (!line || !tokens || !num_tokens) return -1;

   int n = 0;
   char* ptr = line;

   while (*ptr) {
      // Skip leading whitespace
      while (*ptr == ' ' || *ptr == '\t') {
         ptr++;
      }

      if (*ptr == '\0') break;

      if (n >= MAX_TOKENS) return -1;
      tokens[n++] = ptr; // Start of the token

      // Run to the end of the token
      int i = 0;
      while (*ptr && *ptr != ' ' && *ptr != '\t') {
         ptr++;
         i++;
         if (i > MAX_TOKEN_LEN) return -1; // allow length <= MAX_TOKEN_LEN (30), reject 31+
      }

      // End of the token
      if (*ptr == ' ' || *ptr == '\t') {
         *ptr = '\0';
         ptr++;
      }
   }
   *num_tokens = n;
   return 0;
}

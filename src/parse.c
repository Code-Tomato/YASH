// ============================================================================
// Includes
// ============================================================================

#include "../include/parse.h"
#include "../include/yash.h"
#include <stdio.h>
#include <string.h>

// ============================================================================
// Defines
// ============================================================================

#define DEBUG 1

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
         if (k >= MAX_ARGS - 1) return -1;
         cmd->argv[k++] = tokens[i];
         break;
      case TK_REDIR_IN:
         if (i + 1 >= hi || cmd->in_file || kind_of(tokens[i + 1]) != TK_WORD) return -1;
         cmd->in_file = tokens[i + 1];
         i++;
         break;
      case TK_REDIR_OUT:
         if (i + 1 >= hi || cmd->out_file || kind_of(tokens[i + 1]) != TK_WORD) return -1;
         cmd->out_file = tokens[i + 1];
         i++;
         break;
      case TK_REDIR_ERR:
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

   // Parse the line
   if (strnlen(line, MAX_CMDLINE) == MAX_CMDLINE) {
      // Means the input filled the buffer without room for '\0' or is more than 2000 characters
      return -1;
   }

   snprintf(line_out->original, MAX_CMDLINE, "%s", line);

   char* tokens[MAX_TOKENS];
   int num_tokens = 0;
   int result = tokenize_line(line, tokens, &num_tokens);
   if (result == -1) return -1;

   if (num_tokens == 0) return -1;

   if (DEBUG) {
      for (int i = 0; i < num_tokens; i++) {
         fprintf(stderr, "token[%d] = \"%s\"\n", i, tokens[i]);
      }
   }

   // Match the tokens
   int pipe_inx = -1;
   int has_amp = 0;
   if (analyze_structure(tokens, num_tokens, &pipe_inx, &has_amp) == -1) return -1;

   line_out->is_pipeline = (pipe_inx != -1);

   if (pipe_inx == -1) {
      int hi = has_amp ? num_tokens - 1 : num_tokens;
      if (fill_command(&line_out->left, tokens, 0, hi) == -1) return -1;
      line_out->left.background = has_amp ? 1 : 0;
   } else {
      int left_lo = 0, left_hi = pipe_inx;
      int right_lo = pipe_inx + 1;
      int right_hi = num_tokens;
      if (left_hi <= left_lo || right_hi <= right_lo) return -1;
      if (fill_command(&line_out->left, tokens, left_lo, left_hi) == -1) return -1;
      if (fill_command(&line_out->right, tokens, right_lo, right_hi) == -1) return -1;
      line_out->left.background = 0;  // Pipelines can't be background
      line_out->right.background = 0; // Pipelines can't be background
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
         if (i > MAX_TOKEN_LEN) return -1;
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

/**
 * @file exec.c
 * @author Nathan Lemma
 * @brief Execution functions for the YASH shell
 * @date 09-16-2025
 * @details This file contains the execution functions for the YASH shell.
 */

// ============================================================================
// Includes
// ============================================================================

#include "../include/exec.h"
#include "../include/debug.h"
#include "../include/yash.h"

// ============================================================================
// Static Functions
// ============================================================================

static int execute_command(const Command* cmd) {
   // TODO: Implement
   return 0;
}
static int execute_pipeline(const Command* left, const Command* right) {
   // TODO: Implement
   return 0;
}

// ============================================================================
// Public Functions
// ============================================================================

int execute_line(Line* line) {
   return line->is_pipeline ? execute_pipeline(&line->left, &line->right)
                            : execute_command(&line->left);
}

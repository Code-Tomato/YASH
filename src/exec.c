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
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// ============================================================================
// Static Functions
// ============================================================================

/**
 * @brief Checks if the inputs work.
 * @note Didn't want to change setup_redirections()
 *
 * @param c
 * @return int
 */
static int check_input_exists(const Command* c) {
   if (!c || !c->in_file) return 0;
   int fd = open(c->in_file, O_RDONLY);
   if (fd < 0) return -1;
   close(fd);
   return 0;
}

/**
 * @brief Set up redirections. Didn't wanna rewrite it 3 times
 *
 * @param cmd
 * @param p_in_fd
 * @param p_out_fd
 */
static void setup_redirections(const Command* cmd, int p_in_fd, int p_out_fd) {

   // Restore the regular signals so that the child process can be cancelled
   signal(SIGINT, SIG_DFL);
   signal(SIGTSTP, SIG_DFL); // Child should handle SIGTSTP with default behavior
   signal(SIGPIPE, SIG_DFL);

   if (p_in_fd != -1) {
      dup2(p_in_fd, STDIN_FILENO);
      close(p_in_fd);
   }
   if (p_out_fd != -1) {
      dup2(p_out_fd, STDOUT_FILENO);
      close(p_out_fd);
   }

   if (cmd->in_file) {
      int fd = open(cmd->in_file, O_RDONLY);
      if (fd < 0) {
         DEBUG_EXEC("Opening file failed: %s", strerror(errno));
         _exit(errno);
      }
      dup2(fd, STDIN_FILENO);
      close(fd);
   }
   if (cmd->out_file) {
      int fd = open(cmd->out_file, O_WRONLY | O_CREAT | O_TRUNC, FILE_CREATE_MODE);
      if (fd < 0) {
         DEBUG_EXEC("Opening file failed: %s", strerror(errno));
         _exit(errno);
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
   }
   if (cmd->err_file) {
      int fd = open(cmd->err_file, O_WRONLY | O_CREAT | O_TRUNC, FILE_CREATE_MODE);
      if (fd < 0) {
         DEBUG_EXEC("Opening file failed: %s", strerror(errno));
         _exit(errno);
      }
      dup2(fd, STDERR_FILENO);
      close(fd);
   }
}

/**
 * @brief Executes non-piped commands
 *
 * @param cmd
 * @return int
 */
static int execute_command(const Command* cmd) {

   DEBUG_EXEC("Executing the command!!!");

   if (!cmd || !cmd->argv[0]) {
      DEBUG_EXEC("cmd or cmd->argv is NULL");
      return -1;
   }

   if (check_input_exists(cmd) == -1) {
      putchar('\n');
      fflush(stdout);
      return 0;
   }

   pid_t pid = fork();

   if (pid < 0) { // fork() failed
      DEBUG_EXEC("fork() failed (execute_command): %s", strerror(errno));
      return -1;
   }

   if (pid == 0) {
      // Child process
      DEBUG_EXEC("Child process starting, PID: %d", getpid());
      setpgid(0, 0);
      DEBUG_EXEC("Child process set process group to %d", getpgid(0));
      setup_redirections(cmd, -1, -1);

      DEBUG_EXEC("Child process executing command");
      execvp(cmd->argv[0], cmd->argv);

      // You shouldn't be here :(
      DEBUG_EXEC("execvp failed: %s", strerror(errno));
      if (errno == ENOENT) _exit(127);
      _exit(126);
   } else {
      // Parent process
      setpgid(pid, pid);
      foreground_pgid = pid;
      DEBUG_EXEC("Parent process, child PID: %d, foreground_pgid set to %d. Waiting...",
                 pid,
                 foreground_pgid);

      int status;
      waitpid(pid, &status, WUNTRACED);
      DEBUG_EXEC("Child process finished, clearing foreground_pgid");
      foreground_pgid = 0;

      if (WIFSTOPPED(status)) {
         // TODO: hook this into jobs later. For now, return to prompt.
         // (Do NOT print the command here per spec.)
         return 0;
      }
      if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
         putchar('\n');
         fflush(stdout);
      }

      // TODO: For now I ignored stopped/continue
   }

   return 0;
}

/**
 * @brief Execute piped commands
 *
 * @param left
 * @param right
 * @return int
 */
static int execute_pipeline(const Command* left, const Command* right) {
   // Can assume:
   // - left is not NULL and left->argv[0] exists
   // both left and right are guaranteed validright->argv[0] for pipelines
   if (!left || !right) {
      DEBUG_EXEC("Parser error: left or right is NULL");
      return -1;
   }

   if (check_input_exists(left) == -1 || check_input_exists(right) == -1) {
      putchar('\n');
      fflush(stdout);
      return 0;
   }

   DEBUG_EXEC("Executing pipeline");
   DEBUG_EXEC("Left command:");
   DEBUG_COMMAND(left);
   DEBUG_EXEC("Right command:");
   DEBUG_COMMAND(right);

   int p_fd[2]; // 0 = read, 1 = write
   if (pipe(p_fd) < 0) {
      DEBUG_EXEC("pipe() failed: %s", strerror(errno));
      return -1;
   }

   pid_t left_pid = fork();
   if (left_pid < 0) { // fork() failed
      DEBUG_EXEC("fork() failed (execute_pipeline left): %s", strerror(errno));
      return -1;
   }

   // Left Process
   if (left_pid == 0) {
      DEBUG_EXEC("Left child running.");
      setpgid(0, 0);

      close(p_fd[0]);
      setup_redirections(left, -1, p_fd[1]);

      DEBUG_EXEC("Child process executing command");
      execvp(left->argv[0], left->argv);

      // You shouldn't be here :(
      DEBUG_EXEC("execvp failed: %s", strerror(errno));
      if (errno == ENOENT) _exit(127);
      _exit(126);
   }

   pid_t right_pid = fork();
   if (right_pid < 0) { // fork() failed
      DEBUG_EXEC("fork() failed (execute_pipeline left): %s", strerror(errno));
      return -1;
   }

   // Right Process
   if (right_pid == 0) {
      DEBUG_EXEC("Right child running.");
      setpgid(0, left_pid);

      close(p_fd[1]);
      setup_redirections(right, p_fd[0], -1);

      DEBUG_EXEC("Child process executing command");
      execvp(right->argv[0], right->argv);

      // You shouldn't be here :(
      DEBUG_EXEC("execvp failed: %s", strerror(errno));
      if (errno == ENOENT) _exit(127);
      _exit(126);
   }

   // Parent Process
   close(p_fd[0]);
   close(p_fd[1]);
   setpgid(left_pid, left_pid);
   setpgid(right_pid, left_pid);

   foreground_pgid = left_pid;

   int stL = 0, stR = 0;
   waitpid(left_pid, &stL, WUNTRACED);
   waitpid(right_pid, &stR, WUNTRACED);
   foreground_pgid = 0;

   if (WIFSTOPPED(stL) || WIFSTOPPED(stR)) {
      // Whole group is stopped; later this becomes a job entry.
      return 0;
   }

   int left_not_found = WIFEXITED(stL) && WEXITSTATUS(stL) == 127;
   int right_not_found = WIFEXITED(stR) && WEXITSTATUS(stR) == 127;
   if (left_not_found || right_not_found) {
      DEBUG_EXEC("Left or right command not found");
      putchar('\n');
      fflush(stdout);
   }
   return 0;
}

// ============================================================================
// Public Functions
// ============================================================================

int execute_line(Line* line) {
   // Can assume:
   // - line is not NULL
   // - line->is_pipeline is correctly set
   // - line->left is always valid
   // - line->right is valid if is_pipeline==1, or NULL-initialized if is_pipeline==0
   if (line->is_pipeline) {
      return execute_pipeline(&line->left, &line->right);
   } else {
      DEBUG_EXEC("No pipeline - executing single command");
      return execute_command(&line->left);
   }
}

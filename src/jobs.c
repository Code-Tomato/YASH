/**
 * @file jobs.c
 * @author Nathan Lemma
 * @brief Job control functions for the YASH shell
 * @date 09-16-2025
 * @details This file contains the job control functions for the YASH shell.
 */

// ============================================================================
// Includes
// ============================================================================

#include "../include/jobs.h"
#include "../include/debug.h"
#include "../include/yash.h"
#include <stdio.h>
#include <string.h>

// ============================================================================
// Static Globals
// ============================================================================

static Job job_table[MAX_JOBS];
static int job_count;

// ============================================================================
// Public Functions
// ============================================================================

void jobs_init(void) {
   // Zero the table
   for (int i = 0; i < MAX_JOBS; i++) {
      job_table[i].id = 0;
      job_table[i].pgid = 0;
      job_table[i].cmdline[0] = '\0';
      job_table[i].status = JOB_DONE;
      job_table[i].is_background = 0;
   }

   // Reset counters
   job_count = 0;
}

int jobs_add(pid_t pgid, const char* cmdline, int is_background) {
   // Check capacity limit
   if (job_count >= MAX_JOBS) {
      return -1; // Refuse to add - at capacity
   }

   // Find the current maximum ID among active jobs
   int max_id = 0;
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].status != JOB_DONE && job_table[i].id > max_id) {
         max_id = job_table[i].id;
      }
   }
   int new_id = max_id + 1;

   // Find empty slot
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].status == JOB_DONE) {
         // Bash-style numbering: new job number = max current ID + 1
         job_table[i].id = new_id;
         job_table[i].pgid = pgid;
         snprintf(job_table[i].cmdline, MAX_CMDLINE, "%s", cmdline);
         job_table[i].status = is_background ? JOB_RUNNING : JOB_STOPPED;
         job_table[i].is_background = is_background;
         job_count++;
         return job_table[i].id;
      }
   }

   // No empty slots available (shouldn't happen if job_count < MAX_JOBS)
   return -1;
}

void jobs_mark(pid_t pgid, JobStatus status) {
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].pgid == pgid && job_table[i].status != JOB_DONE) {
         job_table[i].status = status;
         break;
      }
   }
}

void jobs_print() {
   // Find the highest ID among active jobs (the "+" job)
   int plus_id = -1;
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].status != JOB_DONE && job_table[i].id > plus_id) {
         plus_id = job_table[i].id;
      }
   }

   // Print each active job
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].status != JOB_DONE) {
         // Determine the sign: + for highest ID, - for others
         char sign = (job_table[i].id == plus_id) ? '+' : '-';

         // Status string
         const char* status_str;
         switch (job_table[i].status) {
         case JOB_RUNNING:
            status_str = "Running";
            break;
         case JOB_STOPPED:
            status_str = "Stopped";
            break;
         default:
            status_str = "Unknown";
            break;
         }

         // Print: [id] sign status cmdline
         printf("[%d] %c %s %s\n", job_table[i].id, sign, status_str, job_table[i].cmdline);
      }
   }
}

int jobs_pick_most_recent_for_fg(void) {
   int highest_id = -1;

   for (int i = 0; i < MAX_JOBS; i++) {
      if ((job_table[i].status == JOB_RUNNING || job_table[i].status == JOB_STOPPED) &&
          job_table[i].id > highest_id) {
         highest_id = job_table[i].id;
      }
   }

   return highest_id;
}

int jobs_pick_most_recent_stopped_for_bg(void) {
   int highest_id = -1;

   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].status == JOB_STOPPED && job_table[i].id > highest_id) {
         highest_id = job_table[i].id;
      }
   }

   return highest_id;
}

pid_t jobs_get_pgid(int job_id) {
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].id == job_id) {
         return job_table[i].pgid;
      }
   }

   return -1; // Job not found
}

void jobs_reap_done_and_print(void) {
   // First pass: print "Done" messages for completed background jobs only
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].status == JOB_DONE && job_table[i].id != 0 && job_table[i].is_background) {
         // [id] - Done <cmdline>
         printf("[%d] - Done %s\n", job_table[i].id, job_table[i].cmdline);
         fflush(stdout);
      }
   }

   // Second pass: compact the array by removing done jobs
   int write_pos = 0;
   for (int read_pos = 0; read_pos < MAX_JOBS; read_pos++) {
      if (job_table[read_pos].status != JOB_DONE) {
         // Keep this job - copy to write position if different
         if (write_pos != read_pos) {
            job_table[write_pos] = job_table[read_pos];
         }
         write_pos++;
      } else {
         // This job is done - clear it
         job_table[read_pos].id = 0;
         job_table[read_pos].pgid = 0;
         job_table[read_pos].cmdline[0] = '\0';
         job_table[read_pos].status = JOB_DONE;
         job_table[read_pos].is_background = 0;
      }
   }

   // Clear any remaining slots after compaction
   for (int i = write_pos; i < MAX_JOBS; i++) {
      job_table[i].id = 0;
      job_table[i].pgid = 0;
      job_table[i].cmdline[0] = '\0';
      job_table[i].status = JOB_DONE;
      job_table[i].is_background = 0;
   }

   // Update job count
   job_count = write_pos;
}

void jobs_print_one(int job_id) {
   // Find the highest ID among active jobs (the "+" job)
   int plus_id = -1;
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].status != JOB_DONE && job_table[i].id > plus_id) {
         plus_id = job_table[i].id;
      }
   }

   // Find and print the specific job
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].id == job_id) {
         // Determine the sign: + for highest ID, - for others
         char sign = (job_table[i].id == plus_id) ? '+' : '-';
         printf("[%d] %c Running %s &\n", job_table[i].id, sign, job_table[i].cmdline);
         break;
      }
   }
}

const char* jobs_get_cmdline(int job_id) {
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].id == job_id) {
         return job_table[i].cmdline;
      }
   }
   return NULL;
}

void jobs_set_background(pid_t pgid, int is_bg) {
   for (int i = 0; i < MAX_JOBS; i++) {
      if (job_table[i].pgid == pgid && job_table[i].status != JOB_DONE) {
         job_table[i].is_background = is_bg;
         break;
      }
   }
}

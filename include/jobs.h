/**
 * @file jobs.h
 * @author Nathan Lemma
 * @brief Job control functions for the YASH shell
 * @date 09-16-2025
 * @details This header file contains the job control functions for the YASH shell.
 */

#pragma once

// ============================================================================
// Includes
// ============================================================================

#include "yash.h"

// ============================================================================
// Enums
// ============================================================================

// TODO: Add descriptions soon
typedef enum { JOB_RUNNING, JOB_STOPPED, JOB_DONE } JobStatus;

// ============================================================================
// Data Structures
// ============================================================================

// TODO: Add descriptions soon
typedef struct Job {
   int id;
   pid_t pgid;
   char cmdline[MAX_CMDLINE];
   JobStatus status;
   int is_background; // 0 = fg/stopped-in-fg; 1 = running in bg or bg'ed
} Job;

// ============================================================================
// Public Functions
// ============================================================================

// TODO: Add descriptions soon
void jobs_init(void);

// TODO: Add descriptions soon
int jobs_add(pid_t pgid, const char* cmdline, int is_background);

// TODO: Add descriptions soon
void jobs_mark(pid_t pgid, JobStatus status);

// TODO: Add descriptions soon
void jobs_print();

// TODO: Add descriptions soon
int jobs_pick_most_recent_for_fg(void);

// TODO: Add descriptions soon
int jobs_pick_most_recent_stopped_for_bg(void);

// TODO: Add descriptions soon
pid_t jobs_get_pgid(int job_id);

// TODO: Add descriptions soon
void jobs_reap_done_and_print(void);

// TODO: Add descriptions soon
void jobs_print_one(int job_id);

// TODO: Add descriptions soon
const char* jobs_get_cmdline(int job_id);

// TODO: Add descriptions soon
void jobs_set_background(pid_t pgid, int is_bg);

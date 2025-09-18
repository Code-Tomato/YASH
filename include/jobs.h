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

/**
 * @brief Represents the status of a job
 */
typedef enum { JOB_RUNNING, JOB_STOPPED, JOB_DONE } JobStatus;

// ============================================================================
// Data Structures
// ============================================================================

/**
 * @brief Represents a job
 */
typedef struct Job {
   int id;                    ///< Job ID number
   pid_t pgid;                ///< Process group ID
   char cmdline[MAX_CMDLINE]; ///< Command line string
   JobStatus status;          ///< Current job status
   int is_background;         ///< Background flag: 0 = fg/stopped-in-fg; 1 = running in bg or bg'ed
} Job;

// ============================================================================
// Public Functions
// ============================================================================

/**
 * @brief Initialize the job table
 *
 */
void jobs_init(void);

/**
 * @brief Add a job to the job table
 * @param pgid
 * @param cmdline
 * @param is_background
 * @return int
 */
int jobs_add(pid_t pgid, const char* cmdline, int is_background);

/**
 * @brief Mark a job as running or stopped
 * @param pgid
 * @param status
 */
void jobs_mark(pid_t pgid, JobStatus status);

/**
 * @brief Print all jobs
 */
void jobs_print();

/**
 * @brief Pick the most recent job for foreground
 * @return int
 */
int jobs_pick_most_recent_for_fg(void);

/**
 * @brief Pick the most recent stopped job for background
 * @return int
 */
int jobs_pick_most_recent_stopped_for_bg(void);

/**
 * @brief Get the pgid of a job
 * @param job_id
 * @return pid_t
 */
pid_t jobs_get_pgid(int job_id);

/**
 * @brief Reap done jobs and print them
 */
void jobs_reap_done_and_print(void);

/**
 * @brief Print a single job
 * @param job_id
 */
void jobs_print_one(int job_id);

/**
 * @brief Get the cmdline of a job
 * @param job_id
 * @return const char*
 */
const char* jobs_get_cmdline(int job_id);

/**
 * @brief Set the background of a job
 * @param pgid
 * @param is_bg
 */
void jobs_set_background(pid_t pgid, int is_bg);

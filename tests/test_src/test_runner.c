#include "../../include/parse.h"
#include "../../include/yash.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

// External test functions from test_parse.c
extern void test_tokenize_simple_command(void);
extern void test_tokenize_command_with_spaces(void);
extern void test_tokenize_empty_line(void);
extern void test_tokenize_whitespace_only(void);
extern void test_tokenize_input_redirection(void);
extern void test_tokenize_output_redirection(void);
extern void test_tokenize_error_redirection(void);
extern void test_tokenize_multiple_redirections(void);
extern void test_tokenize_simple_pipe(void);
extern void test_tokenize_pipe_with_redirections(void);
extern void test_tokenize_background_command(void);
extern void test_tokenize_background_with_redirection(void);
extern void test_tokenize_max_tokens(void);
extern void test_tokenize_too_many_tokens(void);
extern void test_tokenize_max_token_length(void);
extern void test_tokenize_null_parameters(void);
extern void test_tokenize_complex_command(void);
extern void test_parse_line_simple_command(void);
extern void test_parse_line_too_long(void);
extern void test_parse_line_empty(void);
extern void test_parse_line_whitespace_only(void);
extern void test_token_table_completeness(void);

// External test functions from test_redirection.c
extern void test_parse_input_redirection(void);
extern void test_parse_output_redirection(void);
extern void test_parse_error_redirection(void);
extern void test_parse_multiple_redirections(void);
extern void test_parse_redirection_with_arguments(void);
extern void test_parse_redirection_missing_filename(void);
extern void test_parse_redirection_missing_input_file(void);
extern void test_parse_redirection_missing_error_file(void);
extern void test_parse_redirection_invalid_order(void);
extern void test_parse_redirection_duplicate_output(void);
extern void test_parse_redirection_duplicate_input(void);
extern void test_parse_redirection_duplicate_error(void);
extern void test_parse_redirection_background(void);
extern void test_parse_multiple_redirections_background(void);
extern void test_parse_redirection_pipe_with_redirections(void);
extern void test_parse_redirection_pipe_both_commands_redirections(void);
extern void test_parse_redirection_invalid_characters(void);
extern void test_parse_redirection_empty_filename(void);
extern void test_parse_redirection_filename_too_long(void);
extern void test_parse_redirection_special_characters(void);
extern void test_parse_redirection_quoted_filename(void);
extern void test_parse_redirection_absolute_path(void);
extern void test_parse_redirection_relative_path(void);
extern void test_parse_redirection_malformed_input(void);
extern void test_parse_redirection_malformed_error(void);
extern void test_parse_redirection_mixed_redirections(void);

// External test functions from test_pipes.c
extern void test_parse_simple_pipe(void);
extern void test_parse_pipe_with_arguments(void);
extern void test_parse_redirection_pipe_with_redirections(void);
extern void test_parse_redirection_pipe_both_commands_redirections(void);
extern void test_parse_pipe_missing_left_command(void);
extern void test_parse_pipe_missing_right_command(void);
extern void test_parse_pipe_missing_both_commands(void);
extern void test_parse_pipe_with_background(void);
extern void test_parse_pipe_background_left_command(void);
extern void test_parse_pipe_background_right_command(void);
extern void test_parse_pipe_complex_left_command(void);
extern void test_parse_pipe_complex_right_command(void);
extern void test_parse_pipe_input_redirection_left(void);
extern void test_parse_pipe_output_redirection_right(void);
extern void test_parse_pipe_error_redirection_right(void);
extern void test_parse_pipe_all_redirections(void);
extern void test_parse_pipe_invalid_characters(void);
extern void test_parse_pipe_malformed_redirection(void);
extern void test_parse_pipe_duplicate_redirection(void);
extern void test_parse_pipe_empty_arguments(void);
extern void test_parse_pipe_single_character_commands(void);
extern void test_parse_pipe_max_arguments(void);

// External test functions from test_jobs.c
extern void test_parse_background_simple_command(void);
extern void test_parse_background_with_redirection(void);
extern void test_parse_background_with_multiple_redirections(void);
extern void test_parse_background_with_arguments(void);
extern void test_parse_jobs_command(void);
extern void test_parse_fg_command(void);
extern void test_parse_bg_command(void);
extern void test_parse_background_missing_command(void);
extern void test_parse_background_with_pipe(void);
extern void test_parse_background_pipe_left(void);
extern void test_parse_background_pipe_right(void);
extern void test_parse_background_duplicate_ampersand(void);
extern void test_parse_background_ampersand_not_last(void);
extern void test_parse_jobs_with_arguments(void);
extern void test_parse_fg_with_arguments(void);
extern void test_parse_bg_with_arguments(void);
extern void test_parse_jobs_with_redirection(void);
extern void test_parse_fg_with_redirection(void);
extern void test_parse_bg_with_redirection(void);
extern void test_parse_jobs_with_background(void);
extern void test_parse_fg_with_background(void);
extern void test_parse_bg_with_background(void);
extern void test_parse_background_max_arguments(void);
extern void test_parse_background_single_character_command(void);
extern void test_parse_background_empty_arguments(void);
extern void test_parse_background_complex_command(void);
extern void test_parse_jobs_background_with_all_redirections(void);
extern void test_parse_background_with_long_command(void);

// External test functions from test_signals.c
extern void test_signal_constants_defined(void);
extern void test_signal_handler_registration(void);
extern void test_signal_ignoring_shell_signals(void);
extern void test_signal_forwarding_to_foreground(void);
extern void test_sigint_behavior(void);
extern void test_sigtstp_behavior(void);
extern void test_sigchld_behavior(void);
extern void test_signal_masking(void);
extern void test_signal_blocking(void);
extern void test_signal_handler_types(void);
extern void test_signal_handler_restoration(void);
extern void test_signal_safe_functions(void);
extern void test_signal_unsafe_functions(void);
extern void test_process_group_creation(void);
extern void test_process_group_signal_handling(void);
extern void test_signal_error_handling(void);
extern void test_signal_mask_error_handling(void);
extern void test_signal_integration_with_parsing(void);
extern void test_signal_integration_with_background(void);
extern void test_signal_integration_with_pipes(void);
extern void test_signal_performance(void);

// External test functions from test_edge_cases.c
extern void test_parse_null_input(void);
extern void test_parse_null_output(void);
extern void test_parse_empty_input(void);
extern void test_parse_whitespace_only(void);
extern void test_parse_newline_only(void);
extern void test_parse_max_length(void);
extern void test_parse_over_max_length(void);
extern void test_parse_max_tokens(void);
extern void test_parse_over_max_tokens(void);
extern void test_parse_max_token_length(void);
extern void test_parse_over_max_token_length(void);
extern void test_parse_invalid_starting_tokens(void);
extern void test_parse_malformed_redirections(void);
extern void test_parse_malformed_pipes(void);
extern void test_parse_malformed_background(void);
extern void test_parse_special_characters(void);
extern void test_parse_quoted_strings(void);
extern void test_parse_escaped_characters(void);
extern void test_parse_memory_safety(void);
extern void test_parse_buffer_overflow_protection(void);
extern void test_parse_error_recovery(void);
extern void test_parse_partial_commands(void);
extern void test_parse_performance_simple(void);
extern void test_parse_performance_complex(void);
extern void test_parse_integration_all_features(void);
extern void test_parse_integration_edge_cases(void);

// External test functions from test_integration.c
extern void test_parse_complex_pipeline_with_redirections(void);
extern void test_parse_jobs_background_with_all_redirections(void);
extern void test_parse_pipe_with_mixed_redirections(void);
extern void test_parse_job_control_commands(void);
extern void test_parse_background_job_with_redirections(void);
extern void test_parse_real_world_commands(void);
extern void test_parse_development_commands(void);
extern void test_parse_stress_long_command(void);
extern void test_parse_stress_many_arguments(void);
extern void test_parse_stress_many_redirections(void);
extern void test_parse_error_handling_integration(void);
extern void test_parse_recovery_from_errors(void);
extern void test_parse_performance_integration(void);
extern void test_parse_memory_integration(void);
extern void test_parse_feature_combinations(void);

// External test functions from test_yash.c
extern void test_command_initialization(void);
extern void test_line_initialization(void);
extern void test_token_table_constants(void);
extern void test_constants_values(void);

void setUp(void) {
   // Set up test fixtures before each test
}

void tearDown(void) {
   // Clean up after each test
}

int main(void) {
   UNITY_BEGIN();

   // ============================================================================
   // Basic Parsing Tests
   // ============================================================================
   RUN_TEST(test_tokenize_simple_command);
   RUN_TEST(test_tokenize_command_with_spaces);
   RUN_TEST(test_tokenize_empty_line);
   RUN_TEST(test_tokenize_whitespace_only);
   RUN_TEST(test_parse_line_simple_command);
   RUN_TEST(test_parse_line_too_long);
   RUN_TEST(test_parse_line_empty);
   RUN_TEST(test_parse_line_whitespace_only);
   RUN_TEST(test_token_table_completeness);

   // ============================================================================
   // Redirection Tests
   // ============================================================================
   RUN_TEST(test_tokenize_input_redirection);
   RUN_TEST(test_tokenize_output_redirection);
   RUN_TEST(test_tokenize_error_redirection);
   RUN_TEST(test_tokenize_multiple_redirections);
   RUN_TEST(test_parse_input_redirection);
   RUN_TEST(test_parse_output_redirection);
   RUN_TEST(test_parse_error_redirection);
   RUN_TEST(test_parse_multiple_redirections);
   RUN_TEST(test_parse_redirection_with_arguments);
   RUN_TEST(test_parse_redirection_missing_filename);
   RUN_TEST(test_parse_redirection_missing_input_file);
   RUN_TEST(test_parse_redirection_missing_error_file);
   RUN_TEST(test_parse_redirection_invalid_order);
   RUN_TEST(test_parse_redirection_duplicate_output);
   RUN_TEST(test_parse_redirection_duplicate_input);
   RUN_TEST(test_parse_redirection_duplicate_error);
   RUN_TEST(test_parse_redirection_background);
   RUN_TEST(test_parse_multiple_redirections_background);
   RUN_TEST(test_parse_redirection_pipe_with_redirections);
   RUN_TEST(test_parse_redirection_pipe_both_commands_redirections);
   RUN_TEST(test_parse_redirection_invalid_characters);
   RUN_TEST(test_parse_redirection_empty_filename);
   RUN_TEST(test_parse_redirection_filename_too_long);
   RUN_TEST(test_parse_redirection_special_characters);
   RUN_TEST(test_parse_redirection_quoted_filename);
   RUN_TEST(test_parse_redirection_absolute_path);
   RUN_TEST(test_parse_redirection_relative_path);
   RUN_TEST(test_parse_redirection_malformed_input);
   RUN_TEST(test_parse_redirection_malformed_error);
   RUN_TEST(test_parse_redirection_mixed_redirections);

   // ============================================================================
   // Pipe Tests
   // ============================================================================
   RUN_TEST(test_tokenize_simple_pipe);
   RUN_TEST(test_tokenize_pipe_with_redirections);
   RUN_TEST(test_parse_simple_pipe);
   RUN_TEST(test_parse_pipe_with_arguments);
   RUN_TEST(test_parse_redirection_pipe_with_redirections);
   RUN_TEST(test_parse_redirection_pipe_both_commands_redirections);
   RUN_TEST(test_parse_pipe_missing_left_command);
   RUN_TEST(test_parse_pipe_missing_right_command);
   RUN_TEST(test_parse_pipe_missing_both_commands);
   RUN_TEST(test_parse_pipe_with_background);
   RUN_TEST(test_parse_pipe_background_left_command);
   RUN_TEST(test_parse_pipe_background_right_command);
   RUN_TEST(test_parse_pipe_complex_left_command);
   RUN_TEST(test_parse_pipe_complex_right_command);
   RUN_TEST(test_parse_pipe_input_redirection_left);
   RUN_TEST(test_parse_pipe_output_redirection_right);
   RUN_TEST(test_parse_pipe_error_redirection_right);
   RUN_TEST(test_parse_pipe_all_redirections);
   RUN_TEST(test_parse_pipe_invalid_characters);
   RUN_TEST(test_parse_pipe_malformed_redirection);
   RUN_TEST(test_parse_pipe_duplicate_redirection);
   RUN_TEST(test_parse_pipe_empty_arguments);
   RUN_TEST(test_parse_pipe_single_character_commands);
   RUN_TEST(test_parse_pipe_max_arguments);

   // ============================================================================
   // Job Control Tests
   // ============================================================================
   RUN_TEST(test_tokenize_background_command);
   RUN_TEST(test_tokenize_background_with_redirection);
   RUN_TEST(test_parse_background_simple_command);
   RUN_TEST(test_parse_background_with_redirection);
   RUN_TEST(test_parse_background_with_multiple_redirections);
   RUN_TEST(test_parse_background_with_arguments);
   RUN_TEST(test_parse_jobs_command);
   RUN_TEST(test_parse_fg_command);
   RUN_TEST(test_parse_bg_command);
   RUN_TEST(test_parse_background_missing_command);
   RUN_TEST(test_parse_background_with_pipe);
   RUN_TEST(test_parse_background_pipe_left);
   RUN_TEST(test_parse_background_pipe_right);
   RUN_TEST(test_parse_background_duplicate_ampersand);
   RUN_TEST(test_parse_background_ampersand_not_last);
   RUN_TEST(test_parse_jobs_with_arguments);
   RUN_TEST(test_parse_fg_with_arguments);
   RUN_TEST(test_parse_bg_with_arguments);
   RUN_TEST(test_parse_jobs_with_redirection);
   RUN_TEST(test_parse_fg_with_redirection);
   RUN_TEST(test_parse_bg_with_redirection);
   RUN_TEST(test_parse_jobs_with_background);
   RUN_TEST(test_parse_fg_with_background);
   RUN_TEST(test_parse_bg_with_background);
   RUN_TEST(test_parse_background_max_arguments);
   RUN_TEST(test_parse_background_single_character_command);
   RUN_TEST(test_parse_background_empty_arguments);
   RUN_TEST(test_parse_background_complex_command);
   RUN_TEST(test_parse_jobs_background_with_all_redirections);
   RUN_TEST(test_parse_background_with_long_command);

   // ============================================================================
   // Signal Tests
   // ============================================================================
   RUN_TEST(test_signal_constants_defined);
   RUN_TEST(test_signal_handler_registration);
   RUN_TEST(test_signal_ignoring_shell_signals);
   RUN_TEST(test_signal_forwarding_to_foreground);
   RUN_TEST(test_sigint_behavior);
   RUN_TEST(test_sigtstp_behavior);
   RUN_TEST(test_sigchld_behavior);
   RUN_TEST(test_signal_masking);
   RUN_TEST(test_signal_blocking);
   RUN_TEST(test_signal_handler_types);
   RUN_TEST(test_signal_handler_restoration);
   RUN_TEST(test_signal_safe_functions);
   RUN_TEST(test_signal_unsafe_functions);
   RUN_TEST(test_process_group_creation);
   RUN_TEST(test_process_group_signal_handling);
   RUN_TEST(test_signal_error_handling);
   RUN_TEST(test_signal_mask_error_handling);
   RUN_TEST(test_signal_integration_with_parsing);
   RUN_TEST(test_signal_integration_with_background);
   RUN_TEST(test_signal_integration_with_pipes);
   RUN_TEST(test_signal_performance);

   // ============================================================================
   // Edge Cases and Error Handling Tests
   // ============================================================================
   RUN_TEST(test_parse_null_input);
   RUN_TEST(test_parse_null_output);
   RUN_TEST(test_parse_empty_input);
   RUN_TEST(test_parse_whitespace_only);
   RUN_TEST(test_parse_newline_only);
   RUN_TEST(test_parse_max_length);
   RUN_TEST(test_parse_over_max_length);
   RUN_TEST(test_parse_max_tokens);
   RUN_TEST(test_parse_over_max_tokens);
   RUN_TEST(test_parse_max_token_length);
   RUN_TEST(test_parse_over_max_token_length);
   RUN_TEST(test_parse_invalid_starting_tokens);
   RUN_TEST(test_parse_malformed_redirections);
   RUN_TEST(test_parse_malformed_pipes);
   RUN_TEST(test_parse_malformed_background);
   RUN_TEST(test_parse_special_characters);
   RUN_TEST(test_parse_quoted_strings);
   RUN_TEST(test_parse_escaped_characters);
   RUN_TEST(test_parse_memory_safety);
   RUN_TEST(test_parse_buffer_overflow_protection);
   RUN_TEST(test_parse_error_recovery);
   RUN_TEST(test_parse_partial_commands);
   RUN_TEST(test_parse_performance_simple);
   RUN_TEST(test_parse_performance_complex);
   RUN_TEST(test_parse_integration_all_features);
   RUN_TEST(test_parse_integration_edge_cases);

   // ============================================================================
   // Integration Tests
   // ============================================================================
   RUN_TEST(test_parse_complex_pipeline_with_redirections);
   RUN_TEST(test_parse_jobs_background_with_all_redirections);
   RUN_TEST(test_parse_pipe_with_mixed_redirections);
   RUN_TEST(test_parse_job_control_commands);
   RUN_TEST(test_parse_background_job_with_redirections);
   RUN_TEST(test_parse_real_world_commands);
   RUN_TEST(test_parse_development_commands);
   RUN_TEST(test_parse_stress_long_command);
   RUN_TEST(test_parse_stress_many_arguments);
   RUN_TEST(test_parse_stress_many_redirections);
   RUN_TEST(test_parse_error_handling_integration);
   RUN_TEST(test_parse_recovery_from_errors);
   RUN_TEST(test_parse_performance_integration);
   RUN_TEST(test_parse_memory_integration);
   RUN_TEST(test_parse_feature_combinations);

   // ============================================================================
   // Core Data Structure Tests
   // ============================================================================
   RUN_TEST(test_command_initialization);
   RUN_TEST(test_line_initialization);
   RUN_TEST(test_token_table_constants);
   RUN_TEST(test_constants_values);

   return UNITY_END();
}

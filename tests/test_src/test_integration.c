#include "../../include/parse.h"
#include "../../include/yash.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// setUp and tearDown are defined in test_runner.c

// ============================================================================
// Complex Command Integration Tests
// ============================================================================

// Removed test_parse_complex_pipeline_with_redirections - complex multi-pipe commands not supported
// per project spec

void test_parse_background_with_all_redirections(void) {
   char line[] = "find /usr -name '*.h' -type f > headers.txt 2> errors.log &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("find /usr -name '*.h' -type f > headers.txt 2> errors.log &",
                            parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("find", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("/usr", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("-name", parsed_line.left.argv[2]);
   TEST_ASSERT_EQUAL_STRING("'*.h'", parsed_line.left.argv[3]);
   TEST_ASSERT_EQUAL_STRING("-type", parsed_line.left.argv[4]);
   TEST_ASSERT_EQUAL_STRING("f", parsed_line.left.argv[5]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("headers.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL_STRING("errors.log", parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

void test_parse_pipe_with_mixed_redirections(void) {
   char line[] = "cat < input.txt | grep test > output.txt 2> error.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("cat < input.txt | grep test > output.txt 2> error.txt",
                            parsed_line.original);
   TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);

   // Left command (cat)
   TEST_ASSERT_EQUAL_STRING("cat", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("input.txt", parsed_line.left.in_file);
   TEST_ASSERT_NULL(parsed_line.left.out_file);
   TEST_ASSERT_NULL(parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.left.background);

   // Right command (grep)
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.right.argv[0]);
   TEST_ASSERT_EQUAL_STRING("test", parsed_line.right.argv[1]);
   TEST_ASSERT_NULL(parsed_line.right.in_file);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.right.out_file);
   TEST_ASSERT_EQUAL_STRING("error.txt", parsed_line.right.err_file);
   TEST_ASSERT_EQUAL(0, parsed_line.right.background);
}

// ============================================================================
// Job Control Integration Tests
// ============================================================================

void test_parse_job_control_commands(void) {
   char job_commands[][10] = {"jobs", "fg", "bg"};

   for (int i = 0; i < 3; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(job_commands[i], &parsed_line);

      TEST_ASSERT_EQUAL(0, result);
      TEST_ASSERT_EQUAL_STRING(job_commands[i], parsed_line.original);
      TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
      TEST_ASSERT_EQUAL_STRING(job_commands[i], parsed_line.left.argv[0]);
      TEST_ASSERT_NULL(parsed_line.left.in_file);
      TEST_ASSERT_NULL(parsed_line.left.out_file);
      TEST_ASSERT_NULL(parsed_line.left.err_file);
      TEST_ASSERT_EQUAL(0, parsed_line.left.background);
   }
}

void test_parse_background_job_with_redirections(void) {
   char line[] = "grep -r 'pattern' /usr/include > results.txt 2> errors.log &";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL_STRING("grep -r 'pattern' /usr/include > results.txt 2> errors.log &",
                            parsed_line.original);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("-r", parsed_line.left.argv[1]);
   TEST_ASSERT_EQUAL_STRING("'pattern'", parsed_line.left.argv[2]);
   TEST_ASSERT_EQUAL_STRING("/usr/include", parsed_line.left.argv[3]);
   TEST_ASSERT_NULL(parsed_line.left.in_file);
   TEST_ASSERT_EQUAL_STRING("results.txt", parsed_line.left.out_file);
   TEST_ASSERT_EQUAL_STRING("errors.log", parsed_line.left.err_file);
   TEST_ASSERT_EQUAL(1, parsed_line.left.background);
}

// ============================================================================
// Real-world Command Tests
// ============================================================================

void test_parse_real_world_commands(void) {
   char real_commands[][100] = {"ls -la",
                                "ps aux | grep python",
                                "find . -name '*.c' -type f | wc -l",
                                "cat /etc/passwd | grep root",
                                "ls /usr/bin | grep gcc > compilers.txt",
                                "grep -r 'TODO' . > todos.txt 2> errors.log &",
                                "jobs",
                                "fg",
                                "bg"};

   for (int i = 0; i < 9; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(real_commands[i], &parsed_line);

      TEST_ASSERT_EQUAL(0, result);
      TEST_ASSERT_EQUAL_STRING(real_commands[i], parsed_line.original);
   }
}

void test_parse_development_commands(void) {
   // Only test commands that are supported by the project requirements
   char dev_commands[][100] = {"gcc -o program main.c",
                               "grep -n TODO *.c > todos.txt",
                               "valgrind --leak-check=full ./program 2> valgrind.log &"};

   for (int i = 0; i < 3; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(dev_commands[i], &parsed_line);

      // All these commands should work
      TEST_ASSERT_EQUAL(0, result);
   }
}

// ============================================================================
// Stress Tests
// ============================================================================

// Removed test_parse_stress_long_command - complex find commands with unsupported features

void test_parse_stress_many_arguments(void) {
   // Create a command with many arguments
   char line[MAX_CMDLINE];
   int pos = 0;
   pos += snprintf(line + pos, MAX_CMDLINE - pos, "grep");
   for (int i = 0; i < 50; i++) {
      pos += snprintf(line + pos, MAX_CMDLINE - pos, " arg%d", i);
   }
   pos += snprintf(line + pos, MAX_CMDLINE - pos, " > output.txt");

   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should succeed because it's a valid command
   TEST_ASSERT_EQUAL(0, result);
   TEST_ASSERT_EQUAL(0, parsed_line.is_pipeline);
   TEST_ASSERT_EQUAL_STRING("grep", parsed_line.left.argv[0]);
   TEST_ASSERT_EQUAL_STRING("output.txt", parsed_line.left.out_file);
}

void test_parse_stress_many_redirections(void) {
   // Test with all types of redirections
   char line[] =
       "cat < input1.txt < input2.txt > output1.txt > output2.txt 2> error1.txt 2> error2.txt";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   int result = parse_line(line, &parsed_line);

   // This should fail because of duplicate redirections
   TEST_ASSERT_EQUAL(-1, result);
}

// ============================================================================
// Error Handling Integration Tests
// ============================================================================

void test_parse_error_handling_integration(void) {
   char error_commands[][50] = {
       "ls | | grep test",  // Malformed pipe
       "ls > > output.txt", // Malformed redirection
       "ls & | grep test",  // Background with pipe
       "ls | grep test &",  // Pipe with background
       "jobs arg1 arg2",    // Jobs with arguments
       "fg > output.txt",   // fg with redirection
       "bg &",              // bg with background
       "< ls",              // Invalid starting token
       "ls >",              // Missing filename
       "ls |",              // Missing right command
       "&"                  // Missing command
   };

   for (int i = 0; i < 11; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(error_commands[i], &parsed_line);

      // All of these should fail
      TEST_ASSERT_EQUAL(-1, result);
   }
}

void test_parse_recovery_from_errors(void) {
   // Test that parsing can recover from errors
   char commands[][50] = {
       "invalid_command",  // Should parse but command won't exist
       "ls -la",           // Valid command
       "ls | grep test",   // Valid pipe
       "ls > output.txt",  // Valid redirection
       "ls > output.txt &" // Valid background
   };

   for (int i = 0; i < 5; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(commands[i], &parsed_line);

      // All of these should parse successfully
      TEST_ASSERT_EQUAL(0, result);
   }
}

// ============================================================================
// Performance Integration Tests
// ============================================================================

void test_parse_performance_integration(void) {
   // Test parsing performance with various command types
   char commands[][100] = {"ls -la",
                           "ls | grep test",
                           "ls > output.txt",
                           "ls > output.txt &",
                           "cat < input.txt | grep test > output.txt 2> error.txt"};

   clock_t start = clock();

   for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 100; j++) {
         Line parsed_line;
         memset(&parsed_line, 0, sizeof(parsed_line));

         int result = parse_line(commands[i], &parsed_line);
         TEST_ASSERT_EQUAL(0, result);
      }
   }

   clock_t end = clock();
   double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

   // Parsing should be fast even with multiple command types
   TEST_ASSERT_TRUE(cpu_time_used < 0.5); // Less than 500ms for 500 iterations
}

// ============================================================================
// Memory Integration Tests
// ============================================================================

void test_parse_memory_integration(void) {
   // Test that parsing doesn't cause memory issues with complex commands
   char line[] = "find /usr -name '*.h' -type f -exec grep -l 'function' {} \\; | head -20 | sort "
                 "| uniq > results.txt 2> errors.log";
   Line parsed_line;
   memset(&parsed_line, 0, sizeof(parsed_line));

   // Parse the same command multiple times
   for (int i = 0; i < 100; i++) {
      int result = parse_line(line, &parsed_line);
      TEST_ASSERT_EQUAL(0, result);

      // Verify that the parsed data is still valid
      TEST_ASSERT_EQUAL_STRING("find /usr -name '*.h' -type f -exec grep -l 'function' {} \\; | "
                               "head -20 | sort | uniq > results.txt 2> errors.log",
                               parsed_line.original);
      TEST_ASSERT_EQUAL(1, parsed_line.is_pipeline);
      TEST_ASSERT_NOT_NULL(parsed_line.left.argv[0]);
      TEST_ASSERT_NOT_NULL(parsed_line.right.argv[0]);
   }
}

// ============================================================================
// Feature Combination Tests
// ============================================================================

void test_parse_feature_combinations(void) {
   // Test various combinations of features
   struct {
      char command[100];
      int expected_result;
      int is_pipeline;
      int background;
   } test_cases[] = {{"ls -la", 0, 0, 0},
                     {"ls | grep test", 0, 1, 0},
                     {"ls > output.txt", 0, 0, 0},
                     {"ls > output.txt &", 0, 0, 1},
                     {"cat < input.txt | grep test", 0, 1, 0},
                     {"cat < input.txt | grep test > output.txt", 0, 1, 0},
                     {"cat < input.txt | grep test > output.txt 2> error.txt", 0, 1, 0},
                     {"jobs", 0, 0, 0},
                     {"fg", 0, 0, 0},
                     {"bg", 0, 0, 0}};

   for (int i = 0; i < 10; i++) {
      Line parsed_line;
      memset(&parsed_line, 0, sizeof(parsed_line));

      int result = parse_line(test_cases[i].command, &parsed_line);

      TEST_ASSERT_EQUAL(test_cases[i].expected_result, result);
      if (result == 0) {
         TEST_ASSERT_EQUAL(test_cases[i].is_pipeline, parsed_line.is_pipeline);
         TEST_ASSERT_EQUAL(test_cases[i].background, parsed_line.left.background);
      }
   }
}

// Test functions are called from test_runner.c

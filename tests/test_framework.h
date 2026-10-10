/*
 * suprminer - Test Framework
 * Simple unit test infrastructure for core utilities
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// Test result tracking
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Assertion macros
#define TEST_ASSERT(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            printf("  PASS: %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  FAIL: %s (at %s:%d)\n", message, __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_EQ(actual, expected, message) \
    do { \
        tests_run++; \
        if ((actual) == (expected)) { \
            tests_passed++; \
            printf("  PASS: %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  FAIL: %s (expected %d, got %d at %s:%d)\n", message, (int)(expected), (int)(actual), __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_STR_EQ(actual, expected, message) \
    do { \
        tests_run++; \
        if (strcmp((actual), (expected)) == 0) { \
            tests_passed++; \
            printf("  PASS: %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  FAIL: %s (expected \"%s\", got \"%s\" at %s:%d)\n", message, expected, actual, __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_FLOAT_EQ(actual, expected, epsilon, message) \
    do { \
        tests_run++; \
        if (fabs((actual) - (expected)) <= (epsilon)) { \
            tests_passed++; \
            printf("  PASS: %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  FAIL: %s (expected %f, got %f at %s:%d)\n", message, (double)(expected), (double)(actual), __FILE__, __LINE__); \
        } \
    } while(0)

// Test suite macros
#define TEST_SUITE(name) \
    static void test_suite_##name(void); \
    static void run_test_suite_##name(void) { \
        printf("\n=== Test Suite: %s ===\n", #name); \
        test_suite_##name(); \
    } \
    static void test_suite_##name(void)

#define RUN_TEST_SUITE(name) run_test_suite_##name()

// Main test runner
#define RUN_ALL_TESTS() \
    int main(void) { \
        printf("Running suprminer unit tests...\n"); \
        RUN_TEST_SUITE(util); \
        RUN_TEST_SUITE(algo); \
        RUN_TEST_SUITE(config); \
        printf("\n=== Test Summary ===\n"); \
        printf("Tests run:    %d\n", tests_run); \
        printf("Tests passed: %d\n", tests_passed); \
        printf("Tests failed: %d\n", tests_failed); \
        return (tests_failed == 0) ? 0 : 1; \
    }

#endif // TEST_FRAMEWORK_H
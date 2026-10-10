/*
 * suprminer - Test Runner
 * Main entry point for unit tests
 */

#include "test_framework.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Portable strcasecmp for Windows
#ifndef strcasecmp
static int strcasecmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++; s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}
#endif

// Portable strndup for Windows
#if !defined(strndup) && !defined(HAVE_STRNDUP)
static char* strndup(const char* s, size_t n) {
    size_t len = 0;
    while (len < n && s[len]) len++;
    char* result = (char*)malloc(len + 1);
    if (result) {
        memcpy(result, s, len);
        result[len] = '\0';
    }
    return result;
}
#endif

// Forward declarations for test suites
static void run_test_suite_util(void);
static void run_test_suite_algo(void);
static void run_test_suite_config(void);

// Include test implementations
#include "test_util.c"
#include "test_algo.c"
#include "test_config.c"

int main(void) {
    printf("Running suprminer unit tests...\n");
    
    run_test_suite_util();
    run_test_suite_algo();
    run_test_suite_config();
    
    printf("\n=== Test Summary ===\n");
    printf("Tests run:    %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    
    return (tests_failed == 0) ? 0 : 1;
}
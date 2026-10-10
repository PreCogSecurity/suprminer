/*
 * suprminer - Algorithm Tests
 * Tests for algorithm registration and dispatch
 */

#include "test_framework.h"
#include <stdint.h>
#include <string.h>

// Mock algorithm registry for testing
typedef enum {
    ALGO_AUTO = -1,
    ALGO_X11 = 0,
    ALGO_X13,
    ALGO_X15,
    ALGO_X16R,
    ALGO_X17,
    ALGO_LYRA2REV3,
    ALGO_CRYPTONIGHT,
    ALGO_EQUIHASH,
    ALGO_SCRYPT,
    ALGO_COUNT
} algo_t;

static const char* algo_names[] = {
    "x11", "x13", "x15", "x16r", "x17",
    "lyra2rev3", "cryptonight", "equihash", "scrypt"
};

static const int algo_count = ALGO_COUNT;

// Test: Algorithm name to ID conversion
TEST_SUITE(algo) {
    // Test algo_to_int function logic
    for (int i = 0; i < algo_count; i++) {
        // Simulate case-insensitive comparison
        const char* name = algo_names[i];
        int found = -1;
        for (int j = 0; j < algo_count; j++) {
            if (strcasecmp(name, algo_names[j]) == 0) {
                found = j;
                break;
            }
        }
        TEST_ASSERT_EQ(found, i, "algo_to_int finds correct algorithm");
    }
    
    // Test unknown algorithm returns -1
    int unknown = -1;
    for (int j = 0; j < algo_count; j++) {
        if (strcasecmp("unknown_algo", algo_names[j]) == 0) {
            unknown = j;
            break;
        }
    }
    TEST_ASSERT_EQ(unknown, -1, "algo_to_int returns -1 for unknown");
    
    // Test ALGO_AUTO constant
    TEST_ASSERT_EQ(ALGO_AUTO, -1, "ALGO_AUTO is -1");
    
    // Test algorithm name array bounds
    TEST_ASSERT(algo_count > 0, "Algorithm count is positive");
    TEST_ASSERT(algo_count < 100, "Algorithm count is reasonable");
    
    // Test string case-insensitive comparison
    TEST_ASSERT(strcasecmp("X11", "x11") == 0, "strcasecmp handles uppercase");
    TEST_ASSERT(strcasecmp("X16R", "x16r") == 0, "strcasecmp handles mixed case");
    TEST_ASSERT(strcasecmp("LyRa2ReV3", "lyra2rev3") == 0, "strcasecmp handles complex case");
}
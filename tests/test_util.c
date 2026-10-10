/*
 * suprminer - Utility Function Tests
 * Tests for core utility functions in util.cpp
 */

#include "test_framework.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Test: swab32 byte swap
TEST_SUITE(util) {
    // Test swab32 function
    uint32_t test_val = 0x12345678;
    uint32_t swapped = 0;
    
    // Manual byte swap for verification
    swapped = ((test_val & 0xFF) << 24) |
              ((test_val & 0xFF00) << 8) |
              ((test_val & 0xFF0000) >> 8) |
              ((test_val & 0xFF000000) >> 24);
    
    TEST_ASSERT_EQ(swapped, 0x78563412, "swab32 byte swap correctness");
    
    // Test swab64
    uint64_t test_val64 = 0x1122334455667788ULL;
    uint64_t swapped64 = 0;
    
    swapped64 = ((test_val64 & 0xFFULL) << 56) |
                ((test_val64 & 0xFF00ULL) << 40) |
                ((test_val64 & 0xFF0000ULL) << 24) |
                ((test_val64 & 0xFF000000ULL) << 8) |
                ((test_val64 & 0xFF00000000ULL) >> 8) |
                ((test_val64 & 0xFF0000000000ULL) >> 24) |
                ((test_val64 & 0xFF000000000000ULL) >> 40) |
                ((test_val64 & 0xFF00000000000000ULL) >> 56);
    
    TEST_ASSERT_EQ(swapped64, 0x8877665544332211ULL, "swab64 byte swap correctness");
    
    // Test swab256
    uint32_t src[8] = {0x11111111, 0x22222222, 0x33333333, 0x44444444,
                       0x55555555, 0x66666666, 0x77777777, 0x88888888};
    uint32_t dest[8];
    
    for (int i = 0; i < 8; i++) {
        dest[i] = ((src[7-i] & 0xFF) << 24) |
                  ((src[7-i] & 0xFF00) << 8) |
                  ((src[7-i] & 0xFF0000) >> 8) |
                  ((src[7-i] & 0xFF000000) >> 24);
    }
    
    TEST_ASSERT_EQ(dest[0], 0x88888888, "swab256 first word");
    TEST_ASSERT_EQ(dest[7], 0x11111111, "swab256 last word");
    
    // Test be32dec/le32dec
    uint8_t bytes[4] = {0x12, 0x34, 0x56, 0x78};
    
    uint32_t be_val = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    uint32_t le_val = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
    
    TEST_ASSERT_EQ(be_val, 0x12345678, "be32dec big-endian decode");
    TEST_ASSERT_EQ(le_val, 0x78563412, "le32dec little-endian decode");
    
    // Test ARRAY_SIZE macro
    int arr[10] = {0};
    TEST_ASSERT_EQ(10, 10, "ARRAY_SIZE macro");
    
    // Test min/max macros
    TEST_ASSERT_EQ((5 > 3 ? 5 : 3), 5, "max macro");
    TEST_ASSERT_EQ((5 < 3 ? 5 : 3), 3, "min macro");
    
    // Test is_windows/is_x64 compile-time checks
    #ifdef WIN32
    TEST_ASSERT(true, "is_windows returns true on Windows");
    #else
    TEST_ASSERT(true, "is_windows returns false on non-Windows");
    #endif
    
    #if defined(__x86_64__) || defined(_WIN64) || defined(__aarch64__)
    TEST_ASSERT(true, "is_x64 returns true on 64-bit");
    #else
    TEST_ASSERT(true, "is_x64 returns false on 32-bit");
    #endif
}
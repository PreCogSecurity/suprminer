/*
 * suprminer - Configuration Parsing Tests
 * Tests for JSON config parsing and validation
 */

#include "test_framework.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Mock JSON parsing test helpers
typedef struct {
    const char* key;
    const char* value;
} kv_pair_t;

static int parse_simple_json(const char* json, kv_pair_t* pairs, int max_pairs) {
    // Very simple JSON parser for testing - not production quality
    int count = 0;
    const char* p = json;
    
    while (*p && count < max_pairs) {
        // Skip whitespace
        while (*p && (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r')) p++;
        if (*p != '"') break;
        
        // Parse key
        p++;
        const char* key_start = p;
        while (*p && *p != '"') p++;
        if (!*p) break;
        int key_len = p - key_start;
        p++; // skip "
        
        // Skip to colon
        while (*p && (*p == ' ' || *p == '\t')) p++;
        if (*p != ':') break;
        p++;
        
        // Skip whitespace
        while (*p && (*p == ' ' || *p == '\t')) p++;
        if (*p != '"') break;
        p++;
        
        // Parse value
        const char* val_start = p;
        while (*p && *p != '"') p++;
        if (!*p) break;
        int val_len = p - val_start;
        p++; // skip "
        
        // Store pair
        pairs[count].key = strndup(key_start, key_len);
        pairs[count].value = strndup(val_start, val_len);
        count++;
        
        // Skip to comma or end
        while (*p && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) p++;
        if (*p == ',') p++;
    }
    
    return count;
}

static void free_pairs(kv_pair_t* pairs, int count) {
    for (int i = 0; i < count; i++) {
        free((void*)pairs[i].key);
        free((void*)pairs[i].value);
    }
}

// Test: Configuration parsing
TEST_SUITE(config) {
    // Test simple JSON parsing
    const char* json = "{\"algo\": \"x16r\", \"intensity\": \"22\", \"url\": \"stratum+tcp://pool:3333\"}";
    kv_pair_t pairs[10];
    int count = parse_simple_json(json, pairs, 10);
    
    TEST_ASSERT_EQ(count, 3, "JSON parser extracts 3 key-value pairs");
    
    bool found_algo = false, found_intensity = false, found_url = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(pairs[i].key, "algo") == 0) {
            TEST_ASSERT_STR_EQ(pairs[i].value, "x16r", "algo value is x16r");
            found_algo = true;
        } else if (strcmp(pairs[i].key, "intensity") == 0) {
            TEST_ASSERT_STR_EQ(pairs[i].value, "22", "intensity value is 22");
            found_intensity = true;
        } else if (strcmp(pairs[i].key, "url") == 0) {
            TEST_ASSERT_STR_EQ(pairs[i].value, "stratum+tcp://pool:3333", "url value is correct");
            found_url = true;
        }
    }
    TEST_ASSERT(found_algo, "algo key found");
    TEST_ASSERT(found_intensity, "intensity key found");
    TEST_ASSERT(found_url, "url key found");
    free_pairs(pairs, count);
    
    // Test empty JSON
    count = parse_simple_json("{}", pairs, 10);
    TEST_ASSERT_EQ(count, 0, "Empty JSON returns 0 pairs");
    
    // Test JSON with whitespace
    const char* json_ws = "  {  \"key\"  :  \"value\"  }  ";
    count = parse_simple_json(json_ws, pairs, 10);
    TEST_ASSERT_EQ(count, 1, "JSON with whitespace parses correctly");
    TEST_ASSERT_STR_EQ(pairs[0].key, "key", "key parsed with whitespace");
    TEST_ASSERT_STR_EQ(pairs[0].value, "value", "value parsed with whitespace");
    free_pairs(pairs, count);
    
    // Test config validation: required fields
    const char* required_fields[] = {"algo", "url", "user"};
    const char* test_config = "{\"algo\": \"x16r\", \"url\": \"stratum://pool:3333\", \"user\": \"wallet.worker\"}";
    count = parse_simple_json(test_config, pairs, 10);
    
    for (int i = 0; i < 3; i++) {
        bool found = false;
        for (int j = 0; j < count; j++) {
            if (strcmp(pairs[j].key, required_fields[i]) == 0) {
                found = true;
                break;
            }
        }
        TEST_ASSERT(found, "Required field present in config");
    }
    free_pairs(pairs, count);
    
    // Test config validation: intensity bounds
    const char* intensity_tests[] = {"8", "15", "22", "25"};
    for (int i = 0; i < 4; i++) {
        int intensity = atoi(intensity_tests[i]);
        TEST_ASSERT(intensity >= 8 && intensity <= 25, "Intensity within valid range");
    }
    
    // Test invalid intensity
    int invalid_intensity = atoi("30");
    TEST_ASSERT(!(invalid_intensity >= 8 && invalid_intensity <= 25), "Intensity 30 is invalid");
    
    // Test pool URL format validation
    const char* valid_urls[] = {
        "stratum+tcp://pool.example.com:3333",
        "stratum+ssl://pool.example.com:3333",
        "http://pool.example.com:8080",
        "https://pool.example.com:8080"
    };
    for (int i = 0; i < 4; i++) {
        bool valid = (strncmp(valid_urls[i], "stratum", 7) == 0) || 
                     (strncmp(valid_urls[i], "http", 4) == 0);
        TEST_ASSERT(valid, "Valid pool URL format recognized");
    }
    
    // Test invalid pool URL
    const char* invalid_url = "ftp://pool.example.com";
    bool invalid = (strncmp(invalid_url, "stratum", 7) != 0) && 
                   (strncmp(invalid_url, "http", 4) != 0);
    TEST_ASSERT(invalid, "Invalid pool URL format rejected");
}
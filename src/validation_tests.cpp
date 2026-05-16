/**
 * @file validation_tests.cpp
 * @brief Validation test sequence for SimpleFaceMe
 * 
 * Comprehensive hardware and software validation tests
 * Enabled with TEST_MODE=1 build flag
 */

#include "config.h"

#if TEST_MODE == 1

#include <Arduino.h>
#include "servo_control.h"
#include "geometry.h"
#include <LD2450.h>

// External references
extern ServoControl servos;
extern LD2450 radar;
extern bool hasValidTarget;
extern unsigned long lastValidTargetTime;

// Forward declarations for helper functions in main.cpp
struct ClosestTarget {
    int16_t x;
    int16_t y;
    uint16_t distance;
    bool valid;
};
extern ClosestTarget getClosestTarget();

/**
 * Comprehensive validation test sequence
 * Tests servo range, geometry calculations, and system integration
 */
void runValidationTests() {
    SYSTEM_PRINTLN("\n========================================");
    SYSTEM_PRINTLN("VALIDATION TEST SEQUENCE");
    SYSTEM_PRINTLN("========================================\n");
    
    // Test 1: Servo Range Verification
    SYSTEM_PRINTLN("[TEST 1] Servo Range Test");
    SYSTEM_PRINTLN("Testing angles: -90°, -45°, 0°, +45°, +90°");
    float testAngles[] = {-90.0f, -45.0f, 0.0f, 45.0f, 90.0f};
    
    for (uint8_t i = 0; i < NUM_SERVOS; i++) {
        SYSTEM_PRINTF("\nServo %d:\n", i);
        for (int a = 0; a < 5; a++) {
            SYSTEM_PRINTF("  Setting %.0f°... ", testAngles[a]);
            servos.setPosition(i, testAngles[a]);
            delay(1500);
            SYSTEM_PRINTLN("OK");
        }
        servos.setCenter(i);
    }
    
    SYSTEM_PRINTLN("\n[TEST 1] ✓ Complete - verify servos moved correctly\n");
    delay(1000);
    
    // Test 2: Geometry Calculation Verification
    SYSTEM_PRINTLN("[TEST 2] Geometry Calculation Test");
    SYSTEM_PRINTLN("Testing with known target positions:\n");
    
    struct TestCase {
        int16_t x;
        int16_t y;
        const char* description;
    };
    
    TestCase testCases[] = {
        {0, 1000, "Center forward (1m)"},
        {500, 1000, "Right 500mm, forward 1m"},
        {-500, 1000, "Left 500mm, forward 1m"},
        {1000, 1000, "Right 1m, forward 1m (45°)"},
        {-1000, 1000, "Left 1m, forward 1m (-45°)"}
    };
    
    for (int t = 0; t < 5; t++) {
        TargetPosition testTarget;
        testTarget.x = testCases[t].x;
        testTarget.y = testCases[t].y;
        testTarget.valid = true;
        
        SYSTEM_PRINTF("%s\n", testCases[t].description);
        SYSTEM_PRINTF("  Target: (%d, %d) mm\n", testTarget.x, testTarget.y);
        
        float angle0 = calculateServo0Angle(testTarget);
        SYSTEM_PRINTF("  Servo 0 angle: %.1f°\n", angle0);
        servos.setPosition(0, angle0);
        
        #if NUM_SERVOS == 2
            float angle1 = calculateServo1Angle(testTarget);
            SYSTEM_PRINTF("  Servo 1 angle: %.1f°\n", angle1);
            servos.setPosition(1, angle1);
        #endif
        
        delay(2000);
    }
    
    servos.centerAll();
    SYSTEM_PRINTLN("\n[TEST 2] ✓ Complete - verify angles match expected\n");
    delay(1000);
    
    // Test 3: Radar Integration Test
    SYSTEM_PRINTLN("[TEST 3] Radar Integration Test");
    SYSTEM_PRINTLN("Reading 10 radar samples...");
    SYSTEM_PRINTLN("Move your hand in front of sensor\n");
    
    for (int sample = 0; sample < 10; sample++) {
        int validTargets = radar.read();
        SYSTEM_PRINTF("Sample %d: ", sample + 1);
        
        if (validTargets > 0) {
            ClosestTarget closest = getClosestTarget();
            if (closest.valid) {
                SYSTEM_PRINTF("Target at (%d, %d) mm, dist=%d mm\n",
                             closest.x, closest.y, closest.distance);
                
                // Calculate and show angles
                TargetPosition target;
                target.x = closest.x;
                target.y = closest.y;
                target.valid = true;
                
                float angle0 = calculateServo0Angle(target);
                SYSTEM_PRINTF("  → Servo angles: [0]=%.1f°", angle0);
                
                #if NUM_SERVOS == 2
                    float angle1 = calculateServo1Angle(target);
                    SYSTEM_PRINTF(", [1]=%.1f°", angle1);
                #endif
                SYSTEM_PRINTLN("");
            } else {
                SYSTEM_PRINTLN("No valid target");
            }
        } else {
            SYSTEM_PRINTLN("No targets detected");
        }
        
        delay(500);
    }
    
    servos.centerAll();
    SYSTEM_PRINTLN("\n[TEST 3] ✓ Complete\n");
    
    // Test 4: Timeout Behavior
    SYSTEM_PRINTLN("[TEST 4] Timeout Test");
    SYSTEM_PRINTF("Waiting %d ms with no target...\n", TARGET_TIMEOUT_MS);
    
    hasValidTarget = true;
    lastValidTargetTime = millis();
    servos.setPosition(0, 45.0f);  // Move servo off-center
    
    SYSTEM_PRINTLN("Servo moved to 45° - should return to center on timeout");
    
    while (millis() - lastValidTargetTime < TARGET_TIMEOUT_MS + 500) {
        unsigned long elapsed = millis() - lastValidTargetTime;
        if (elapsed > TARGET_TIMEOUT_MS && hasValidTarget) {
            SYSTEM_PRINTLN("→ Timeout triggered - centering servos");
            servos.centerAll();
            hasValidTarget = false;
        }
        delay(100);
    }
    
    SYSTEM_PRINTLN("\n[TEST 4] ✓ Complete\n");
    
    // Summary
    SYSTEM_PRINTLN("========================================");
    SYSTEM_PRINTLN("VALIDATION TESTS COMPLETE");
    SYSTEM_PRINTLN("========================================");
    SYSTEM_PRINTLN("Review test results and verify:");
    SYSTEM_PRINTLN("  1. Servos reached all test angles");
    SYSTEM_PRINTLN("  2. Geometry calculations match expectations");
    SYSTEM_PRINTLN("  3. Radar detects and tracks targets");
    SYSTEM_PRINTLN("  4. Timeout returns servos to center");
    SYSTEM_PRINTLN("\nNormal tracking mode starting...\n");
    
    delay(2000);
}

#endif // TEST_MODE == 1

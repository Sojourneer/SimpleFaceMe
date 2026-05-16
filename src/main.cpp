#include <Arduino.h>
#include "config.h"
#include <LD2450.h>
#include "servo_control.h"
#include "geometry.h"

#if TEST_MODE == 1
// Forward declaration for validation tests
void runValidationTests();
#endif

/**
 * SimpleFaceMe - LD2450 Radar Target Tracking Demo
 * 
 * Complete integration: Radar → Geometry → Servo control
 */

#ifdef RUNTIME_MODE_SELECT
// Runtime configuration globals (definitions)
uint8_t g_numServos = 1;
ServoConfig g_servo0Config = {0, 0};
ServoConfig g_servo1Config = {0, 0};
#endif

// Helper macros for servo access (runtime vs compile-time)
#ifdef RUNTIME_MODE_SELECT
    #define SERVOS servos->
    #define NUM_SERVOS_ACTUAL g_numServos
#else
    #define SERVOS servos.
    #define NUM_SERVOS_ACTUAL NUM_SERVOS
#endif

// LD2450 Radar instance
LD2450 radar;
HardwareSerial radarSerial(LD2450_UART_NUM);

// Servo control instance
#ifdef RUNTIME_MODE_SELECT
ServoControl* servos = nullptr;  // Runtime allocation
#else
ServoControl servos(PCA9685_ADDR, NUM_SERVOS);  // Compile-time
#endif

// Target tracking state
unsigned long lastValidTargetTime = 0;
bool hasValidTarget = false;

// Helper structure for closest target
struct ClosestTarget {
    int16_t x;
    int16_t y;
    uint16_t distance;
    bool valid;
};

/**
 * Find closest valid target from radar data
 * Returns target with shortest distance (sqrt(x^2 + y^2))
 * Applies coordinate transformations from build_flags
 */
ClosestTarget getClosestTarget() {
    ClosestTarget closest;
    closest.valid = false;
    closest.distance = 65535; // Max uint16_t
    
    for (int i = 0; i < radar.getSensorSupportedTargetCount(); i++) {
        LD2450::RadarTarget target = radar.getTarget(i);
        
        if (target.valid && target.distance > 0) {
            if (target.distance < closest.distance) {
                // Apply coordinate transformations
                int16_t x = target.x;
                int16_t y = target.y;
                
                #if LD2450_FLIP_X == 1
                    x = -x;
                #endif
                
                #if LD2450_FLIP_Y == 1
                    y = -y;
                #endif
                
                x += LD2450_OFFSET_X;
                y += LD2450_OFFSET_Y;
                
                closest.x = x;
                closest.y = y;
                closest.distance = target.distance;
                closest.valid = true;
            }
        }
    }
    
    return closest;
}

/**
 * Get time since last valid target detected
 */
unsigned long timeSinceLastTarget() {
    if (hasValidTarget) {
        return millis() - lastValidTargetTime;
    }
    return 0xFFFFFFFF; // Very large number if never had target
}

void setup() {
    Serial.begin(DEBUG_SERIAL_BAUD);
    delay(1000);
    
    SYSTEM_PRINTLN("\n\n========================================");
    SYSTEM_PRINTLN("SimpleFaceMe - Target Tracking Demo");
    SYSTEM_PRINTLN("========================================\n");
    
    #ifdef RUNTIME_MODE_SELECT
        // Detect mode and configure runtime settings
        SYSTEM_PRINTLN("--- Runtime Mode Detection ---");
        initRuntimeConfig();
        
        // Create servo controller with detected configuration
        servos = new ServoControl(PCA9685_ADDR, g_numServos);
        SYSTEM_PRINTLN();
    #endif
    
    // Print configuration
    printConfiguration();
    
    // Initialize servos FIRST (safer - can see servo response immediately)
    SYSTEM_PRINTLN("\n--- Initializing Servos ---");
    #ifdef RUNTIME_MODE_SELECT
        if (servos->begin()) {
    #else
        if (servos.begin()) {
    #endif
        SYSTEM_PRINTLN("✓ Servos initialized and centered");
    } else {
        SYSTEM_PRINTLN("✗ Servo initialization failed!");
    }
    
    // Initialize LD2450 radar
    SYSTEM_PRINTLN("\n--- Initializing LD2450 Radar ---");
    radarSerial.begin(LD2450_BAUD, SERIAL_8N1, LD2450_RX_PIN, LD2450_TX_PIN);
    radar.begin(radarSerial, false);
    
    SYSTEM_PRINTLN("Waiting for radar data...");
    if (radar.waitForSensorMessage(false)) {
        SYSTEM_PRINTLN("✓ LD2450 initialized successfully");
    } else {
        SYSTEM_PRINTLN("⚠ LD2450 init timeout (may still work)");
    }
    
    SYSTEM_PRINTLN("\n--- Startup Demo ---");
    SYSTEM_PRINTLN("Testing servo range...");
    
    // Demo servo sweep
    delay(1000);
    SYSTEM_PRINTLN("Sweep: -90° → 0° → +90° → 0°");
    for (uint8_t i = 0; i < NUM_SERVOS_ACTUAL; i++) {
        SERVOS setPosition(i, -90.0f);
    }
    delay(1000);
    for (uint8_t i = 0; i < NUM_SERVOS_ACTUAL; i++) {
        SERVOS setPosition(i, 0.0f);
    }
    delay(1000);
    for (uint8_t i = 0; i < NUM_SERVOS_ACTUAL; i++) {
        SERVOS setPosition(i, 90.0f);
    }
    delay(1000);
    for (uint8_t i = 0; i < NUM_SERVOS_ACTUAL; i++) {
        SERVOS setPosition(i, 0.0f);
    }
    delay(500);
    
    SYSTEM_PRINTLN("✓ Servo sweep complete - tracking active\n");
    SYSTEM_PRINTLN("Debug flags:");
    SYSTEM_PRINTF("  SYSTEM=%d, SENSOR=%d, MOTOR=%d, GEOMETRY=%d\n",
                  DEBUG_SYSTEM, DEBUG_SENSOR, DEBUG_MOTOR, DEBUG_GEOMETRY);
    SYSTEM_PRINTF("  TEST_MODE=%d\n\n", TEST_MODE);
    
    #if TEST_MODE == 1
        runValidationTests();
    #endif

    SYSTEM_PRINTF("Entering main loop with %s servos\n", (NUM_SERVOS_ACTUAL == 1) ? "SINGLE" : "DUAL");
}

void loop() {
    // Read radar data
    int validTargets = radar.read();
    
    if (validTargets > 0) {
        // Get all targets for debug
        SENSOR_PRINTF("Valid targets: %d\n", validTargets);
        
        for (int i = 0; i < radar.getSensorSupportedTargetCount(); i++) {
            LD2450::RadarTarget target = radar.getTarget(i);
            if (target.valid) {
                // Apply transformations for debug output
                int16_t x = target.x;
                int16_t y = target.y;
                #if LD2450_FLIP_X == 1
                    x = -x;
                #endif
                #if LD2450_FLIP_Y == 1
                    y = -y;
                #endif
                x += LD2450_OFFSET_X;
                y += LD2450_OFFSET_Y;
                
                SENSOR_PRINTF("  Target %d: X=%d mm, Y=%d mm, Dist=%d mm\n", 
                             i, x, y, target.distance);
            }
        }
        
        // Find closest target
        ClosestTarget closest = getClosestTarget();
        
        if (closest.valid) {
            // Update timeout tracking
            lastValidTargetTime = millis();
            hasValidTarget = true;
            
            #if DEBUG_VERBOSE
            // Print closest target (always, even if SENSOR debug off)
            if (DEBUG_SENSOR) {
                Serial.printf("→ TARGET: X=%d mm, Y=%d mm, Distance=%d mm\n", 
                             closest.x, closest.y, closest.distance);
            } else {
                SENSOR_PRINTF("→ CLOSEST: X=%d mm, Y=%d mm\n", 
                             closest.x, closest.y);
            }
            #endif

            // Convert to TargetPosition for geometry calculation
            TargetPosition target;
            target.x = closest.x;
            target.y = closest.y;
            target.valid = true;
            
            // Calculate servo angles using proper geometry
            float angle0 = calculateServo0Angle(target);
            SERVOS setPosition(0, angle0);
            
            #ifdef RUNTIME_MODE_SELECT
                // Runtime: check actual servo count
                if (NUM_SERVOS_ACTUAL == 2) {
                    float angle1 = calculateServo1Angle(target);
                    SERVOS setPosition(1, angle1);
                    
                    if (DEBUG_MOTOR || DEBUG_GEOMETRY) {
                        Serial.printf("  Servo angles: [0]=%.1f°, [1]=%.1f°\n", angle0, angle1);
                    }
                } else {
                    if (DEBUG_MOTOR || DEBUG_GEOMETRY) {
                        Serial.printf("  Servo angle: %.1f°\n", angle0);
                    }
                }
            #else
                // Compile-time: use preprocessor
                #if NUM_SERVOS == 2
                    float angle1 = calculateServo1Angle(target);
                    SERVOS setPosition(1, angle1);
                    
                    if (DEBUG_MOTOR || DEBUG_GEOMETRY) {
                        Serial.printf("  Servo angles: [0]=%.1f°, [1]=%.1f°\n", angle0, angle1);
                    }
                #else
                    if (DEBUG_MOTOR || DEBUG_GEOMETRY) {
                        Serial.printf("  Servo angle: %.1f°\n", angle0);
                    }
                #endif
            #endif
        }
    } else if (validTargets == 0) {
        SENSOR_PRINTLN("No targets detected");
    } else {
        // Error codes
        SENSOR_PRINTF("Radar read error: %d\n", validTargets);
    }
    
    // Check timeout - return to center
    unsigned long timeSince = timeSinceLastTarget();
    if (hasValidTarget && timeSince > TARGET_TIMEOUT_MS) {
        SYSTEM_PRINTF("⚠ Target timeout: %lu ms - centering servos\n", timeSince);
        SERVOS centerAll();
        hasValidTarget = false; // Reset so we don't spam this message
    }
    
    delay(SERVO_UPDATE_PERIOD_MS);  // Respect configured update rate
}
/**
 * @file config.h
 * @brief Build configuration validation for SimpleFaceMe
 * 
 * All configuration MUST be defined via platformio.ini build_flags.
 * This file validates required parameters and provides compile-time errors.
 * 
 * Per platformio-practices: No defaults in code, all values from build system.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//=============================================================================
// HARDWARE PIN CONFIGURATION VALIDATION
//=============================================================================

#ifndef DEBUG_SERIAL_BAUD
    #error "DEBUG_SERIAL_BAUD must be defined in platformio.ini build_flags"
#endif

#ifndef LD2450_UART_NUM
    #error "LD2450_UART_NUM must be defined in platformio.ini build_flags"
#endif

#ifndef LD2450_TX_PIN
    #error "LD2450_TX_PIN must be defined in platformio.ini build_flags"
#endif

#ifndef LD2450_RX_PIN
    #error "LD2450_RX_PIN must be defined in platformio.ini build_flags"
#endif

#ifndef PCA9685_SDA_PIN
    #error "PCA9685_SDA_PIN must be defined in platformio.ini build_flags"
#endif

#ifndef PCA9685_SCL_PIN
    #error "PCA9685_SCL_PIN must be defined in platformio.ini build_flags"
#endif

//=============================================================================
// GEOMETRY CONFIGURATION VALIDATION
//=============================================================================

#ifndef SENSOR_OFFSET_X
    #error "SENSOR_OFFSET_X must be defined in platformio.ini build_flags"
#endif

#ifndef SENSOR_OFFSET_Y
    #error "SENSOR_OFFSET_Y must be defined in platformio.ini build_flags"
#endif

//=============================================================================
// SERVO CONFIGURATION - Runtime or Compile-time
//=============================================================================

#ifdef RUNTIME_MODE_SELECT
    // Runtime mode selection via GPIO pin
    #ifndef MODE_SELECT_PIN
        #error "MODE_SELECT_PIN must be defined when RUNTIME_MODE_SELECT=1"
    #endif
    
    // Single servo geometry
    #ifndef SINGLE_SERVO_OFFSET_X
        #error "SINGLE_SERVO_OFFSET_X must be defined for runtime mode"
    #endif
    #ifndef SINGLE_SERVO_OFFSET_Y
        #error "SINGLE_SERVO_OFFSET_Y must be defined for runtime mode"
    #endif
    
    // Dual servo geometry
    #ifndef DUAL_SERVO1_OFFSET_X
        #error "DUAL_SERVO1_OFFSET_X must be defined for runtime mode"
    #endif
    #ifndef DUAL_SERVO1_OFFSET_Y
        #error "DUAL_SERVO1_OFFSET_Y must be defined for runtime mode"
    #endif
    #ifndef DUAL_SERVO2_OFFSET_X
        #error "DUAL_SERVO2_OFFSET_X must be defined for runtime mode"
    #endif
    #ifndef DUAL_SERVO2_OFFSET_Y
        #error "DUAL_SERVO2_OFFSET_Y must be defined for runtime mode"
    #endif
#else
    // Compile-time mode selection
    #ifndef NUM_SERVOS
        #error "NUM_SERVOS must be defined in platformio.ini build_flags"
    #endif
    
    // Single servo configuration
    #if NUM_SERVOS == 1
        #ifndef SERVO_OFFSET_X
            #error "SERVO_OFFSET_X must be defined for single servo build"
        #endif
        #ifndef SERVO_OFFSET_Y
            #error "SERVO_OFFSET_Y must be defined for single servo build"
        #endif
    #endif
    
    // Dual servo configuration
    #if NUM_SERVOS == 2
        #ifndef SERVO1_OFFSET_X
            #error "SERVO1_OFFSET_X must be defined for dual servo build"
        #endif
        #ifndef SERVO1_OFFSET_Y
            #error "SERVO1_OFFSET_Y must be defined for dual servo build"
        #endif
        #ifndef SERVO2_OFFSET_X
            #error "SERVO2_OFFSET_X must be defined for dual servo build"
        #endif
        #ifndef SERVO2_OFFSET_Y
            #error "SERVO2_OFFSET_Y must be defined for dual servo build"
        #endif
    #endif
#endif

//=============================================================================
// HARDWARE CONFIGURATION VALIDATION
//=============================================================================

#ifndef LD2450_BAUD
    #error "LD2450_BAUD must be defined in platformio.ini build_flags"
#endif

#ifndef PCA9685_ADDR
    #error "PCA9685_ADDR must be defined in platformio.ini build_flags"
#endif

#ifndef SERVO_FREQ
    #error "SERVO_FREQ must be defined in platformio.ini build_flags"
#endif

#ifndef LD2450_FLIP_X
    #error "LD2450_FLIP_X must be defined in platformio.ini build_flags"
#endif

#ifndef LD2450_FLIP_Y
    #error "LD2450_FLIP_Y must be defined in platformio.ini build_flags"
#endif

#ifndef LD2450_OFFSET_X
    #error "LD2450_OFFSET_X must be defined in platformio.ini build_flags"
#endif

#ifndef LD2450_OFFSET_Y
    #error "LD2450_OFFSET_Y must be defined in platformio.ini build_flags"
#endif

//=============================================================================
// SERVO CALIBRATION VALIDATION
//=============================================================================

#ifndef PULSE_MIN_US
    #error "PULSE_MIN_US must be defined in platformio.ini build_flags"
#endif

#ifndef PULSE_CENTER_US
    #error "PULSE_CENTER_US must be defined in platformio.ini build_flags"
#endif

#ifndef PULSE_MAX_US
    #error "PULSE_MAX_US must be defined in platformio.ini build_flags"
#endif

//=============================================================================
// BEHAVIOR PARAMETERS VALIDATION
//=============================================================================

#ifndef SERVO_UPDATE_PERIOD_MS
    #error "SERVO_UPDATE_PERIOD_MS must be defined in platformio.ini build_flags"
#endif

#ifndef TARGET_TIMEOUT_MS
    #error "TARGET_TIMEOUT_MS must be defined in platformio.ini build_flags"
#endif

#ifndef DEBUG_VERBOSE
    #error "DEBUG_VERBOSE must be defined in platformio.ini build_flags"
#endif

#ifndef DEBUG_SYSTEM
    #error "DEBUG_SYSTEM must be defined in platformio.ini build_flags"
#endif

#ifndef DEBUG_SENSOR
    #error "DEBUG_SENSOR must be defined in platformio.ini build_flags"
#endif

#ifndef DEBUG_MOTOR
    #error "DEBUG_MOTOR must be defined in platformio.ini build_flags"
#endif

#ifndef DEBUG_GEOMETRY
    #error "DEBUG_GEOMETRY must be defined in platformio.ini build_flags"
#endif

#ifndef TEST_MODE
    #error "TEST_MODE must be defined in platformio.ini build_flags"
#endif

//=============================================================================
// DEBUG MACROS - Categorized with token concatenation
//=============================================================================

// Helper macro for token concatenation
#define CAT_ENABLED(cat) DEBUG_##cat

// Category-aware debug macros (compact using token pasting)
#define DEBUG_PRINT(cat, x) do { if (CAT_ENABLED(cat)) { Serial.print(x); } } while(0)
#define DEBUG_PRINTLN(cat, x) do { if (CAT_ENABLED(cat)) { Serial.println(x); } } while(0)
#define DEBUG_PRINTF(cat, ...) do { if (CAT_ENABLED(cat)) { Serial.printf(__VA_ARGS__); } } while(0)

// Legacy macros for backward compatibility (controlled by DEBUG_VERBOSE)
#if DEBUG_VERBOSE == 1
    #define DEBUG_PRINT_LEGACY(x) Serial.print(x)
    #define DEBUG_PRINTLN_LEGACY(x) Serial.println(x)
#else
    #define DEBUG_PRINT_LEGACY(x)
    #define DEBUG_PRINTLN_LEGACY(x)
#endif

// Category-specific convenience macros
#define SYSTEM_PRINT(x)    DEBUG_PRINT(SYSTEM, x)
#define SYSTEM_PRINTLN(x)  DEBUG_PRINTLN(SYSTEM, x)
#define SYSTEM_PRINTF(...) DEBUG_PRINTF(SYSTEM, __VA_ARGS__)

#define SENSOR_PRINT(x)    DEBUG_PRINT(SENSOR, x)
#define SENSOR_PRINTLN(x)  DEBUG_PRINTLN(SENSOR, x)
#define SENSOR_PRINTF(...) DEBUG_PRINTF(SENSOR, __VA_ARGS__)

#define MOTOR_PRINT(x)     DEBUG_PRINT(MOTOR, x)
#define MOTOR_PRINTLN(x)   DEBUG_PRINTLN(MOTOR, x)
#define MOTOR_PRINTF(...)  DEBUG_PRINTF(MOTOR, __VA_ARGS__)

#define GEOMETRY_PRINT(x)     DEBUG_PRINT(GEOMETRY, x)
#define GEOMETRY_PRINTLN(x)   DEBUG_PRINTLN(GEOMETRY, x)
#define GEOMETRY_PRINTF(...)  DEBUG_PRINTF(GEOMETRY, __VA_ARGS__)

//=============================================================================
// RUNTIME MODE CONFIGURATION
//=============================================================================

#ifdef RUNTIME_MODE_SELECT

// Servo configuration structure (stored in program memory)
struct ServoConfig {
    int16_t offset_x;
    int16_t offset_y;
};

// Global runtime configuration (extern declaration)
extern uint8_t g_numServos;
extern ServoConfig g_servo0Config;
extern ServoConfig g_servo1Config;

// Mode detection function
inline uint8_t detectServoMode() {
    pinMode(MODE_SELECT_PIN, INPUT_PULLUP);
    delay(10);  // Allow pin to stabilize
    
    // LOW (grounded) = dual servo, HIGH (pullup) = single servo
    bool pinState = digitalRead(MODE_SELECT_PIN);
    
    SYSTEM_PRINTF("Mode select pin (GPIO%d): %s\n", 
                  MODE_SELECT_PIN, pinState ? "HIGH" : "LOW");
    
    if (pinState) {
        // Single servo mode
        SYSTEM_PRINTLN("→ SINGLE SERVO MODE");
        return 1;
    } else {
        // Dual servo mode
        SYSTEM_PRINTLN("→ DUAL SERVO MODE");
        return 2;
    }
}

// Initialize runtime configuration
inline void initRuntimeConfig() {
    g_numServos = detectServoMode();
    
    if (g_numServos == 1) {
        // Single servo
        g_servo0Config.offset_x = SINGLE_SERVO_OFFSET_X;
        g_servo0Config.offset_y = SINGLE_SERVO_OFFSET_Y;
    } else {
        // Dual servo
        g_servo0Config.offset_x = DUAL_SERVO1_OFFSET_X;
        g_servo0Config.offset_y = DUAL_SERVO1_OFFSET_Y;
        g_servo1Config.offset_x = DUAL_SERVO2_OFFSET_X;
        g_servo1Config.offset_y = DUAL_SERVO2_OFFSET_Y;
    }
    
    SYSTEM_PRINTF("Configured for %d servo(s)\n", g_numServos);
}

#endif // RUNTIME_MODE_SELECT

//=============================================================================
// CONFIGURATION SUMMARY (for verification)
//=============================================================================

inline void printConfiguration() {
    Serial.println("=== SimpleFaceMe Configuration ===");
    
    Serial.println("\n--- Hardware Pins ---");
    Serial.print("Debug Serial: USB CDC @ "); Serial.print(DEBUG_SERIAL_BAUD); Serial.println(" baud");
    Serial.print("LD2450 UART: "); Serial.print(LD2450_UART_NUM);
    Serial.print(" (TX="); Serial.print(LD2450_TX_PIN);
    Serial.print(", RX="); Serial.print(LD2450_RX_PIN); Serial.println(")");
    Serial.print("PCA9685 I2C: SDA="); Serial.print(PCA9685_SDA_PIN);
    Serial.print(", SCL="); Serial.println(PCA9685_SCL_PIN);
    
    Serial.println("\n--- Geometry (mm) ---");
    Serial.print("Sensor: ("); Serial.print(SENSOR_OFFSET_X);
    Serial.print(", "); Serial.print(SENSOR_OFFSET_Y); Serial.println(")");
    
    #ifdef RUNTIME_MODE_SELECT
        Serial.print("Num Servos: "); Serial.print(g_numServos);
        Serial.println(" (runtime detected)");
        Serial.print("Servo 0: ("); Serial.print(g_servo0Config.offset_x);
        Serial.print(", "); Serial.print(g_servo0Config.offset_y); Serial.println(")");
        if (g_numServos == 2) {
            Serial.print("Servo 1: ("); Serial.print(g_servo1Config.offset_x);
            Serial.print(", "); Serial.print(g_servo1Config.offset_y); Serial.println(")");
        }
    #else
        #if NUM_SERVOS == 1
            Serial.print("Servo: ("); Serial.print(SERVO_OFFSET_X);
            Serial.print(", "); Serial.print(SERVO_OFFSET_Y); Serial.println(")");
        #else
            Serial.print("Servo 1: ("); Serial.print(SERVO1_OFFSET_X);
            Serial.print(", "); Serial.print(SERVO1_OFFSET_Y); Serial.println(")");
            Serial.print("Servo 2: ("); Serial.print(SERVO2_OFFSET_X);
            Serial.print(", "); Serial.print(SERVO2_OFFSET_Y); Serial.println(")");
        #endif
    #endif
    
    Serial.println("\n--- Hardware Config ---");
    #ifdef RUNTIME_MODE_SELECT
        Serial.print("Num Servos: "); Serial.print(g_numServos);
        Serial.println(" (runtime)");
    #else
        Serial.print("Num Servos: "); Serial.println(NUM_SERVOS);
    #endif
    Serial.print("LD2450 Baud: "); Serial.println(LD2450_BAUD);
    Serial.print("PCA9685 Addr: 0x"); Serial.println(PCA9685_ADDR, HEX);
    Serial.print("Servo Freq: "); Serial.print(SERVO_FREQ); Serial.println(" Hz");
    
    Serial.println("\n--- LD2450 Coordinate Transform ---");
    Serial.print("Flip X: "); Serial.println(LD2450_FLIP_X ? "YES" : "NO");
    Serial.print("Flip Y: "); Serial.println(LD2450_FLIP_Y ? "YES" : "NO");
    Serial.print("Offset X: "); Serial.print(LD2450_OFFSET_X); Serial.println(" mm");
    Serial.print("Offset Y: "); Serial.print(LD2450_OFFSET_Y); Serial.println(" mm");
    
    Serial.println("\n--- Servo Calibration (µs) ---");
    Serial.print("Pulse Min: "); Serial.println(PULSE_MIN_US);
    Serial.print("Pulse Center: "); Serial.println(PULSE_CENTER_US);
    Serial.print("Pulse Max: "); Serial.println(PULSE_MAX_US);
    
    Serial.println("\n--- Behavior ---");
    Serial.print("Servo Update: "); Serial.print(SERVO_UPDATE_PERIOD_MS); Serial.println(" ms");
    Serial.print("Target Timeout: "); Serial.print(TARGET_TIMEOUT_MS); Serial.println(" ms");
    Serial.print("Debug Verbose: "); Serial.println(DEBUG_VERBOSE);
    
    Serial.println("\n--- Debug Categories ---");
    Serial.print("SYSTEM:   "); Serial.println(DEBUG_SYSTEM ? "ENABLED" : "disabled");
    Serial.print("SENSOR:   "); Serial.println(DEBUG_SENSOR ? "ENABLED" : "disabled");
    Serial.print("MOTOR:    "); Serial.println(DEBUG_MOTOR ? "ENABLED" : "disabled");
    Serial.print("GEOMETRY: "); Serial.println(DEBUG_GEOMETRY ? "ENABLED" : "disabled");
    
    Serial.println("\n==================================");
}

#endif // CONFIG_H

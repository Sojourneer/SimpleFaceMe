/**
 * @file geometry.cpp
 * @brief Geometry calculations implementation
 */

#include "geometry.h"
#include <math.h>

float calculateServoAngle(const TargetPosition& target, 
                         int16_t servo_offset_x, 
                         int16_t servo_offset_y) {
    if (!target.valid) {
        return 0.0f;  // Return center position for invalid targets
    }
    
    // Calculate target position relative to servo
    // Target coordinates are in sensor frame
    // Servo offsets are relative to sensor
    int16_t rel_x = target.x - servo_offset_x;
    int16_t rel_y = target.y - servo_offset_y;
    
    GEOMETRY_PRINTF("Target: (%d, %d) mm, Servo offset: (%d, %d) mm\n",
                   target.x, target.y, servo_offset_x, servo_offset_y);
    GEOMETRY_PRINTF("Relative: (%d, %d) mm\n", rel_x, rel_y);
    
    // Calculate angle using atan2
    // atan2(x, y) gives angle from forward (+Y) axis to target
    // This matches servo convention: 0° = straight ahead, +angle = right, -angle = left
    float angle_rad = atan2((float)rel_x, (float)rel_y);
    float angle_deg = angle_rad * 180.0f / PI;
    
    GEOMETRY_PRINTF("Calculated angle: %.1f°\n", angle_deg);
    
    // Clamp to servo limits
    if (angle_deg < -90.0f) {
        GEOMETRY_PRINTF("Clamping %.1f° to -90°\n", angle_deg);
        angle_deg = -90.0f;
    }
    if (angle_deg > 90.0f) {
        GEOMETRY_PRINTF("Clamping %.1f° to +90°\n", angle_deg);
        angle_deg = 90.0f;
    }
    
    return angle_deg;
}

float calculateServo0Angle(const TargetPosition& target) {
    #ifdef RUNTIME_MODE_SELECT
        // Runtime mode - use global configuration
        return calculateServoAngle(target, g_servo0Config.offset_x, g_servo0Config.offset_y);
    #else
        // Compile-time mode
        #if NUM_SERVOS == 1
            return calculateServoAngle(target, SERVO_OFFSET_X, SERVO_OFFSET_Y);
        #else
            return calculateServoAngle(target, SERVO1_OFFSET_X, SERVO1_OFFSET_Y);
        #endif
    #endif
}

float calculateServo1Angle(const TargetPosition& target) {
    #ifdef RUNTIME_MODE_SELECT
        // Runtime mode - use global configuration
        return calculateServoAngle(target, g_servo1Config.offset_x, g_servo1Config.offset_y);
    #else
        // Compile-time mode - only valid for dual servo
        #if NUM_SERVOS == 2
            return calculateServoAngle(target, SERVO2_OFFSET_X, SERVO2_OFFSET_Y);
        #else
            return 0.0f;  // Should never be called in single servo mode
        #endif
    #endif
}

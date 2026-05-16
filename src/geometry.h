/**
 * @file geometry.h
 * @brief Geometry calculations for servo targeting
 * 
 * Calculates servo angles needed to point at radar targets
 * based on sensor and servo positions defined in build configuration
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <Arduino.h>
#include "config.h"

/**
 * Target position in sensor coordinate system
 */
struct TargetPosition {
    int16_t x;  // X coordinate in mm (left-/right+)
    int16_t y;  // Y coordinate in mm (forward distance)
    bool valid; // Whether target is valid
};

/**
 * Calculate servo angle to point at target
 * 
 * @param target Target position in sensor coordinates (mm)
 * @param servo_offset_x Servo X position relative to sensor (mm)
 * @param servo_offset_y Servo Y position relative to sensor (mm)
 * @return Angle in degrees (-90 to +90, 0=straight ahead)
 *         Returns 0.0 if target is invalid
 */
float calculateServoAngle(const TargetPosition& target, 
                         int16_t servo_offset_x, 
                         int16_t servo_offset_y);

/**
 * Calculate servo angle for servo 0 (single servo or left servo in dual config)
 */
float calculateServo0Angle(const TargetPosition& target);

/**
 * Calculate servo angle for servo 1 (right servo in dual config)
 * Only used when num_servos == 2
 */
float calculateServo1Angle(const TargetPosition& target);

#endif // GEOMETRY_H

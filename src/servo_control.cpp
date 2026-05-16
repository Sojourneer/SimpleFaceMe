/**
 * @file servo_control.cpp
 * @brief Servo control implementation using PCA9685
 */

#include "servo_control.h"
#include <Wire.h>

ServoControl::ServoControl(uint8_t i2c_address, uint8_t num_servos)
    : pwm(Adafruit_PWMServoDriver(i2c_address)), numServos(num_servos) {
}

bool ServoControl::begin() {
    MOTOR_PRINTLN("Initializing PCA9685...");
    
    // Initialize I2C with configured pins
    Wire.begin(PCA9685_SDA_PIN, PCA9685_SCL_PIN);
    
    // Initialize PCA9685
    pwm.begin();
    
    // Set PWM frequency for servos (typically 50Hz)
    pwm.setPWMFreq(SERVO_FREQ);
    
    // Small delay for PCA9685 to stabilize
    delay(10);
    
    MOTOR_PRINTF("PCA9685 initialized at 0x%02X, %d Hz\n", PCA9685_ADDR, SERVO_FREQ);
    MOTOR_PRINTF("Controlling %d servo(s)\n", numServos);
    
    // Center all servos on startup
    centerAll();
    
    MOTOR_PRINTLN("Servos centered and ready");
    
    return true;
}

void ServoControl::setPosition(uint8_t servo_num, float angle_degrees) {
    if (servo_num >= numServos) {
        MOTOR_PRINTF("ERROR: Invalid servo num %d (max %d)\n", servo_num, numServos - 1);
        return;
    }
    
    // Clamp angle to safe range
    if (angle_degrees < -90.0f) angle_degrees = -90.0f;
    if (angle_degrees > 90.0f) angle_degrees = 90.0f;
    
    uint16_t pulse_us = angleToPulseWidth(angle_degrees);
    setPulseWidth(servo_num, pulse_us);
    
    MOTOR_PRINTF("Servo %d → %.1f° (%d µs)\n", servo_num, angle_degrees, pulse_us);
}

void ServoControl::setCenter(uint8_t servo_num) {
    setPosition(servo_num, 0.0f);
}

void ServoControl::centerAll() {
    MOTOR_PRINTLN("Centering all servos...");
    for (uint8_t i = 0; i < numServos; i++) {
        setCenter(i);
    }
}

void ServoControl::disable(uint8_t servo_num) {
    if (servo_num >= numServos) {
        return;
    }
    
    // Set all PWM values to 0 (no signal)
    pwm.setPWM(servo_num, 0, 0);
    MOTOR_PRINTF("Servo %d disabled\n", servo_num);
}

void ServoControl::disableAll() {
    MOTOR_PRINTLN("Disabling all servos...");
    for (uint8_t i = 0; i < numServos; i++) {
        disable(i);
    }
}

uint16_t ServoControl::angleToPulseWidth(float angle) {
    // Map angle [-90, +90] to pulse width [PULSE_MIN_US, PULSE_MAX_US]
    // 0 degrees = PULSE_CENTER_US
    // Linear mapping: pulse = center + (angle / 90) * (max - center)
    
    float pulse_us;
    if (angle >= 0) {
        // Positive angle: interpolate between center and max
        pulse_us = PULSE_CENTER_US + (angle / 90.0f) * (PULSE_MAX_US - PULSE_CENTER_US);
    } else {
        // Negative angle: interpolate between min and center
        pulse_us = PULSE_CENTER_US + (angle / 90.0f) * (PULSE_CENTER_US - PULSE_MIN_US);
    }
    
    // Clamp to absolute limits
    if (pulse_us < PULSE_MIN_US) pulse_us = PULSE_MIN_US;
    if (pulse_us > PULSE_MAX_US) pulse_us = PULSE_MAX_US;
    
    return (uint16_t)pulse_us;
}

void ServoControl::setPulseWidth(uint8_t servo_num, uint16_t pulse_us) {
    // Clamp to safe range
    if (pulse_us < PULSE_MIN_US) pulse_us = PULSE_MIN_US;
    if (pulse_us > PULSE_MAX_US) pulse_us = PULSE_MAX_US;
    
    // Use Adafruit library's writeMicroseconds method
    // This handles the conversion to 12-bit PWM values internally
    pwm.writeMicroseconds(servo_num, pulse_us);
}

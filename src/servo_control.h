/**
 * @file servo_control.h
 * @brief Servo control using PCA9685 PWM driver
 * 
 * Uses Adafruit PWM Servo Driver Library (required)
 * Supports single or dual servo configurations
 */

#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

class ServoControl {
public:
    /**
     * Constructor
     * @param i2c_address PCA9685 I2C address (typically 0x40)
     * @param num_servos Number of servos (1 or 2)
     */
    ServoControl(uint8_t i2c_address, uint8_t num_servos);
    
    /**
     * Initialize PCA9685 and I2C communication
     * @return true if successful, false on error
     */
    bool begin();
    
    /**
     * Set servo position by angle
     * @param servo_num Servo index (0 or 1)
     * @param angle_degrees Angle in degrees (-90 to +90, 0=center)
     */
    void setPosition(uint8_t servo_num, float angle_degrees);
    
    /**
     * Set servo to center position (0 degrees)
     * @param servo_num Servo index (0 or 1)
     */
    void setCenter(uint8_t servo_num);
    
    /**
     * Center all servos
     */
    void centerAll();
    
    /**
     * Disable servo (stop sending PWM signal)
     * @param servo_num Servo index (0 or 1)
     */
    void disable(uint8_t servo_num);
    
    /**
     * Disable all servos
     */
    void disableAll();

private:
    Adafruit_PWMServoDriver pwm;
    uint8_t numServos;
    
    /**
     * Convert angle in degrees to pulse width in microseconds
     * @param angle Angle in degrees (-90 to +90)
     * @return Pulse width in microseconds
     */
    uint16_t angleToPulseWidth(float angle);
    
    /**
     * Set servo using pulse width in microseconds
     * @param servo_num Servo index
     * @param pulse_us Pulse width in microseconds
     */
    void setPulseWidth(uint8_t servo_num, uint16_t pulse_us);
};

#endif // SERVO_CONTROL_H

---
description: SimpleFaceMe - Demo project using HLK-LD2450 radar sensor to control servos that track detected targets
---

# SimpleFaceMe

## Revision History
- 2026-05-13: Initial project setup

## Overview

SimpleFaceMe is a quick demo integrating the HLK-LD2450 24GHz radar sensor with servo motors to create target-tracking "heads". The system detects human targets using the radar sensor and commands servos to face the primary target.

**Two build configurations:**
- **Single servo build**: One MG996R servo for simple target tracking
- **Dual servo build**: Two servos controlling separate "heads" that coordinate to face the same target

## Hardware

- ESP32-S3 DevKit C1 (N16R8)
- HLK-LD2450 24GHz Radar Sensor
- PCA9685 PWM Servo Driver (I2C)
- MG996R servos (1 or 2 depending on build)
- USB to TTL adapter for LD2450 configuration

## Quick Start

### Build for Single Servo
```bash
pio run -e single_servo
pio run -e single_servo -t upload
```

### Build for Dual Servos
```bash
pio run -e dual_servo
pio run -e dual_servo -t upload
```

## Configuration

All configuration is done via `platformio.ini` build_flags. See [docs/architecture.md](docs/architecture.md) for geometry configuration details.

## Documentation

- [docs/architecture.md](docs/architecture.md) - System architecture and geometry
- [docs/implementation.md](docs/implementation.md) - Implementation plan

## References

- HLK-LD2450 Python implementation: `../HLK-LD2450/`
- Servo validation project: `../ConversingHeads/test/servo_validation/`

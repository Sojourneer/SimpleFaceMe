---
description: SimpleFaceMe - Demo project using HLK-LD2450 radar sensor to control servos that track detected targets
---

# SimpleFaceMe

## Revision History
- 2026-05-13: Initial project setup
- 2026-05-16: Added unified build with runtime mode selection via GPIO21

## Overview

SimpleFaceMe is a quick demo integrating the HLK-LD2450 24GHz radar sensor with servo motors to create target-tracking "heads". The system detects human targets using the radar sensor and commands servos to face the primary target.

**Three build configurations:**
- **`unified`**: Single firmware image — mode selected at boot via GPIO21 (recommended)
- **`single_servo`**: Compile-time single servo build (MG996R)
- **`dual_servo`**: Compile-time dual servo build (two 20Kg servos), for the ConversingHeads project, realizing parallax

## Hardware

- ESP32-S3 DevKit C1 (N16R8)
- HLK-LD2450 24GHz Radar Sensor
- PCA9685 PWM Servo Driver (I2C)
- MG996R servo or ConversingHeads 20kg servo pair
- Optional: USB to TTL adapter if using LD2450 configuration tool

## Quick Start

### Unified Build (Recommended)

One firmware image supports both configurations. Mode is selected at boot by the state of **GPIO21**:

| GPIO21 at power-on | Mode |
|--------------------|------|
| Floating (pullup)  | Single servo |
| Grounded           | Dual servo |

```bash
pio run -e unified -t upload
```

To switch modes, connect or disconnect a wire between GPIO21 and GND, then power-cycle. No reflashing needed.

### Compile-Time Builds

For a fixed configuration:

```bash
# Single servo (MG996R)
pio run -e single_servo -t upload

# Dual servos (20Kg pair)
pio run -e dual_servo -t upload
```

## Configuration

All configuration is done via `platformio.ini` build_flags. See [docs/architecture.md](docs/architecture.md) for geometry configuration details.

## Documentation

- [docs/architecture.md](docs/architecture.md) - System architecture and geometry
- [docs/implementation.md](docs/implementation.md) - Implementation plan


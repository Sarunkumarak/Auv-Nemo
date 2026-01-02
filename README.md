
# NEMO – AUV Control & Calibration System

## Overview
This repository contains the control and calibration system for **NEMO – Navigational Explorer for Marine Observation**, an Autonomous Underwater Vehicle (AUV) built using **ESP32-S3** and **NVIDIA Jetson Orin**.

The system supports **IMU calibration**, **8-thruster control**, **keyboard-based manual operation**, **ARM/DISARM safety**, and full **6 Degrees of Freedom (6-DOF)** motion using a **case-based control logic**.

---

## System Architecture
### ESP32-S3
- Low-level thruster control
- IMU data acquisition
- PWM generation for ESCs
- Real-time safety handling

### NVIDIA Jetson Orin
- High-level control logic
- Keyboard input handling
- 6-DOF motion mapping
- Serial communication with ESP32-S3

---

## Features

### IMU Calibration
- Accelerometer calibration
- Gyroscope bias correction
- Magnetometer calibration (optional)
- Calibration triggered via keyboard
- Calibration data stored for reuse

### Thruster Control (8 Thrusters)
- Independent control of all 8 thrusters
- Forward and reverse thrust
- Differential thrust for maneuvering
- Thruster mixing for stable motion

### ARM / DISARM Safety
- **ARM Mode**
  - Enables thrusters
  - Accepts motion commands
- **DISARM Mode**
  - Immediately stops all thrusters
  - Prevents accidental activation
- Default DISARM state on startup

### 6 Degrees of Freedom (6-DOF)
| DOF   | Motion |
|------|--------|
| Surge | Forward / Backward |
| Sway  | Left / Right |
| Heave | Up / Down |
| Roll  | X-axis rotation |
| Pitch | Y-axis rotation |
| Yaw   | Z-axis rotation |

---

## Keyboard Control Mapping
```

ARM            → A
DISARM         → D

Forward        → W
Backward       → S
Left           → Q
Right          → E
Up             → R
Down           → F

Yaw Left       → J
Yaw Right      → L
Pitch Up       → I
Pitch Down     → K
Roll Left      → U
Roll Right     → O

IMU Calibration → C
Emergency Stop → SPACE

```

---

## Control Logic (Case Method)
The system uses a **case-based switch logic** to map keyboard commands to motion actions.

### Control Flow
```

switch(command):
case ARM:
enable_thrusters()
case DISARM:
disable_thrusters()
case FORWARD:
apply_surge_positive()
case YAW_LEFT:
apply_yaw_negative()
case CALIBRATE_IMU:
run_imu_calibration()

```

This structure ensures:
- Clear command handling
- High safety
- Easy debugging
- Future expandability

---

## Communication
- Jetson Orin → ESP32-S3 via UART / USB Serial
- Command packets include:
  - System mode (ARM / DISARM)
  - Motion type
  - Thrust magnitude
- ESP32 validates commands before execution

---

## Safety Features
- Startup in DISARM mode
- Emergency stop support
- Thruster timeout protection
- Invalid command rejection

---

## Hardware Used
- ESP32-S3
- NVIDIA Jetson Orin
- 8 × Brushless Thrusters
- ESCs
- IMU (Accelerometer + Gyroscope)
- Power Distribution Board

---

## Future Enhancements
- PID control for depth and heading
- Sensor fusion (IMU + Depth sensor)
- Autonomous navigation modes
- ROS2 integration
- Mission scripting support

---







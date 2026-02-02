# RoboGenesis RC Car - ESP32 Project

## Overview
Intelligent RC car using ESP32 with L298N motor driver and ultrasonic obstacle avoidance system.

## Hardware Components
- ESP32 Development Board
- L298N Motor Driver
- DC Motors (x2)
- HC-SR04 Ultrasonic Sensor
- SG90 Servo Motor
- RC Car Chassis

## Pin Configuration

### L298N Motor Driver
- Left Motor Enable: GPIO 25
- Left Motor IN1: GPIO 26
- Left Motor IN2: GPIO 27
- Right Motor Enable: GPIO 32
- Right Motor IN1: GPIO 33
- Right Motor IN2: GPIO 14

### Ultrasonic Sensor & Servo
- Servo Pin: GPIO 4
- Ultrasonic Trig: GPIO 2
- Ultrasonic Echo: GPIO 15

## Features
- **Differential Drive Control**: Smooth turning with independent wheel speed control
- **Obstacle Avoidance**: Continuous scanning with intelligent path planning
- **Emergency Stop**: Immediate response to close obstacles
- **Modular Design**: Easy integration with future Bluetooth safety override
- **State Machine**: Robust behavior management

## Installation
1. Install PlatformIO IDE
2. Open project folder
3. Connect hardware according to pin configuration
4. Upload code using PlatformIO

## Usage
- Car will automatically start moving forward
- Continuously scans for obstacles
- Automatically avoids obstacles using intelligent path selection
- Monitor Serial Monitor (115200 baud) for status updates

## Future Integration
- Bluetooth safety override system
- Manual control mode
- Advanced path planning algorithms
- Camera integration

## Git Commit Messages (for Mandatory GitHub Task)

### Recommended Commit Sequence:

1. `feat: configure platformio for esp32 development`
2. `feat: implement modular motor control class for l298n driver`
3. `feat: add differential wheel speed control logic`
4. `feat: create ultrasonic sensor module with servo scanning`
5. `feat: implement obstacle detection and avoidance algorithms`
6. `feat: integrate state machine for autonomous navigation`
7. `feat: add bluetooth safety override integration point`
8. `chore: update gitignore for esp32 platformio project`
9. `docs: add comprehensive readme with pin configuration`
10. `refactor: optimize sensor scanning and motor control timing`

### Alternative Granular Commits:
- `feat: add motor control header with class definition`
- `feat: implement motor control constructor and initialization`
- `feat: add basic motor movement functions`
- `feat: implement differential drive control method`
- `feat: create ultrasonic sensor class structure`
- `feat: implement servo-controlled scanning functionality`
- `feat: add distance measurement and filtering`
- `feat: implement obstacle detection algorithms`
- `feat: create autonomous navigation state machine`
- `feat: integrate all modules in main application`

## Troubleshooting
- Ensure all pins are correctly connected
- Check power supply for motors (separate power recommended)
- Verify servo and ultrasonic sensor functionality
- Monitor Serial output for debugging information

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



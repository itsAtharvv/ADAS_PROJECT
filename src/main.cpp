#include <Arduino.h>
#include "motor_control.h"
#include "ultrasonic_sensor.h"

// Pin definitions for ESP32
// L298N Motor Driver Pins
const int LEFT_MOTOR_ENABLE = 25;
const int LEFT_MOTOR_IN1 = 26;
const int LEFT_MOTOR_IN2 = 27;
const int RIGHT_MOTOR_ENABLE = 32;
const int RIGHT_MOTOR_IN1 = 33;
const int RIGHT_MOTOR_IN2 = 14;

// Ultrasonic Sensor and Servo Pins
const int SERVO_PIN = 4;
const int ULTRASONIC_TRIG = 2;
const int ULTRASONIC_ECHO = 15;

// System configuration
const int DEFAULT_SPEED = 180;
const int TURN_SPEED = 120;
const unsigned long OBSTACLE_CHECK_INTERVAL = 100; // ms
const unsigned long SERIAL_PRINT_INTERVAL = 1000; // ms

// Global objects
MotorControl* motors;
UltrasonicSensor* sensor;

// System state
enum RobotState {
    IDLE,
    MOVING_FORWARD,
    TURNING_LEFT,
    TURNING_RIGHT,
    AVOIDING_OBSTACLE,
    EMERGENCY_STOP
};

RobotState currentState = IDLE;
unsigned long lastObstacleCheck = 0;
unsigned long lastSerialPrint = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=== RoboGenesis RC Car Initializing ===");
    
    // Initialize motor control
    motors = new MotorControl(LEFT_MOTOR_ENABLE, LEFT_MOTOR_IN1, LEFT_MOTOR_IN2,
                             RIGHT_MOTOR_ENABLE, RIGHT_MOTOR_IN1, RIGHT_MOTOR_IN2);
    motors->initialize();
    
    // Initialize ultrasonic sensor
    sensor = new UltrasonicSensor(SERVO_PIN, ULTRASONIC_TRIG, ULTRASONIC_ECHO);
    sensor->initialize();
    
    // Start scanning
    sensor->startScanning();
    
    // Allow sensors to stabilize
    delay(2000);
    
    Serial.println("=== System Ready ===");
    currentState = MOVING_FORWARD;
}

void loop() {
    unsigned long currentTime = millis();
    
    // Update ultrasonic scanning
    sensor->updateScanning();
    
    // Check for obstacles periodically
    if (currentTime - lastObstacleCheck >= OBSTACLE_CHECK_INTERVAL) {
        handleObstacleDetection();
        lastObstacleCheck = currentTime;
    }
    
    // Execute current state behavior
    executeStateBehavior();
    
    // Print status periodically
    if (currentTime - lastSerialPrint >= SERIAL_PRINT_INTERVAL) {
        printSystemStatus();
        lastSerialPrint = currentTime;
    }
    
    // Small delay to prevent overwhelming the system
    delay(10);
}

void handleObstacleDetection() {
    if (sensor->isDangerClose()) {
        // Immediate danger - emergency stop
        motors->emergencyStop();
        currentState = EMERGENCY_STOP;
        Serial.println("DANGER! Emergency stop activated!");
        
        // Perform full scan to find clear path
        sensor->performFullScan();
        
        // Find best direction to turn
        int bestAngle = findBestDirection();
        if (bestAngle < 90) {
            currentState = TURNING_LEFT;
            Serial.println("Turning left to avoid obstacle");
        } else if (bestAngle > 90) {
            currentState = TURNING_RIGHT;
            Serial.println("Turning right to avoid obstacle");
        } else {
            // No clear path, back up
            motors->moveBackward(DEFAULT_SPEED);
            delay(1000);
            motors->stop();
            currentState = IDLE;
        }
        
    } else if (sensor->isObstacleAhead()) {
        // Obstacle detected - slow down and prepare to turn
        motors->moveForward(DEFAULT_SPEED / 2);
        currentState = AVOIDING_OBSTACLE;
        
        // Find best turning direction
        int bestAngle = findBestDirection();
        if (bestAngle < 85) {
            currentState = TURNING_LEFT;
        } else if (bestAngle > 95) {
            currentState = TURNING_RIGHT;
        }
    }
}

int findBestDirection() {
    float maxDistance = 0;
    int bestAngle = 90;
    
    // Check all readings to find clearest path
    for (int i = 0; i < sensor->getReadingCount(); i++) {
        auto readings = sensor->getReadings();
        if (readings[i].distance > maxDistance) {
            maxDistance = readings[i].distance;
            bestAngle = readings[i].angle;
        }
    }
    
    return bestAngle;
}

void executeStateBehavior() {
    static unsigned long stateStartTime = millis();
    
    switch (currentState) {
        case IDLE:
            motors->stop();
            break;
            
        case MOVING_FORWARD:
            motors->moveForward(DEFAULT_SPEED);
            break;
            
        case TURNING_LEFT:
            motors->turnLeft(TURN_SPEED);
            // Turn for a limited time
            if (millis() - stateStartTime > 1500) {
                currentState = MOVING_FORWARD;
                stateStartTime = millis();
            }
            break;
            
        case TURNING_RIGHT:
            motors->turnRight(TURN_SPEED);
            // Turn for a limited time
            if (millis() - stateStartTime > 1500) {
                currentState = MOVING_FORWARD;
                stateStartTime = millis();
            }
            break;
            
        case AVOIDING_OBSTACLE:
            // State is handled in obstacle detection
            break;
            
        case EMERGENCY_STOP:
            // Wait for manual intervention or clear path
            if (!sensor->isDangerClose()) {
                currentState = IDLE;
                stateStartTime = millis();
            }
            break;
    }
}

void printSystemStatus() {
    Serial.println("=== System Status ===");
    Serial.printf("State: %d\n", currentState);
    Serial.printf("Closest obstacle: %.2f cm at %d°\n", 
                 sensor->getClosestObstacleDistance(), 
                 sensor->getClosestObstacleAngle());
    Serial.printf("Motors moving: %s\n", motors->isMoving() ? "Yes" : "No");
    Serial.printf("Scanning: %s\n", sensor->isCurrentlyScanning() ? "Yes" : "No");
    Serial.println("====================");
}

// Future: Bluetooth safety override integration point
void handleBluetoothOverride() {
    // This function will be implemented when Bluetooth module is added
    // Expected to receive commands like:
    // - STOP: Emergency stop
    // - PAUSE: Pause current operation
    // - RESUME: Resume normal operation
    // - MANUAL: Switch to manual control mode
}
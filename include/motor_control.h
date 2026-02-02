#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

class MotorControl {
private:
    // L298N Motor Driver Pins
    struct Motor {
        int enablePin;
        int in1Pin;
        int in2Pin;
        int currentSpeed;
    };

    Motor leftMotor;
    Motor rightMotor;

    // Motor configuration
    const int maxSpeed = 255;
    const int minSpeed = 0;

    void setMotorSpeed(Motor& motor, int speed, bool forward);
    void stopMotor(Motor& motor);

public:
    MotorControl(int leftEnable, int leftIn1, int leftIn2, 
                 int rightEnable, int rightIn1, int rightIn2);
    
    void initialize();
    
    // Basic movement functions
    void moveForward(int speed);
    void moveBackward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void stop();
    
    // Differential speed control for smooth turning
    void differentialDrive(int leftSpeed, int rightSpeed);
    
    // Speed control functions
    void setMaxSpeed(int speed);
    int getMaxSpeed() const { return maxSpeed; }
    
    // Safety functions
    void emergencyStop();
    bool isMoving() const;
};

#endif

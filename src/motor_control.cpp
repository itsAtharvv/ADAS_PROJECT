#include "motor_control.h"

MotorControl::MotorControl(int leftEnable, int leftIn1, int leftIn2, 
                          int rightEnable, int rightIn1, int rightIn2) {
    leftMotor = {leftEnable, leftIn1, leftIn2, 0};
    rightMotor = {rightEnable, rightIn1, rightIn2, 0};
}

void MotorControl::initialize() {
    // Configure all motor pins as outputs
    pinMode(leftMotor.enablePin, OUTPUT);
    pinMode(leftMotor.in1Pin, OUTPUT);
    pinMode(leftMotor.in2Pin, OUTPUT);
    pinMode(rightMotor.enablePin, OUTPUT);
    pinMode(rightMotor.in1Pin, OUTPUT);
    pinMode(rightMotor.in2Pin, OUTPUT);
    
    // Initialize motors to stopped state
    stop();
    
    Serial.println("Motor control initialized");
}

void MotorControl::setMotorSpeed(Motor& motor, int speed, bool forward) {
    // Constrain speed to valid range
    speed = constrain(speed, minSpeed, maxSpeed);
    motor.currentSpeed = speed;
    
    // Set direction
    if (forward) {
        digitalWrite(motor.in1Pin, HIGH);
        digitalWrite(motor.in2Pin, LOW);
    } else {
        digitalWrite(motor.in1Pin, LOW);
        digitalWrite(motor.in2Pin, HIGH);
    }
    
    // Set speed using PWM
    analogWrite(motor.enablePin, speed);
}

void MotorControl::stopMotor(Motor& motor) {
    digitalWrite(motor.in1Pin, LOW);
    digitalWrite(motor.in2Pin, LOW);
    analogWrite(motor.enablePin, 0);
    motor.currentSpeed = 0;
}

void MotorControl::moveForward(int speed) {
    speed = constrain(speed, minSpeed, maxSpeed);
    setMotorSpeed(leftMotor, speed, true);
    setMotorSpeed(rightMotor, speed, true);
}

void MotorControl::moveBackward(int speed) {
    speed = constrain(speed, minSpeed, maxSpeed);
    setMotorSpeed(leftMotor, speed, false);
    setMotorSpeed(rightMotor, speed, false);
}

void MotorControl::turnLeft(int speed) {
    speed = constrain(speed, minSpeed, maxSpeed);
    setMotorSpeed(leftMotor, speed/2, true);  // Slow down left motor
    setMotorSpeed(rightMotor, speed, true);   // Keep right motor at full speed
}

void MotorControl::turnRight(int speed) {
    speed = constrain(speed, minSpeed, maxSpeed);
    setMotorSpeed(leftMotor, speed, true);    // Keep left motor at full speed
    setMotorSpeed(rightMotor, speed/2, true); // Slow down right motor
}

void MotorControl::differentialDrive(int leftSpeed, int rightSpeed) {
    leftSpeed = constrain(leftSpeed, minSpeed, maxSpeed);
    rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);
    
    // Determine direction for each motor based on speed sign
    bool leftForward = leftSpeed >= 0;
    bool rightForward = rightSpeed >= 0;
    
    setMotorSpeed(leftMotor, abs(leftSpeed), leftForward);
    setMotorSpeed(rightMotor, abs(rightSpeed), rightForward);
}

void MotorControl::stop() {
    stopMotor(leftMotor);
    stopMotor(rightMotor);
}

void MotorControl::emergencyStop() {
    stop();
    Serial.println("EMERGENCY STOP ACTIVATED");
}

bool MotorControl::isMoving() const {
    return (leftMotor.currentSpeed > 0 || rightMotor.currentSpeed > 0);
}

#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <HCSR04.h>

class UltrasonicSensor {
private:
    // Servo for scanning
    Servo scanServo;
    int servoPin;
    
    // Ultrasonic sensor
    HCSR04* ultrasonic;
    int trigPin;
    int echoPin;
    
    // Scanning parameters
    const int minAngle = 15;
    const int maxAngle = 165;
    const int scanStep = 5;
    const int scanDelay = 50; // milliseconds between readings
    
    int currentAngle;
    bool isScanning;
    unsigned long lastScanTime;
    
    // Distance readings
    struct DistanceReading {
        int angle;
        float distance;
        unsigned long timestamp;
    };
    
    static const int maxReadings = 30;
    DistanceReading readings[maxReadings];
    int readingCount;
    
    // Safety parameters
    const float dangerThreshold = 20.0; // cm
    const float warningThreshold = 40.0; // cm
    
public:
    UltrasonicSensor(int servoPin, int trigPin, int echoPin);
    
    void initialize();
    
    // Basic distance measurement
    float getDistance();
    
    // Servo control
    void setAngle(int angle);
    int getCurrentAngle() const { return currentAngle; }
    
    // Scanning functions
    void startScanning();
    void stopScanning();
    void updateScanning();
    bool isCurrentlyScanning() const { return isScanning; }
    
    // Obstacle detection
    bool isObstacleAhead();
    bool isDangerClose();
    float getClosestObstacleDistance();
    int getClosestObstacleAngle();
    
    // Data access
    DistanceReading* getReadings() { return readings; }
    int getReadingCount() const { return readingCount; }
    void clearReadings();
    
    // Advanced scanning patterns
    void performFullScan();
    void performFrontScan();
    
    // Debug functions
    void printReadings();
    void printCurrentReading();
};

#endif

#include "ultrasonic_sensor.h"

UltrasonicSensor::UltrasonicSensor(int servoPin, int trigPin, int echoPin) 
    : servoPin(servoPin), trigPin(trigPin), echoPin(echoPin) {
    currentAngle = 90; // Start facing forward
    isScanning = false;
    lastScanTime = 0;
    readingCount = 0;
    ultrasonic = nullptr;
}

void UltrasonicSensor::initialize() {
    // Initialize servo
    scanServo.attach(servoPin);
    setAngle(currentAngle);
    
    // Initialize ultrasonic sensor
    ultrasonic = new HCSR04(trigPin, echoPin);
    
    // Allow components to settle
    delay(100);
    
    Serial.println("Ultrasonic sensor initialized");
}

float UltrasonicSensor::getDistance() {
    if (!ultrasonic) return -1.0;
    
    float distance = ultrasonic->dist();
    
    // Filter out invalid readings
    if (distance < 0 || distance > 400) { // Max range of HC-SR04
        return -1.0;
    }
    
    return distance;
}

void UltrasonicSensor::setAngle(int angle) {
    angle = constrain(angle, minAngle, maxAngle);
    currentAngle = angle;
    scanServo.write(angle);
    delay(15); // Allow servo to reach position
}

void UltrasonicSensor::startScanning() {
    isScanning = true;
    currentAngle = minAngle;
    setAngle(currentAngle);
    clearReadings();
    lastScanTime = millis();
    Serial.println("Started ultrasonic scanning");
}

void UltrasonicSensor::stopScanning() {
    isScanning = false;
    setAngle(90); // Return to forward position
    Serial.println("Stopped ultrasonic scanning");
}

void UltrasonicSensor::updateScanning() {
    if (!isScanning) return;
    
    unsigned long currentTime = millis();
    
    // Check if it's time for next reading
    if (currentTime - lastScanTime >= scanDelay) {
        float distance = getDistance();
        
        if (distance > 0) {
            // Store reading
            if (readingCount < maxReadings) {
                readings[readingCount].angle = currentAngle;
                readings[readingCount].distance = distance;
                readings[readingCount].timestamp = currentTime;
                readingCount++;
            } else {
                // Shift array and add new reading
                for (int i = 0; i < maxReadings - 1; i++) {
                    readings[i] = readings[i + 1];
                }
                readings[maxReadings - 1].angle = currentAngle;
                readings[maxReadings - 1].distance = distance;
                readings[maxReadings - 1].timestamp = currentTime;
            }
        }
        
        // Move to next angle
        currentAngle += scanStep;
        if (currentAngle > maxAngle) {
            currentAngle = minAngle; // Reset to start
        }
        
        setAngle(currentAngle);
        lastScanTime = currentTime;
    }
}

bool UltrasonicSensor::isObstacleAhead() {
    // Check angles 75-105 degrees (front area)
    for (int i = 0; i < readingCount; i++) {
        if (readings[i].angle >= 75 && readings[i].angle <= 105) {
            if (readings[i].distance > 0 && readings[i].distance < warningThreshold) {
                return true;
            }
        }
    }
    return false;
}

bool UltrasonicSensor::isDangerClose() {
    // Check for immediate danger in front
    for (int i = 0; i < readingCount; i++) {
        if (readings[i].angle >= 85 && readings[i].angle <= 95) {
            if (readings[i].distance > 0 && readings[i].distance < dangerThreshold) {
                return true;
            }
        }
    }
    return false;
}

float UltrasonicSensor::getClosestObstacleDistance() {
    float closestDistance = 999.0;
    
    for (int i = 0; i < readingCount; i++) {
        if (readings[i].distance > 0 && readings[i].distance < closestDistance) {
            closestDistance = readings[i].distance;
        }
    }
    
    return (closestDistance == 999.0) ? -1.0 : closestDistance;
}

int UltrasonicSensor::getClosestObstacleAngle() {
    float closestDistance = 999.0;
    int closestAngle = -1;
    
    for (int i = 0; i < readingCount; i++) {
        if (readings[i].distance > 0 && readings[i].distance < closestDistance) {
            closestDistance = readings[i].distance;
            closestAngle = readings[i].angle;
        }
    }
    
    return closestAngle;
}

void UltrasonicSensor::clearReadings() {
    readingCount = 0;
}

void UltrasonicSensor::performFullScan() {
    Serial.println("Performing full scan...");
    clearReadings();
    
    for (int angle = minAngle; angle <= maxAngle; angle += scanStep) {
        setAngle(angle);
        delay(scanDelay);
        float distance = getDistance();
        
        if (distance > 0 && readingCount < maxReadings) {
            readings[readingCount].angle = angle;
            readings[readingCount].distance = distance;
            readings[readingCount].timestamp = millis();
            readingCount++;
        }
    }
    
    setAngle(90); // Return to forward
    Serial.println("Full scan completed");
}

void UltrasonicSensor::performFrontScan() {
    Serial.println("Performing front scan...");
    clearReadings();
    
    for (int angle = 60; angle <= 120; angle += scanStep) {
        setAngle(angle);
        delay(scanDelay);
        float distance = getDistance();
        
        if (distance > 0 && readingCount < maxReadings) {
            readings[readingCount].angle = angle;
            readings[readingCount].distance = distance;
            readings[readingCount].timestamp = millis();
            readingCount++;
        }
    }
    
    setAngle(90); // Return to forward
    Serial.println("Front scan completed");
}

void UltrasonicSensor::printReadings() {
    Serial.println("=== Distance Readings ===");
    for (int i = 0; i < readingCount; i++) {
        Serial.printf("Angle: %d°, Distance: %.2f cm\n", 
                     readings[i].angle, readings[i].distance);
    }
    Serial.println("========================");
}

void UltrasonicSensor::printCurrentReading() {
    float distance = getDistance();
    Serial.printf("Current reading - Angle: %d°, Distance: %.2f cm\n", 
                 currentAngle, distance);
}

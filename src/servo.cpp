#include <ESP32Servo.h>

int servoPin = 18;

Servo myservo;

void setup_servo() {
  myservo.attach(servoPin);
}
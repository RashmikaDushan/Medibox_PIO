#include <ESP32Servo.h>

extern Servo myservo;

void setup_servo();

void servo_control(int min_angle, float control_fac);
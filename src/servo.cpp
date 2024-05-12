#include <servo.h>
#include <globals.h>
#include <definitions.h>

int servoPin = servo_pin;
int angle = 0;

Servo myservo;

// Servo setup
void setup_servo()
{
  myservo.attach(servoPin);
}

// Servo control function
void servo_control(int min_angle, float control_fac)
{

  // Calculate the servo angle
  angle = (180 - min_angle) * light_intensity * control_fac;

  if (max_ldr_left)
  {
    angle += (min_angle * 1.5);
  }
  else
  {
    angle += (min_angle * 0.5);
  }

 // Check if the angle is within 0<=angle<=180
  if (angle > 180)
  {
    angle = 180;
  }

  myservo.write(angle); // Write the angle to the servo
  delay(15); // Delay for the servo to reach the position
}
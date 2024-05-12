#include <servo.h>
#include <globals.h>

int servoPin = 19;
int angle = 0;

Servo myservo;

void setup_servo()
{
  myservo.attach(servoPin);
}

void servo_control(int min_angle, float control_fac)
{

  angle = (180 - min_angle) * light_intensity * control_fac;

  if (max_ldr_left)
  {
    angle += (min_angle * 1.5);
  }
  else
  {
    angle += (min_angle * 0.5);
  }

  if (angle > 180)
  {
    angle = 180;
  }
  Serial.print("angle: ");
  Serial.print(angle);
    Serial.print(" min angle: ");
  Serial.print(min_angle);
    Serial.print(" fac: ");
  Serial.println(control_fac);
  myservo.write(angle);
}
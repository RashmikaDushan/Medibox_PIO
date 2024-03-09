#include <functions.h>
#include <definitions.h>
#include <globals.h>

// Variables
// Notes for the buzzer
int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

// ring an alarm
void ring_alarm()
{
  // light up both LEDs
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);

  // ring the buzzer
  while (digitalRead(CANCEL) == HIGH)
  {
    for (int i = 0; i < n_notes; i++)
    {
      if (digitalRead(CANCEL) == LOW)
      {
        delay(200);
        break;
      }
      tone(BUZZER, notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
    }
  }
  delay(200);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}

#include <functions.h>
#include <definitions.h>
#include <globals.h>
#include <icons.h>

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

bool muted = false; // Mute temprature and humidity level alarm

// Alarm variables
// Enable/Disable all alarms
bool alarm_enabled = false;
// Number of alarms
int n_alarms = 3;
// Alarm hours
int alarm_hours[] = {0, 0, 0};
// Alarm minutes
int alarm_minutes[] = {1, 10, 0};
// Alarm triggered
bool alarm_triggered[] = {false, false, false};
bool alarms_enabled[] = {false, false, false};

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
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  delay(200);
}

// alarm when the temp and the humidity are not in the range
void alarm_temp_humidity(int n, int note)
{
  // ring the buzzer
  if (digitalRead(CANCEL) == LOW) // if not in the menu
  {
    muted = !muted;
    delay(200);
    return;
  }
  if (!muted)
  {
    for (int i = 0; i < n; i++)
    {
      tone(BUZZER, notes[note]);
      delay(100);
      noTone(BUZZER);
      delay(100);
    }
  }
}

void check_alarms()
{
  // check for alarms
  if (alarm_enabled)
  {
    // iterating through all alarms
    for (int i = 0; i < n_alarms; i++)
    {
      if (alarm_triggered[i] == false && alarms_enabled[i] == true && rtc.getHour(true) == alarm_hours[i] && rtc.getMinute() == alarm_minutes[i])
      {
        // Show message on display
        display.clearDisplay();
        print_line("Alarm " + String(i), 1, 0, 0, true);
        print_line("Medicine Time", 2, 11, 0, true);
        ring_alarm(); // call the ringing function
        alarm_triggered[i] = true;
      }
    }
  }
}

// function for setting an alarm
void set_alarm(int alarm)
{
    bool go_to_next = true;
  int temp_hour = alarm_hours[alarm];
  while (true)
  {
    display.clearDisplay();
    print_line("Enter hour: ", 1, 10, 30, false);
    print_line(String(temp_hour), 2, 35, 60, false);
    display.display();

    int pressed = button_press();
    if (pressed == UP)
    {
      temp_hour += 1;
      temp_hour = temp_hour % 24;
    }

    else if (pressed == DOWN)
    {
      temp_hour -= 1;
      if (temp_hour < 0)
      {
        temp_hour = 23;
      }
    }

    else if (pressed == OK)
    {
      break;
    }

    else if (pressed == CANCEL)
    {
      go_to_next = false;
      display.clearDisplay();
      print_line("   Alarm is not set", 1, 30, 0, true);
      delay(2000);
      play_animaton(1, cross, 2, 32, 48, 16);
      break;
    }
  }

  int temp_minute = alarm_minutes[alarm];
  while (go_to_next)
  {
    display.clearDisplay();
    print_line("Enter minute: ", 1, 10, 25, false);
    print_line(String(temp_minute), 2, 35, 60, false);
    display.display();

    int pressed = button_press();
    if (pressed == UP)
    {
      temp_minute += 1;
      temp_minute = temp_minute % 60;
    }

    else if (pressed == DOWN)
    {
      temp_minute -= 1;
      if (temp_minute < 0)
      {
        temp_minute = 59;
      }
    }

    else if (pressed == OK)
    {
      alarm_hours[alarm] = temp_hour;
      alarm_minutes[alarm] = temp_minute;
      alarm_enabled = true;
      alarms_enabled[alarm] = true;
      alarm_triggered[alarm] = false;
      display.clearDisplay();
      print_line("     Alarm is set", 1, 30, 0, true);
      delay(2000);
      play_animaton(1, tick, 2, 32, 48, 16);
      break;
    }

    else if (pressed == CANCEL)
    {
      display.clearDisplay();
      print_line("   Alarm is not set", 1, 30, 0, true);
      delay(2000);
      play_animaton(1, cross, 2, 32, 48, 16);
      break;
    }
  }
  go_to_next = true;
}

void disable_all_alarms()
{
  display.clearDisplay();
  print_line(" All alarms disabled", 1, 30, 0, true);
  alarm_enabled = false;
  for (int i = 0; i < n_alarms; i++)
  {
    alarms_enabled[i] = false;
  }
  delay(2000);
  play_animaton(1, tick, 2, 32, 48, 16);
};

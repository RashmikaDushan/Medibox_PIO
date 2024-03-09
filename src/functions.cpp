#include <WiFi.h>
#include <time.h>

#include <functions.h>
#include <globals.h>
#include <definitions.h>

// Time variables for the RTC
int year = 0;
String month = " ";
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

bool muted = false; // Mute temprature and humidity level alarm

int UTC_OFFSET = 19800; // UTC offset for NTC time server - +5.30 for Sri Lanka

// Month names
String months[] = {"january",
                   "february",
                   "march",
                   "april", "may",
                   "june",
                   "july",
                   "august",
                   "september",
                   "october",
                   "november",
                   "december"};

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
bool alarm_triggered[] = {false, false, false, false};

// For RTC clock
unsigned long timeNow = 0;
unsigned long timelast = 0;

// Main menu option number
int current_mode = 0;
// Number of main menu options
int max_modes = 6;
// Main menu options
String options[] = {"1 - Set Time",
                    "2 - Set Alarm 1",
                    "3 - Set Alarm 2",
                    "4 - Set Alarm 3",
                    "5 - Remove Alarm",
                    "6 - Set Timezone"};

// object Declarations
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display
DHTesp dhtSensor; // Declaration for a DHT sensor

// function to automatically update the current time over wifi
void update_time_wifi(void)
{
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char year_str[8];
  char month_str[8];
  char day_str[8];
  char hour_str[8];
  char min_str[8];
  char sec_str[8];

  strftime(year_str, 8, "%Y", &timeinfo);
  strftime(month_str, 8, "%B", &timeinfo);
  strftime(day_str, 8, "%d", &timeinfo);
  strftime(sec_str, 8, "%S", &timeinfo);
  strftime(hour_str, 8, "%H", &timeinfo);
  strftime(min_str, 8, "%M", &timeinfo);

  year = atoi(year_str);
  month = month_str;
  hours = atoi(hour_str);
  minutes = atoi(min_str);
  days = atoi(day_str);
  seconds = atoi(sec_str);
  display.clearDisplay();
  print_line("Time updated online!", 1, 0, 0,1);
  delay(2000);
  display.clearDisplay();
}

// function to automatically update the current time on device
void update_time(void)
{
  timeNow = millis() / 1000;    // number of seconds passed since
  seconds = timeNow - timelast; // number of seconds passed fr

  // if a minute has passed
  if (seconds >= 60)
  {
    timelast += 60;
    minutes += 1;
  }

  // if an hour has passed
  if (minutes == 60)
  {
    minutes = 0;
    hours += 1;
  }

  // if a day has passed
  if (hours == 24)
  {
    hours = 0;
    days += 1;
    update_time_wifi();

    // enable the alarms again
    for (int i = 0; i < n_alarms; i++)
    {
      alarm_triggered[i] = false;
    }
  }
}

// alarm when the temp and the humidity are not in the range
void alarm_temp_humidity(int n, int note)
{
  // ring the buzzer
  if (digitalRead(CANCEL) == LOW) // if not in the menu
  {
    delay(200);
    muted = !muted;
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

// function to display the current time DD:HH:MM:SS
void print_time_now(void)
{
  display.clearDisplay();
  print_line(String(days) + " " + month + " " + String(year), 1, 0, 0,0);
  print_line(String(hours), 2, 16, 0,0);
  print_line(":", 2, 16, 20,0);
  print_line(String(minutes), 2, 16, 30,0);
  print_line(":", 2, 16, 50,0);
  print_line(String(seconds), 2, 16, 60,0);
  display.display();
}

// function to automatically update the current time while checking for alarms
void update_time_with_check_alarm()
{
  // update time
  update_time();
  print_time_now();

  // check for alarms
  if (alarm_enabled)
  {
    // iterating through all alarms
    for (int i = 0; i < n_alarms; i++)
    {
      if (alarm_triggered[i] == false && hours == alarm_hours[i] && minutes == alarm_minutes[i])
      {
        // Show message on display
        display.clearDisplay();
        print_line("Alarm " + String(i), 1, 0, 0,1);
        print_line("Medicine Time", 1, 11, 0,1);
        Serial.println("Alarm is ringing");
        ring_alarm(); // call the ringing function
        alarm_triggered[i] = true;
      }
    }
  }
}

// function for setting the time zone
void set_time_zone(bool start)
{
  int minute_offset = 0;
  while (true)
  {
    display.clearDisplay();
    print_line("Enter time zone: ", 0, 0, 1,0);
    if (minute_offset < 0)
    {
      print_line("-" + String(abs(minute_offset / 60)) + " : " + String(abs(minute_offset) % 60), 1, 11, 0,0);
    }
    else
    {
      print_line(String(abs(minute_offset / 60)) + " : " + String(abs(minute_offset) % 60), 1, 11, 0,0);
    }
    int pressed = button_press();
    if (pressed == UP)
    {
      if (minute_offset < 720)
      {
        minute_offset += 30;
      }
    }
    else if (pressed == DOWN)
    {
      if (minute_offset > -720)
      {
        minute_offset -= 30;
      }
    }
    else if (pressed == OK)
    {
      UTC_OFFSET = minute_offset * 60;
      Serial.println(UTC_OFFSET);
      display.clearDisplay();
      print_line("Time zone set.", 1, 0, 0,1);
      delay(1000);
      update_time_wifi();
      return;
    }
    else if (pressed == CANCEL)
    {
      display.clearDisplay();
      if(start){
        print_line("Default timezone set.", 1, 0, 0,1);
        print_line("UTC+5.30", 1, 11, 0,1);
        delay(1000);
        update_time_wifi();
        return;
      }
      else{
        print_line("Timezone not set.", 1, 0, 0,1);
        delay(1000);
        return;
      }
      
    }
    display.display();
  }
}

// function for setting time
void set_time()
{
  int temp_hour = hours;
  while (true)
  {
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour), 1, 0, 0,1);

    int pressed = button_press();
    if (pressed == UP)
    {
      delay(200);
      temp_hour += 1;
      temp_hour = temp_hour % 24;
    }

    else if (pressed == DOWN)
    {
      delay(200);
      temp_hour -= 1;
      if (temp_hour < 0)
      {
        temp_hour = 23;
      }
    }
    else if (pressed == OK)
    {
      delay(200);
      hours = temp_hour;
      break;
    }
    else if (pressed == CANCEL)
    {
      delay(200);
      break;
    }
  }

  int temp_minute = minutes;
  while (true)
  {
    display.clearDisplay();
    print_line("Enter minute: " + String(temp_minute), 1, 0, 0,1);

    int pressed = button_press();
    if (pressed == UP)
    {
      delay(200);
      temp_minute += 1;
      temp_minute = temp_minute % 60;
    }

    else if (pressed == DOWN)
    {
      delay(200);
      temp_minute -= 1;
      if (temp_minute < 0)
      {
        temp_minute = 59;
      }
    }

    else if (pressed == OK)
    {
      delay(200);
      minutes = temp_minute;
      break;
    }

    else if (pressed == CANCEL)
    {
      delay(200);
      break;
    }
  }

  display.clearDisplay();
  print_line("Time is set", 1, 0, 0,1);
  delay(1000);
}

// function for setting an alarm
void set_alarm(int alarm)
{
  int temp_hour = alarm_hours[alarm];
  while (true)
  {
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour), 0, 0, 2,1);

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
      alarm_hours[alarm] = temp_hour;
      break;
    }

    else if (pressed == CANCEL)
    {
      break;
    }
  }

  int temp_minute = alarm_minutes[alarm];
  while (true)
  {
    display.clearDisplay();
    print_line("Enter minute: " + String(temp_minute), 0, 0, 2,1);

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
      alarm_minutes[alarm] = temp_minute;
      alarm_enabled = true;
      break;
    }

    else if (pressed == CANCEL)
    {
      break;
    }
  }
  display.clearDisplay();
  print_line("Alarm is set", 0, 0, 2,1);
  delay(1000);
}

// to toggle between items in the menu
void run_mode(int mode)
{
  if (mode == 0)
  {
    set_time();
  }
  else if (mode == 1 || mode == 2 || mode == 3)
  {
    set_alarm(mode - 1);
  }

  else if (mode == 4)
  {
    alarm_enabled = false;
  }
  else if (mode == 5)
  {
    set_time_zone(0);
  }
}

// function to navigate through the menu
void go_to_menu(void)
{
  while (true)
  {
    display.clearDisplay();
    print_line(options[current_mode], 1, 0, 0,1);
    int pressed = button_press();
    if (pressed == UP)
    {
      current_mode = (current_mode + 1) % max_modes;
    }
    else if (pressed == DOWN)
    {
      current_mode = (current_mode + 5) % max_modes;
    }
    else if (pressed == OK)
    {
      Serial.println(current_mode);
      run_mode(current_mode);
    }
    else if (pressed == CANCEL)
    {
      return;
    }
  }
}

// check and print the humidity level and the temprature
void check_temp_humd(void)
{
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  bool all_good = true;
  if (data.temperature > 35)
  {
    all_good = false;
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    print_line("TEMP HIGH", 1, 45, 0,0);
    alarm_temp_humidity(2, 7);
  }
  else if (data.temperature < 25)
  {
    all_good = false;
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    print_line("TEMP LOW", 1, 45, 0,0);
    alarm_temp_humidity(2, 0);
  }
  else
  {
    print_line("Temprature is:" + String(data.temperature) + "C", 1, 45, 0,0);
  }
  if (data.humidity > 85)
  {
    all_good = false;
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    print_line("HUMD HIGH", 1, 56, 0,0);
    alarm_temp_humidity(3, 7);
  }
  else if (data.humidity < 35)
  {
    all_good = false;
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    print_line("HUMD LOW", 1, 56, 0,0);
    alarm_temp_humidity(3, 0);
  }
  else
  {
    print_line("Humidity is:" + String(data.humidity) + "%", 1, 56, 0,0);
  }
  display.display();
  if (all_good)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}

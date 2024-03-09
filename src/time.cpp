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

int UTC_OFFSET = 19800; // UTC offset for NTC time server - +5.30 for Sri Lanka

// For RTC clock
unsigned long timeNow = 0;
unsigned long timelast = 0;

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

// function to display the current time DD:HH:MM:SS
void print_time_now(void)
{
  display.clearDisplay();
  print_line(String(days) + " " + month + " " + String(year), 1, 0, 0 , false);
  print_line(String(hours), 2, 16, 0 , false);
  print_line(":", 2, 16, 20,0);
  print_line(String(minutes), 2, 16, 30 , false);
  print_line(":", 2, 16, 50,0);
  print_line(String(seconds), 2, 16, 60 , false);
}

// function to automatically update the current time while checking for alarms
void update_time_temp_humd_alarm()
{
  // update time
  update_time();
  print_time_now();
  check_temp_humd();
  display.display();
  check_alarms();
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


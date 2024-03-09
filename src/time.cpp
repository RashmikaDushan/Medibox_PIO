#include <time.h>

#include <functions.h>
#include <globals.h>
#include <definitions.h>
#include <icons.h>

// Time variables for the RTC
int year = 0;
int month = 0;
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
struct tm timeinfo;

char year_str[8];
char month_str[8];
char day_str[8];
char hour_str[8];
char min_str[8];
char sec_str[8];

int UTC_OFFSET_RTC = 19800; // UTC offset for RTC clock - +5.30 for Sri Lanka
int UTC_OFFSET_NTC = 0;     // UTC offset for NTC time server
ESP32Time rtc(UTC_OFFSET_RTC);

// For RTC clock
unsigned long timeNow = 0;
unsigned long timelast = 0;

// function to automatically update the current time over wifi
void update_time_wifi(void)
{
  configTime(UTC_OFFSET_NTC, UTC_OFFSET_DST, NTP_SERVER);
  getLocalTime(&timeinfo);

  strftime(year_str, 8, "%Y", &timeinfo);
  strftime(month_str, 8, "%m", &timeinfo);
  strftime(day_str, 8, "%d", &timeinfo);
  strftime(sec_str, 8, "%S", &timeinfo);
  strftime(hour_str, 8, "%H", &timeinfo);
  strftime(min_str, 8, "%M", &timeinfo);

  year = atoi(year_str);
  month = atoi(month_str);
  hours = atoi(hour_str);
  minutes = atoi(min_str);
  days = atoi(day_str);
  seconds = atoi(sec_str);
  rtc.setTime(seconds, minutes, hours, days, month, year);
  display.clearDisplay();
  print_line(" Time updated online!", 1, 30, 0, true);
  delay(2000);
  play_animaton(1, cloud, 2, 32, 48, 16);
  display.clearDisplay();
}

// function to display the current time DD:HH:MM:SS
void print_time_now(void)
{
  if (days = !rtc.getDay())
  {
    // enable the alarms again
    for (int i = 0; i < n_alarms; i++)
    {
      alarm_triggered[i] = false;
    }
    days = rtc.getDay();
  }
  display.clearDisplay();
  print_line(rtc.getDate(), 1, 0, 16, false);
  print_line(rtc.getTime(), 2, 16, 16, false);
}

// function to automatically update the current time while checking for alarms
void update_time_temp_humd_alarm()
{
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
    print_line("Enter time zone: ", 1, 10, 20, false);
    if (minute_offset < 0)
    {
      print_line("-", 2, 35, 10, false);
      print_line(String(abs(minute_offset / 60)), 2, 35, 35, false);
      print_line("  : " + String(abs(minute_offset) % 60), 2, 35, 35, false);
    }
    else
    {
      print_line(String(abs(minute_offset / 60)), 2, 35, 35, false);
      print_line("  : " + String(abs(minute_offset) % 60), 2, 35, 35, false);
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
      UTC_OFFSET_RTC = minute_offset * 60;
      rtc.offset = UTC_OFFSET_RTC;
      Serial.println(UTC_OFFSET_RTC);
      display.clearDisplay();
      print_line("    Time zone set.", 1, 30, 0, true);
      delay(2000);
      play_animaton(1, tick, 2, 32, 48, 16);
      update_time_wifi();
      return;
    }
    else if (pressed == CANCEL)
    {
      display.clearDisplay();
      if (start)
      {
        print_line("Default timezone set.", 1, 25, 0, true);
        print_line("      UTC+5.30", 1, 36, 0, true);
        rtc.offset = UTC_OFFSET_RTC;
        delay(2000);
        play_animaton(1, tick, 2, 32, 48, 16);
        update_time_wifi();
        return;
      }
      else
      {
        print_line("  Timezone not set.", 1, 25, 0, true);
        delay(2000);
        play_animaton(1, cross, 2, 32, 48, 16);
        return;
      }
    }
    display.display();
  }
}

// function for setting time
void set_time()
{
  bool go_to_next = true;
  int temp_hour = 0;
  timeinfo = rtc.getTimeStruct();
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
      print_line("   Time is not set", 1, 30, 0, true);
      delay(2000);
      play_animaton(1, cross, 2, 32, 48, 16);
      break;
    }
  }

  int temp_minute = 0;
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
      timeinfo.tm_min = temp_minute;
      timeinfo.tm_hour = temp_hour;
      timeinfo = convertTimezone(timeinfo, UTC_OFFSET_RTC);
      display.clearDisplay();
      print_line("     Time is set", 1, 30, 0, true);
      delay(2000);
      play_animaton(1, tick, 2, 32, 48, 16);
      break;
    }

    else if (pressed == CANCEL)
    {
      display.clearDisplay();
      print_line("   Time is not set", 1, 30, 0, true);
      delay(2000);
      play_animaton(1, cross, 2, 32, 48, 16);
      break;
    }
  }
  rtc.setTimeStruct(timeinfo);
  go_to_next = true;
}

struct tm convertTimezone(tm &source_time, int offset_seconds)
{
  // Convert source time to seconds since epoch
  time_t source_time_t = mktime(&source_time);

  // Add the offset to get the target time in seconds since epoch
  time_t target_time_t = source_time_t + offset_seconds - 3600 * 11;

  tm target_time;
  gmtime_r(&target_time_t, &target_time);
  if (target_time.tm_isdst != 0)
  {
    target_time_t -= 3600;
    gmtime_r(&target_time_t, &target_time);
  }

  return target_time;
}

#include <Wire.h>


// functions in time.cpp
void update_time_wifi(void); // Fetch time from the internet
void print_time_now(void); // Print the time
void update_time_temp_humd_alarm(); // Update time, temperature, humidity and check alarms every loop
void set_time_zone(bool start); // Set the time zone
void set_time(); // Set the time
struct tm convertTimezone(tm &source_time, int offset_seconds); // Convert the time from one zone to another
void print_time(tm timeinfo); // Print the time

// functions in temphumd.cpp
void check_temp_humd(void); // Check the temperature and humidity and ring an alarm if needed

// functions in menu.cpp
void run_mode(int mode); // Run the mode in the menu
void go_to_menu(void); // Go to the menu

// functions in inout.cpp
void print_line(String text, int text_size, int row, int column, bool printImmediatly);
int button_press(); // Get the button press

// functions in alarm.cpp
void disable_all_alarms(); // Disable all alarms
void ring_alarm(); // Ring the set alarm
void check_alarms(); // Check for alarms
void alarm_temp_humidity(int n, int note); // Alarm for temperature and humidity
void set_alarm(int alarm); // Set the alarm
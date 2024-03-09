#include <Wire.h>

void print_line(String text, int text_size, int row, int column, bool printImmediatly);

void update_time_wifi(void);
void alarm_temp_humidity(int n, int note);
void print_time_now(void);
void ring_alarm();
void check_alarms();
void update_time_temp_humd_alarm();
int button_press();
void set_time_zone(bool start);
void set_time();
void set_alarm(int alarm);
void run_mode(int mode);
void go_to_menu(void);
void check_temp_humd(void);
void print_time(tm timeinfo);
struct tm convertTimezone(tm &source_time, int offset_seconds);

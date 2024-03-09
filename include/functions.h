#include <Wire.h>
#include <globals.h>
#include <definitions.h>

void print_line(String text, int text_size, int row, int column,bool printImmediatly);

void update_time_wifi(void);
void update_time(void);
void alarm_temp_humidity(int n, int note);
void print_time_now(void);
void ring_alarm();
void update_time_with_check_alarm();
int button_press();
void set_time_zone(bool start);
void set_time();
void set_alarm(int alarm);
void run_mode(int mode);
void go_to_menu(void);
void check_temp_humd(void);

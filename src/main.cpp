// Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <time.h>

// Defnitions
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET_DST 0

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin# (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0X3C ///< See datasheet for Address; 0X3D for 128x64,

#define BUZZER 18

#define LED_GREEN 15
#define LED_RED 2

#define CANCEL 32
#define UP 33
#define DOWN 35
#define OK 34

#define DHT 12

// object Declarations
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

// Variables
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

String SSID = "SLT-Fiber-4B2B";
String PASSWORD = "msii6267963";

int year = 0;
String month = " ";
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

bool muted = false; // mute temprature and humidity level alarm

// UTC offset for NTC time server
int UTC_OFFSET = 19800;

String months[] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};

bool alarm_enabled = false;
int n_alarms = 3;
int alarm_hours[] = {0, 0, 0};
int alarm_minutes[] = {1, 10, 0};

bool alarm_triggered[] = {false, false, false, false};

unsigned long timeNow = 0;
unsigned long timelast = 0;

int current_mode = 0;
int max_modes = 6;
String options[] = {"1 - Set Time", "2 - Set Alarm 1", "3 - Set Alarm 2", "4 - Set Alarm 3", "5 - Remove Alarm", "6 - Set Timezone"};

// function to print a line of text in a given text size and the given position
void print_line(String text, int text_size, int row, int column)
{
  display.setTextSize(text_size);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(column, row);      // Start at (row, column)
  display.println(text);

  // display.display(); // delay (2000);
}

// function for setting the time zone
void set_time_zone()
{
  int minute_offset = 0;
  while (true)
  {
    display.clearDisplay();
    print_line("Enter time zone: ", 1, 0, 0);
    print_line("Enter time zone: ", 1, 22, 0);
    if (minute_offset < 0)
    {
      print_line("-" + String(abs(minute_offset / 60)) + " : " + String(abs(minute_offset) % 60), 1, 11, 0);
    }
    else
    {
      print_line(String(abs(minute_offset / 60)) + " : " + String(abs(minute_offset) % 60), 1, 11, 0);
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
      print_line("Time zone set.", 1, 0, 0);
      update_time_wifi();
      return;
    }
    else if (pressed == CANCEL)
    {
      display.clearDisplay();
      print_line("Default timezone set.", 1, 0, 0);
      print_line("UTC+5.30", 1, 11, 0);
      update_time_wifi();
      return;
    }
    display.display();
  }
}


void setup()
{
  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(CANCEL, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(OK, INPUT);

  dhtSensor.setup(DHT, DHTesp::DHT22);

  // put your setup code here, to run once:
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  print_line("Welcome to Medibox", 1, 0, 0); // (String, text_size, cursor_row, cursor_column)
  delay(3000);

  WiFi.begin(SSID, PASSWORD, 6);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    display.clearDisplay();
    print_line("Connecting to Wifi   "+SSID, 1, 0, 0); // (String, text_size, cursor_row, cursor_column)
  }
  delay(2000);
  display.clearDisplay();
  print_line("Connected to Wifi!!!", 1, 0, 0);
  delay(2000);

  set_time_zone(); // ask to enter the time zone
}

void loop()
{
  delay(400);
}

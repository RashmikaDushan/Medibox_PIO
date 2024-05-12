#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <ESP32Time.h>

#ifndef GLOBALS_H
#define GLOBALS_H

// object Declarations
extern Adafruit_SSD1306 display; // Declaration for an SSD1306 display
extern DHTesp dhtSensor;         // Declaration for a DHT sensor

// Variables
// Notes for the buzzer
extern int n_notes;
extern int notes[];

// Time variables for the RTC
extern int year;
extern int month;
extern int days;
extern int hours;
extern int minutes;
extern int seconds;
extern struct tm timeinfo;

extern bool muted; // Mute temprature and humidity level alarm

extern int UTC_OFFSET; // UTC offset for NTC time server - +5.30 for Sri Lanka

// Month names
extern String months[];

// Alarm variables
// Enable/Disable all alarms
extern bool alarm_enabled;
extern bool alarms_enabled[];
// Number of alarms
extern int n_alarms;
// Alarm hours
extern int alarm_hours[];
// Alarm minutes
extern int alarm_minutes[];
// Alarm triggered
extern bool alarm_triggered[];

// For RTC clock
extern unsigned long timeNow;
extern unsigned long timelast;

// Main menu option number
extern int current_mode;
// Number of main menu options
extern int max_modes;
// Main menu options
extern String options[];

extern ESP32Time rtc;

extern int min_angle;
extern float control_fac;
extern bool max_ldr_left;
extern float light_intensity;
extern int angle;
extern int servoPin;
extern int medicine;

extern bool on_off;

#endif // GLOBALS_H
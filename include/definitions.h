// Defnitions
#define NTP_SERVER "pool.ntp.org" // NTP server
#define UTC_OFFSET_DST 0          // UTC daytime offset = 0 for no daytime offset

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin# (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0X3C ///< See datasheet for Address; 0X3D for 128x64,

#define BUZZER 18 // Buzzer pin

#define servo_pin 19 // Servo pin

#define LED_GREEN 15 // Green LED pin
#define LED_RED 2    // RED LED pin

#define CANCEL 33 // Cancel button
#define UP 26     // Up button
#define DOWN 25   // Down button
#define OK 32     // OK button

#define DHT 27 // DHT sensor pin

#define LDR_Left 35 // LDR1 pin
#define LDR_Right 34 // LDR2 pin

#define SSID "SLT-Fiber-4B2B"  // Wifi SSID
#define PASSWORD "msii6267963" // Wifi password


// Default values for min anflge, control factor for each medicine type
#define default_angle 30
#define default_fac 0.75

#define medA_angle 45
#define medA_fac 0.25

#define medB_angle 60
#define medB_fac 0.5

#define medC_angle 75
#define medC_fac 0.6

#define default_med_type 4 // Default medicine type when power on
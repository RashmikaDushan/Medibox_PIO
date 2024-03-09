// Defnitions
#define NTP_SERVER "pool.ntp.org" // NTP server
#define UTC_OFFSET_DST 0          // UTC daytime offset = 0 for no daytime offset

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin# (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0X3C ///< See datasheet for Address; 0X3D for 128x64,

#define BUZZER 18 // Buzzer pin

#define LED_GREEN 15 // Green LED pin
#define LED_RED 2    // RED LED pin

#define CANCEL 25 // Cancel button
#define UP 26     // Up button
#define DOWN 33   // Down button
#define OK 32     // OK button

#define DHT 12 // DHT sensor pin

#define SSID "Wokwi-GUEST"  // Wifi SSID
#define PASSWORD "" // Wifi password
// Libraries
#include <WiFi.h>
#include <Wire.h>

#include <functions.h>
#include <definitions.h>
#include <globals.h>

void setup()
{
  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT); // Buzzer output pin

  pinMode(LED_GREEN, OUTPUT); // Green LED output pin
  pinMode(LED_RED, OUTPUT);   // Red LED output pin

  pinMode(CANCEL, INPUT_PULLUP); // Cancel button input pin
  pinMode(UP, INPUT_PULLUP);     // Up button input pin
  pinMode(DOWN, INPUT_PULLUP);   // Down button input pin
  pinMode(OK, INPUT_PULLUP);     // OK button input pin

  dhtSensor.setup(DHT, DHTesp::DHT22); // DHT sensor setup

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) // OLED display setup
  {
    Serial.println(F("SSD1306 allocation failed")); // If the display allocation failed
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay(); // Clear the display
  print_line("Welcome to Medibox", 1, 0, 0, true);
  delay(3000);

  WiFi.begin(SSID, PASSWORD, 6);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    display.clearDisplay();
    print_line("Connecting to Wifi   " + String(SSID), 1, 0, 0, true); // (String, text_size, cursor_row, cursor_column)
  }
  delay(2000);
  display.clearDisplay();
  print_line("Connected to Wifi!!!", 1, 0, 0, true);
  delay(2000);

  set_time_zone(true); // ask to enter the time zone
}

void loop()
{
  if (button_press() == OK) // If pressed OK, go to the menu
  {
    go_to_menu();
  }
  update_time_temp_humd_alarm(); // Update the display with the time, temperature, humidity and check alarms
  delay(100);
}

// Libraries
#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>

#include <functions.h>
#include <definitions.h>
#include <globals.h>
#include <icons.h>
#include <mqtt.h>
#include <ldr.h>
#include <servo.h>

char* ldr_data;
char* temp_data;
bool on_off = 1;

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

  pinMode(LDR_Left, INPUT); // LDR1 input pin
  pinMode(LDR_Right, INPUT); // LDR2 input pin

  dhtSensor.setup(DHT, DHTesp::DHT22); // DHT sensor setup

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) // OLED display setup
  {
    Serial.println(F("SSD1306 allocation failed")); // If the display allocation failed
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay(); // Clear the display
  print_line("Welcome", 2, 0, 22, true);
  print_line("to", 1, 18, 57, true);
  print_line("Medibox", 2, 29, 22, true);
  print_line("210057R", 1, 53, 45, true);
  delay(3000);
  play_animaton(3,heartbeat,4,48,40, 8);

  WiFi.begin(SSID, PASSWORD, 6);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    display.clearDisplay();
    print_line(" Connecting to Wifi!    " + String(SSID), 1, 20, 0, true); // (String, text_size, cursor_row, cursor_column)
  }
  delay(2000);
  play_animaton(1,wifi,2,32,48, 16);
  display.clearDisplay();
  print_line(" Connected to Wifi!!!", 1, 30, 0, true);
  Serial.println("Connected to Wifi");
  delay(2000);

  set_time_zone(true); // ask to enter the time zone
  mqtt_setup(); // MQTT setup
  ldr_data=ldr_read();
  setup_servo(); // Servo setup
}

void loop()
{ 
  if(on_off){
    if (button_press() == OK) // If pressed OK, go to the menu
  {
    go_to_menu();
  }
  temp_data = update_time_temp_humd_alarm(); // Update the display with the time, temperature, humidity and check alarms
  delay(100);
  ldr_data = ldr_read(); // LDR JSON data
  }
  else{
    display.clearDisplay();
    print_line("System is off", 1, 30, 0, true);
    delay(1000);}
  servo_control(min_angle,control_fac);
  mqtt_loop(ldr_data, temp_data);
}

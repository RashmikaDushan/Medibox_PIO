#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <globals.h>
#include <definitions.h>

#include <mqtt.h>
#include <servo.h>

// MQTT broker details
const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
// Subscribed topics
const char *mqtt_sub_topic_ang = "dushan/sub/ang";
const char *mqtt_sub_topic_fac = "dushan/sub/fac";
const char *mqtt_sub_topic_on = "dushan/sub/on_off";
const char *mqtt_sub_topic_med = "dushan/sub/med";

// Publishing topics
const char *mqtt_pub_topic_ldr = "dushan/pub/ldr";
const char *mqtt_pub_topic_temp = "dushan/pub/temp";

// Default values
int min_angle = default_angle;
float control_fac = default_fac;
int medicine = default_med_type;

WiFiClient espClient;
PubSubClient client(espClient);

void change_medicine_type(int medicine)
{
  switch (medicine)
  {
  case 1:
    min_angle = medA_angle;
    control_fac = medA_fac;
    break;
  case 2:
    min_angle = medB_angle;
    control_fac = medB_fac;
    break;
  case 3:
    min_angle = medC_angle;
    control_fac = medC_fac;
    break;
  default:
    break;
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char temp[length + 1]; // Temporary character array for payload

  for (unsigned int i = 0; i < length; i++)
  {
    temp[i] = (char)payload[i];
  }
  temp[length] = '\0';

  // Change the main switch on / off
  if (strcmp(topic, mqtt_sub_topic_on) == 0)
  {
    // if recieved true, on_off = true
    if (strcmp(temp, "true") == 0)
    {
      on_off = true;
    }
    // else on_off = false
    else if (strcmp(temp, "false") == 0)
    {
      on_off = false;
    }
    Serial.println(on_off);
  }

  // Change the min angle
  else if (strcmp(topic, mqtt_sub_topic_ang) == 0)
  {
    min_angle = atoi(temp);
  }

  // Change the control factor
  else if (strcmp(topic, mqtt_sub_topic_fac) == 0)
  {
    control_fac = atof(temp);
  }

  // Change the medicine type
  else if (strcmp(topic, mqtt_sub_topic_med) == 0)
  {
    medicine = atoi(temp);
  }

  // change the min angle and control factor according to the medicine type
  change_medicine_type(medicine);

  // If main switch is on control the servo
  if (on_off)
  {
    servo_control(min_angle, control_fac);
  }
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Dushanbalasooriya_ESP32Client"))
    {
      Serial.println("connected");
      client.subscribe(mqtt_sub_topic_ang);
      client.subscribe(mqtt_sub_topic_fac);
      client.subscribe(mqtt_sub_topic_on);
      client.subscribe(mqtt_sub_topic_med);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqtt_setup()
{
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  Serial.println("MQTT setup done");
}

void mqtt_loop(char *ldr_data, char *temp_data)
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  static unsigned long lastMillis = 0;
  if (millis() - lastMillis > 5000)
  {
    lastMillis = millis();

    if (on_off)
    {
      client.publish(mqtt_pub_topic_ldr, ldr_data);
      client.publish(mqtt_pub_topic_temp, temp_data);
    }
  }
}
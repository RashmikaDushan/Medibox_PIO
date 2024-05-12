#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <globals.h>

#include <mqtt.h>
#include <servo.h>


// MQTT broker details
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
// const char* mqtt_username = "rush";
// const char* mqtt_password = "dushan";
const char* mqtt_sub_topic_ang = "dushan/sub/ang";
const char* mqtt_sub_topic_fac = "dushan/sub/fac";
const char* mqtt_pub_topic_ldr = "dushan/pub/ldr";
const char* mqtt_pub_topic_temp = "dushan/pub/temp";

int min_angle = 30;
float control_fac = 0.75;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strcmp(topic, mqtt_sub_topic_ang) == 0) {
    min_angle =  atoi((char*)payload);
  } else if (strcmp(topic, mqtt_sub_topic_fac) == 0) {
    control_fac = atof((char*)payload);
  }

  servo_control(min_angle,control_fac);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Dushanbalasooriya_ESP32Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_sub_topic_ang);
      client.subscribe(mqtt_sub_topic_fac);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void mqtt_setup() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  Serial.println("MQTT setup done");
}

void mqtt_loop(char* ldr_data,char* temp_data) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Send a test message every 5 seconds
  static unsigned long lastMillis = 0;
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    client.publish(mqtt_pub_topic_ldr, ldr_data);
    client.publish(mqtt_pub_topic_temp, temp_data);
  }
}
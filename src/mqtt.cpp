#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include <mqtt.h>


// MQTT broker details
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
// const char* mqtt_username = "rush";
// const char* mqtt_password = "dushan";
const char* mqtt_sub_topic = "dushan/sub";
const char* mqtt_pub_topic_ldr = "dushan/pub/ldr";
const char* mqtt_pub_topic_temp = "dushan/pub/temp";

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Dushanbalasooriya_ESP32Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload, length);

  Serial.print("Message received from MQTT broker:");
  Serial.println(doc["msg"].as<String>());
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
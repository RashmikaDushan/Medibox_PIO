#include <functions.h>
#include <globals.h>
#include <definitions.h>

// object Declarations
DHTesp dhtSensor;// Declaration for a DHT sensor

// check and print the humidity level and the temprature
char* check_temp_humd(void)
{
    StaticJsonDocument<200> doc;

    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    doc["temperature"] = data.temperature; // Create a JSON object
    doc["humidity"] = data.humidity;

    bool all_good = true;
    if (data.temperature > 35)
    {
        all_good = false;
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        print_line("TEMP HIGH", 1, 45, 0, false);
        alarm_temp_humidity(2, 7);
    }
    else if (data.temperature < 25)
    {
        all_good = false;
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        print_line("TEMP LOW", 1, 45, 0, false);
        alarm_temp_humidity(2, 0);
    }
    else
    {
        print_line("Temprature is:" + String(data.temperature) + "C", 1, 45, 0, false);
    }
    if (data.humidity > 85)
    {
        all_good = false;
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        print_line("HUMD HIGH", 1, 56, 0, false);
        alarm_temp_humidity(3, 7);
    }
    else if (data.humidity < 35)
    {
        all_good = false;
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        print_line("HUMD LOW", 1, 56, 0, false);
        alarm_temp_humidity(3, 0);
    }
    else
    {
        print_line("Humidity is:" + String(data.humidity) + "%", 1, 56, 0, 0);
    }
    if (all_good)
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
    }

    char* jsonData = new char[200];
    serializeJson(doc, jsonData, 200);
    
    return jsonData;
}

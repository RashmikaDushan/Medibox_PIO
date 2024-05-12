#include <ldr.h>
#include <definitions.h>
#include <globals.h>

int ldr_left;
int ldr_right;

float light_intensity = 0.0;
bool max_ldr_left = false;

char *ldr_read()
{

  StaticJsonDocument<200> doc;

  ldr_left = analogRead(LDR_Left);   // Read LDR1 value
  ldr_right = analogRead(LDR_Right); // Read LDR2 value

  if (ldr_left >= ldr_right)
  {
    doc["ldr"] = ldr_right;
    doc["ldr_number"] = "Right";
    light_intensity = ldr_right/4096.0;
    max_ldr_left = false;
  }
  else
  {
    doc["ldr"] = ldr_left;
    doc["ldr_number"] = "Left";
    max_ldr_left = true;
    light_intensity = ldr_left/4096.0;
  }

  char *jsonData = new char[200];
  serializeJson(doc, jsonData, 200);

  return jsonData;
}
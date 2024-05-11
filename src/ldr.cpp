#include <ldr.h>
#include <definitions.h>

int ldr_1;
int ldr_2;

char* ldr_read() {
  
  StaticJsonDocument<200> doc;

  ldr_1 = analogRead(LDR1); // Read LDR1 value
  ldr_2 = analogRead(LDR2); // Read LDR2 value

  if(ldr_1 <= ldr_2){
    doc["ldr"] = ldr_1;
    doc["ldr_number"] = 1;
  }
  else{
    doc["ldr"] = ldr_2;
    doc["ldr_number"] = 2;
  }

  char* jsonData = new char[200];
  serializeJson(doc, jsonData,200);

  return jsonData;
}
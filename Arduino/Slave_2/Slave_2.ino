#include <ArduinoJson.h>

DynamicJsonDocument doc(1024);

void setup() {
  Serial.begin(9600);
}

void loop() {
  readSensor();
  delay(3000);
}

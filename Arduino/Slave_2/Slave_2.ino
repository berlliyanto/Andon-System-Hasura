#include <ArduinoJson.h>
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Font3x5.h"
#include "Verdana10b.h"

#define proximityPin 2

#define DISPLAYS_ACROSS 1  //-> Number of P10 panels used, side to side.
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

DynamicJsonDocument docSlave2(100);
const char* CURRENT_STATE = "standby";
const char* CURRENT_TYPE = "None";
int shotCounter = 0;
int proximityState = 0;

unsigned long previousMillis = 0;
unsigned long interval = 100;

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Timer1.initialize(1000);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);
  pinMode(proximityPin, INPUT);
  Serial.begin(9600);
  docSlave2["state"] = "ready";
  docSlave2["type"] = "none";
  serializeJson(docSlave2, Serial);
  Serial.println();
  dmd.selectFont(Verdana10b);
}

void loop() {
  unsigned long currentMillis = millis();
  // mainConSerial();
  p10State(CURRENT_STATE, CURRENT_TYPE);

  if (currentMillis - previousMillis >= interval) {
    // readProximity();
    previousMillis = currentMillis;
  }
  //  for (int i = 0; i < 70; i++) {
  //   // dmd.drawString(-32 + i, -2, "STANDBY", 7, GRAPHICS_NORMAL);
  //   // dmd.drawString(-32 + i, 7, "STANDBY", 7, GRAPHICS_NORMAL);
  //   Serial.println(i);
  //   delay(100);
  //   dmd.clearScreen(true);
  // }
  // Serial.println("123");
  // delay(1000);
}

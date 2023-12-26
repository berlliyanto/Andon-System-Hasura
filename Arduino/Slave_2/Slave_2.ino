#include <ArduinoJson.h>
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Font3x5.h"
#include "huruf_kecilku.h"

#define proximityPin 2

#define DISPLAYS_ACROSS 1  //-> Number of P10 panels used, side to side.
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

DynamicJsonDocument docSlave2(100);
const char* CURRENT_STATE = "standby";
const char* CURRENT_TYPE = "None";
int shotCounter = 0;
int proximityState = 0;

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup() {
  Timer1.initialize(1000);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);
  pinMode(proximityPin, INPUT);
  Serial.begin(9600);
  Serial.println("Ready to go");
}

void loop() {
  dmd.selectFont(System5x7);
  mainConSerial();
  p10State(CURRENT_STATE, CURRENT_TYPE);
  readProximity();
}

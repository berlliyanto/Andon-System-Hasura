#include <ArduinoJson.h>
#include <DMD32.h> 
#include "fonts/Verdana10b.h"
#include "fonts/SystemFont5x7.h"
#include <HardwareSerial.h>

//FOR MATRIX RGB
// #define R1 25
// #define G1 26
// #define BL1 27
// #define R2 14
// #define G2 12
// #define BL2 13
// #define CH_A 23
// #define CH_B 19
// #define CH_C 5
// #define CH_D 18
// #define CH_E -1 // assign to any available pin if using two panels or 64x64 panels with 1/32 scan
// #define CLK 2
// #define LAT 4
// #define OE 15

#define proximityPin 35
#define RXm 16
#define TXm 17

#define DISPLAYS_ACROSS 1  //-> Number of P10 panels used, side to side.
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

HardwareSerial SerialMaster(2);

DynamicJsonDocument docSlave2(100);
const char* CURRENT_STATE = "standby";
const char* CURRENT_TYPE = "None";
int shotCounter = 0;
int proximityState = 0;

unsigned long previousMillis = 0;
unsigned long interval = 100;

hw_timer_t* timer = NULL;

void IRAM_ATTR triggerScan() {
  dmd.scanDisplayBySPI();
}

void setup() {
  uint8_t cpuClock = ESP.getCpuFreqMHz();

  timer = timerBegin(0, cpuClock, true);
  delay(500);

  timerAttachInterrupt(timer, &triggerScan, true);
  delay(500);

  timerAlarmWrite(timer, 300, true);
  delay(500);

  timerAlarmEnable(timer);
  delay(500);

  dmd.clearScreen(true); 
  pinMode(proximityPin, INPUT);
  
  Serial.begin(115200);
  SerialMaster.begin(115200, SERIAL_8N1, RXm, TXm);
  delay(1000);

  docSlave2["state"] = "ready";
  docSlave2["type"] = "none";
  serializeJson(docSlave2, SerialMaster);
  SerialMaster.println();
  
}

void loop() {

  dmd.selectFont(Verdana10b);
  unsigned long currentMillis = millis();
  mainConSerial();
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

#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <Fonts/Picopixel.h>
#include "muHeavy8ptBold.h"
#include <vector>

// FOR MATRIX RGB
#define R1 25
#define G1 26
#define BL1 27
#define R2 14
#define G2 12
#define BL2 13
#define CH_A 23
#define CH_B 19
#define CH_C 5
#define CH_D 18
#define CH_E -1  // assign to any available pin if using two panels or 64x64 panels with 1/32 scan
#define CLK 2
#define LAT 4
#define OE 15

#define proximityPin 35
#define RXm 16
#define TXm 17

// #define DISPLAYS_ACROSS 1  //-> Number of P10 panels used, side to side.
// #define DISPLAYS_DOWN 1
// DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

HardwareSerial SerialMaster(2);
DynamicJsonDocument docSlave2(4096);
MatrixPanel_I2S_DMA* dma_display = nullptr;
HUB75_I2S_CFG::i2s_pins _pins = { R1, G1, BL1, R2, G2, BL2, CH_A, CH_B, CH_C, CH_D, CH_E, LAT, OE, CLK };

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);
uint16_t myYELLOW = dma_display->color565(255, 255, 0);
uint16_t myMAGENTA = dma_display->color565(255, 0, 255);
uint16_t myCYAN = dma_display->color565(0, 255, 255);
uint16_t myORANGE = dma_display->color565(255, 165, 0);
uint16_t myPINK = dma_display->color565(255, 182, 193);
uint16_t myPURPLE = dma_display->color565(128, 0, 128);
uint16_t myLIME = dma_display->color565(0, 255, 0);
uint16_t myBROWN = dma_display->color565(165, 42, 42);
uint16_t myGOLD = dma_display->color565(255, 215, 0);
uint16_t myTEAL = dma_display->color565(0, 128, 128);
uint16_t myNAVY = dma_display->color565(0, 0, 128);
uint16_t myOLIVE = dma_display->color565(128, 128, 0);
uint16_t myMAROON = dma_display->color565(128, 0, 0);
uint16_t mySKYBLUE = dma_display->color565(135, 206, 250);
uint16_t myTURQUOISE = dma_display->color565(64, 224, 208);

std::vector<String> listCall;
const char* firstLine = "STANDBY";
int lastFirstLineXPos = 0;
int lastSecondLineXPos = 0;
int shotCounter = 0;
int proximityState = 0;

unsigned long previousMillis = 0;
unsigned long interval = 500;

hw_timer_t* timer = NULL;

// void IRAM_ATTR triggerScan() {
//   dmd.scanDisplayBySPI();
// }

void setup() {
  // uint8_t cpuClock = ESP.getCpuFreqMHz();

  // timer = timerBegin(0, cpuClock, true);
  // delay(500);

  // timerAttachInterrupt(timer, &triggerScan, true);
  // delay(500);

  // timerAlarmWrite(timer, 300, true);
  // delay(500);

  // timerAlarmEnable(timer);
  // delay(500);

  // dmd.clearScreen(true);

  // Module configuration
  HUB75_I2S_CFG mxconfig(
    32,                     // width
    16,                     // height
    1,                      // chain length
    _pins,                  // pin mapping
    HUB75_I2S_CFG::FM6126A  // driver chip
  );

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(10);  //0-255
  dma_display->setFont(&muHeavy8ptBold);
  dma_display->setTextSize(1);
  dma_display->setTextWrap(false);
  dma_display->clearScreen();

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
  unsigned long currentMillis = millis();
  mainConSerial();
  displayP10();

  if (currentMillis - previousMillis >= interval) {
    // readProximity();
    previousMillis = currentMillis;
  }
}

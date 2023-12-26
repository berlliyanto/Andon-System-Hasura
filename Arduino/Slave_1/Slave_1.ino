#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <ArduinoJson.h>
// #include <TFT_eSPI.h>
// #include <qrcode.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define shotCountPin 10

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

DynamicJsonDocument docSlave1(100);

MCUFRIEND_kbv tft;
Adafruit_GFX_Button callBtn, scanBtn, endBtn, backBtn, MTCallBtn, MoldCallBtn,
  TekCallBtn, MatCallBtn, QuaCallBtn, CHScanBtn, AddScanBtn, SerScanBtn, RunChangeBtn;

const int XP = 6, XM = A2, YP = A1, YM = 7;
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int pX, pY;
char* CURRENT_VIEW = "HOME";
char* LAST_VIEW = "HOME";
char* LAST_VIEW2 = "";
char* LAST_VIEW3 = "";
char* RFID_STATUS = "";
bool isChangedView = false;
bool readyToChangeView = true;

//Millis
unsigned long touchLastMillis = 0;
unsigned long touchIntervalMillis = 50;

void setup() {
  Serial.begin(9600);
  pinMode(shotCountPin, INPUT_PULLUP);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  HomeView("HOME");
}

void loop() {
  // unsigned long currentMillis = millis();
  routes();
  touchOperation();
  shotCount();
  // if (currentMillis - touchLastMillis >= touchIntervalMillis) {
    
  //   touchLastMillis = currentMillis;
  // }

}
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include "qrcode.h"
#include <vector>

// --------------Define ----------------//

#define Lcd_X 320
#define Lcd_Y 480

#define RXm 16
#define TXm 17

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define USE_LOCAL_KBV 1
#define shotCountPin 35

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#if defined(ESP32)
int XP = 27, YP = 4, XM = 15, YM = 14;  //most common configuration
#else
int XP = 7, YP = A2, XM = A1, YM = 6;  //next common configuration
#endif
#if USE_LOCAL_KBV
#include "TouchScreen_kbv.h"  //my hacked version
#define TouchScreen TouchScreen_kbv
#define TSPoint TSPoint_kbv
#else
#include <TouchScreen.h>  //Adafruit Library
#endif
// --------------Define End ----------------//

// --------------Declare Variable ----------------//
std::vector<String> listNgIds;
std::vector<String> listItemId;

int pX, pY;
int listItemsLength = 0;

// VARIABEL TO KEEP STACK OF ROUTE OF PAGES
char* CURRENT_VIEW = "HOME";
char* LAST_VIEW = "HOME";
char* LAST_VIEW2 = "";
char* LAST_VIEW3 = "";

char* RFID_STATUS = "";
const char* HOME_STATUS = "STANDBY";
bool isChangedView = false;
bool readyToChangeView = true;
int callCount = 0;
int manPower = 0;
bool isLoading = false;
bool connectionStatus = false;

String listItems = "";
String itemId = "";

char* valueNg1Ids[] = { "181", "177", "173", "179", "184" };
char* valueNg2Ids[] = { "178", "187", "189", "191", "175" };
char* keyNg1[] = { "black_dot", "silver", "shortshot", "running_in", "color" };
char* keyNg2[] = { "weld_line", "crack", "mold", "problem", "sink_mark" };
char* buttonNgList1[] = { "Black Dot", "Silver", "Shortshot", "RunningIn", "Color" };
char* buttonNgList2[] = { "Weld Line", "Crack", "Pin Mark", "Under Out", "Sink Mark" };
uint16_t colorButtonNgList1[] = { WHITE, WHITE, WHITE, WHITE, WHITE };
uint16_t colorButtonNgList2[] = { WHITE, WHITE, WHITE, WHITE, WHITE };
bool isButtonNgList1Pressed[] = { false, false, false, false, false };
bool isButtonNgList2Pressed[] = { false, false, false, false, false };

char* buttonNgListState[] = {};
char* buttonNgList2State[] = {};

String currentType = "";
const char* workOrderNo = "";

const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

// --------------Object Initialize ----------------//
QRCode qrcode;
DynamicJsonDocument docSlave1(4096);
DynamicJsonDocument docSlave1ForNG(512);
DynamicJsonDocument docSlave1ForItems(2048);
HardwareSerial SerialMaster(2);
MCUFRIEND_kbv tft;
Adafruit_GFX_Button callBtn, scanBtn, endBtn, backBtn, MTCallBtn, MoldCallBtn,
  TekCallBtn, MatCallBtn, QuaCallBtn, CHScanBtn, AddScanBtn, SerScanBtn, QrScanBtn,
   RunChangeBtn, NgBtn, submitBtn, NgListBtn1[5], NgListBtn2[5];
Adafruit_GFX_Button* ItemButtons;
TouchScreen ts(XP, YP, XM, YM, 300);  //re-initialised after diagnose
// --------------Object Initialize END ----------------//

//Millis
unsigned long loadingMillis = 0;
unsigned long loadingIntervalMillis = 50;

const int QRcode_Version = 8;  // set the version (range 1->40)
const int QRcode_ECC = 0;      // set the Error Correction level (range 0-3) or symbolic (ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH)
const char* QR_VALUE = "";
// --------------Declare Variable END ----------------//

void setup() {
  Serial.begin(115200);
  SerialMaster.begin(115200, SERIAL_8N1, RXm, TXm);
  delay(1000);
  pinMode(shotCountPin, INPUT_PULLUP);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;
  tft.begin(ID);
  tft.setRotation(0);
  HomeView("HOME");
  Serial.print("ID: ");
  Serial.println(ID);
}

void loop() {
  routes();          // LISTEN CURRENT ROUTES
  touchOperation();  // TFT TOUCH OPERATION
  // shotCount(); // LISTEN INPUT FROM PIN IF HIGH SEND THE SHOT TO MASTER
  mainConSerial();  // MAIN SERIAL COMMUNICATION FROM MASTER
  LoadingCircle();  // LOADING ANIMATION
  // indicator(); // CONNECTION STATUS ON TOP RIGHT TFT
}

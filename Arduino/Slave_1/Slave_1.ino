#include <SPFD5408_Adafruit_GFX.h>     // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h>  // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// Calibrate values
#define TS_MINX 120
#define TS_MINY 145
#define TS_MAXX 935
#define TS_MAXY 905

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define BTN_WIDTH 240

#define BTN_HOME_HEIGHT 70
#define BTN_HOME_MARGIN 8
#define BTN_HOME_QTY 3
const int BTN_HOME_COLOR[3] PROGMEM = { RED, CYAN, BLUE };
const char* const BTN_HOME_LABEL[3] PROGMEM = { "CALL", "SCAN", "END" };

#define BTN_CALL_HEIGHT 50
#define BTN_CALL_MARGIN 5
#define BTN_CALL_QTY 5
const int BTN_CALL_COLOR[5] PROGMEM = { RED, CYAN, YELLOW, MAGENTA, WHITE };
const char* const BTN_CALL_LABEL[5] PROGMEM = { "MAINTENANCE", "MOLD", "TEKNISI", "MATERIAL", "QUALITY" };

#define BTN_SCAN_HEIGHT 70
#define BTN_SCAN_MARGIN 8
#define BTN_SCAN_QTY 3
const int BTN_SCAN_COLOR[3] PROGMEM = { GREEN, WHITE, CYAN };
const char* const BTN_SCAN_LABEL[3] PROGMEM = { "CHANGE", "ADD MP", "SERVE" };

#define BTN_CHANGE_HEIGHT 40
#define BTN_CHANGE_MARGIN 5
#define BTN_CHANGE_QTY 6
const int BTN_CHANGE_COLOR[6] PROGMEM = { GREEN, RED, CYAN, YELLOW, MAGENTA, WHITE };
const char* const BTN_CHANGE_LABEL[6] PROGMEM = { "RUNNING", "MAINTENANCE", "MOLD", "TEKNISI", "MATERIAL", "QUALITY" };

#define BTN_BACK_MARGIN 10
#define BTN_BACK_COLOR WHITE
#define BTN_BACK_LABEL "<BACK"

#define INDICATOR_TIMEOUT 5000
#define REQUEST_REFRESH_TIMEOUT 60000
#define REQUEST_TIMEOUT 10000
#define MAX_ATTEMPTS 1

#define BOXSIZE 40
#define PENRADIUS 3

#define BTN_BACK_MARGIN 10
#define BTN_BACK_COLOR WHITE
#define BTN_BACK_LABEL "<BACK"

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
char* ACTIVE_SCREEN = "HOME";
int BTN_START_X = 0;
int BTN_START_Y = 0;
char* OLD_STATUS = "";
char* CURRENT_STATUS = "";
boolean IS_SERVE = false;
char* RFID_STATUS = "";

static byte session;
static uint32_t refreshTimer;
static uint32_t requestTimer;
static uint32_t indicatorTimer;
bool waitingResponse = 0;
int attempt;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Lets go!!!"));

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);

  BTN_START_X = (tft.width() - BTN_WIDTH) / 2;

  delay(10);
  attempt = 0;
  refreshTimer = 0;
  indicatorTimer = 0;

  showHomeScreen("");
}

void loop() {
  if (millis() > indicatorTimer && !waitingResponse) tft.fillCircle(tft.width() - 10, 7, 5, BLACK);

  if (millis() > refreshTimer) {
    if (CURRENT_STATUS == "") refreshTimer = millis() + 5000;
    else refreshTimer = millis() + REQUEST_REFRESH_TIMEOUT;
  }

  if (CURRENT_STATUS == "") {
    normalState();
  }

  delay(50);
}

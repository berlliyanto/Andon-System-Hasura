#include <SPI.h>
#include <EthernetENC.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <vector>

HardwareSerial SerialSlave1(1);
HardwareSerial SerialSlave2(2);

//------------Defined PIN----------------//

#define RXs1 16
#define TXs1 17
#define RXs2 26
#define TXs2 25
#define RFID_SS_PIN 4
#define RFID_RST_PIN 22
#define ETHERNET_SS_PIN 5

//------------Defined PIN END----------------//

//------------Variable Declaration----------------//
// Number of known default keys (hard-coded)
// NOTE: Synchronize the NR_KNOWN_KEYS define with the defaultKeys[] array
#define NR_KNOWN_KEYS   8
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};

#define INDICATOR_TIMEOUT 5000
#define REQUEST_REFRESH_TIMEOUT 60000
#define REQUEST_TIMEOUT 20000

static uint32_t refreshTimer;
static uint32_t requestTimer;
static uint32_t indicatorTimer;
bool waitingResponse = false;

String wcCode = "1WCINJ-009";

byte serverERP[] = { 89, 116, 231, 224 };  // name address for erp hasura
byte mac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x32 };
byte serverDev[] = { 192, 168, 1, 124 };
byte serverTes[] = { 192, 168, 1, 141 };

bool onInit = true;

String host = "89.116.231.224";
String hostDev = "192.168.1.124";
String hostTes = "192.168.1.141";
int port = 800;
int portDev = 800;
int portTes = 8080;

bool printSerial1State = true;
bool printSerial2State = true;

//GLOBAL VARIABEL STATE AND TYPE
const char* stateFromSlave1 = "standby";
const char* typeFromSlave1 = "none";

const char* stateFromSlave2 = "";
const char* typeFromSlave2 = "none";

int workOrderNo  = 0;
int workCenterId = 0;

//MILLIS INTERVAL
unsigned long previousApiMillis = 0;
const long intervalApi = 50;
unsigned long previousRfidMillis = 0;
const long intervalRfid = 10;
unsigned long previousSerial1Millis = 0;
const long intervalSerial1 = 15;
unsigned long previousSerial2Millis = 0;
const long intervalSerial2 = 20;
unsigned long previousListenMillis = 0;
const long intervalListen = 20000;

const char* currentStatus = "";
const char* oldStatus = "";
bool isListenStatus = false;
String holdingJson = ""; // JSON FOR SEND TO SLAVE
String rfidValue = ""; // KEEP RFID VALUE

//------------Variable Declaration END----------------//


//------------Instance Object--------------//
EthernetClient client;
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
DynamicJsonDocument docSlave1(4096); // JSON FOR SLAVE 1
DynamicJsonDocument docSlave1Loading(200); // JSON FOR LOADING ANIMATION
DynamicJsonDocument docSlave2(4096); // JSON FOR SLAVE 2
DynamicJsonDocument bodyResponse(4096); // JSON TO KEEP RESPONSE BODY
DynamicJsonDocument tempJson(4096); // SEND JSON

//------------Instance Object END--------------//

void setup() {
  pinMode(ETHERNET_SS_PIN, OUTPUT);
  pinMode(RFID_SS_PIN, OUTPUT);
  digitalWrite(ETHERNET_SS_PIN, HIGH);
  digitalWrite(RFID_SS_PIN, HIGH);

  SPI.begin();

  digitalWrite(RFID_SS_PIN, LOW);
  rfid.PCD_Init();
  digitalWrite(RFID_SS_PIN, HIGH);
  delay(1000);
  digitalWrite(ETHERNET_SS_PIN, LOW);
  Ethernet.init(ETHERNET_SS_PIN);
  digitalWrite(ETHERNET_SS_PIN, HIGH);

  Serial.begin(115200);
  delay(1000);
  Serial.println("Begin Ethernet");
  ethernetBegin(mac);  //ethernet_config.ino

  SerialSlave1.begin(115200, SERIAL_8N1, RXs1, TXs1);  //SLAVE 1
  SerialSlave2.begin(115200, SERIAL_8N1, RXs2, TXs2);  //SLAVE 2

  connectClient(serverDev, portDev);
}

void loop() {
  Ethernet.maintain();
  unsigned long millisTime = millis();

  /*
    ALWAYS USE THE MILLIS FUNCTION FOR MULTI PROCESSES AND AVOID PROCESS BLOCKING
    BECAUSE IN THE MASTER THERE IS A API CALL PROCESS
  */

  //RFID OPERATION
  if (millisTime - previousRfidMillis >= intervalRfid) {
    readRFID(holdingJson);
    previousRfidMillis = millisTime;
  }

  //SERIAL 1 COMMUNICATION
  if (millisTime - previousSerial1Millis >= intervalSerial1) {
    mainConSerial1();
    previousSerial1Millis = millisTime;
  }

  //SERIAL 2 COMMUNICATION
  if (millisTime - previousSerial2Millis >= intervalSerial2) {
    mainConSerial2();
    previousSerial2Millis = millisTime;
  }

  //RECEIVE RESPONSE
  if (millisTime - previousApiMillis >= intervalApi) {
    receiveResponse();
    previousApiMillis = millisTime;
  } 

  //TIMEOUT
  if (millis() > requestTimer && waitingResponse) {
    Serial.println("TimeOut");
    sendLoadingState("isNotLoading");
    waitingResponse = false;
  }

  //LISTEN STATUS
  // if(millisTime - previousListenMillis >= intervalListen){
  //   listenStatus();
  //   previousListenMillis = millisTime;
  // }
}
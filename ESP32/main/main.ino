#include <SPI.h>
#include <EthernetENC.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

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

String wcCode = "1WCINJ-008";

byte serverERP[] = { 89, 116, 231, 224 };  // name address for erp hasura
byte mac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x32 };
byte serverDev[] = { 192, 168, 1, 154 };
byte serverTes[] = { 192, 168, 1, 191 };

bool initState = true;
String host = "89.116.231.224";
String hostDev = "192.168.1.154";
String hostTes = "192.168.1.191";
int port = 800;
int portTes = 80;

bool printSerial1State = true;
bool printSerial2State = true;

const char* stateFromSlave1 = "standby";
const char* typeFromSlave1 = "none";

const char* stateFromSlave2 = "";
const char* typeFromSlave2 = "none";

unsigned long previousApiMillis = 0;
const long intervalApi = 2000;
unsigned long previousRfidMillis = 0;
const long intervalRfid = 10;
unsigned long previousSerial1Millis = 0;
const long intervalSerial1 = 15;
unsigned long previousSerial2Millis = 0;
const long intervalSerial2 = 20;

String holdingJson = "";

//------------Variable Declaration END----------------//


//------------Instance Object--------------//
EthernetServer server = EthernetServer(23);
EthernetClient client;
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
DynamicJsonDocument docSlave1(200);
DynamicJsonDocument docSlave1Loading(200);
DynamicJsonDocument docSlave2(200);

//------------Instance Object END--------------//

void setup() {
  pinMode(RFID_SS_PIN, OUTPUT);
  digitalWrite(RFID_SS_PIN, LOW);  

  pinMode(ETHERNET_SS_PIN, OUTPUT);
  digitalWrite(ETHERNET_SS_PIN, HIGH);

  SPI.begin();
  delay(1000);
  rfid.PCD_Init();
  Ethernet.init(5);

  Serial.begin(115200);
  delay(1000);
  Serial.println("Begin Ethernet");
  ethernetBegin(mac);  //ethernet_config.ino

  SerialSlave1.begin(115200, SERIAL_8N1, RXs1, TXs1);  //SLAVE 1
  SerialSlave2.begin(115200, SERIAL_8N1, RXs2, TXs2);  //SLAVE 2

  connectClient(serverDev, port);
}

void loop() {
  unsigned long millisTime = millis();

  if (millisTime - previousRfidMillis >= intervalRfid) {
    readRFID(holdingJson);
    previousRfidMillis = millisTime;
  }

  if (millisTime - previousSerial1Millis >= intervalSerial1) {
    mainConSerial1();
    previousSerial1Millis = millisTime;
  }

  if (millisTime - previousSerial2Millis >= intervalSerial2) {
    mainConSerial2();
    previousSerial2Millis = millisTime;
  }

  if (millisTime - previousApiMillis >= intervalApi) {
    receiveResponse();
  }
}
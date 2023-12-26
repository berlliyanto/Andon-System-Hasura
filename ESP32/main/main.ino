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
#define SS_PIN 4
#define RST_PIN 22

//------------Defined PIN END----------------//

//------------Variable Declaration----------------//

byte serverERP[] = { 89, 116, 231, 224 };  // name address for erp hasura
byte mac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x32 };

bool isClientConnected = false;
String host = "89.116.231.224";
int portERP = 800;

bool printSerial1State = true;
bool printSerial2State = true;
bool readyToTap = true;
bool printWebData = true;

const char* stateFromSlave1 = "standby";
const char* typeFromSlave1 = "none";

unsigned long previousApiMillis = 0;
const long intervalApi = 2000;
unsigned long previousRfidMillis = 0;
const long intervalRfid = 10;
unsigned long previousSerial1Millis = 0;
const long intervalSerial1 = 15;
unsigned long previousSerial2Millis = 0;
const long intervalSerial2 = 20;

//------------Variable Declaration END----------------//


//------------Instance Object--------------//

EthernetClient client;
MFRC522 rfid(SS_PIN, RST_PIN);
StaticJsonDocument<200> docSlave1;
StaticJsonDocument<200> docSlave2;

//------------Instance Object END--------------//

void setup() {
  SPI.begin();
  rfid.PCD_Init();
  Ethernet.init(5);

  Serial.begin(115200);
  delay(1000);
  Serial.println("Begin Ethernet");
  ethernetBegin(mac);  //ethernet_config.ino

  SerialSlave1.begin(9600, SERIAL_8N1, RXs1, TXs1);  //SLAVE 1
  SerialSlave2.begin(9600, SERIAL_8N1, RXs2, TXs2);  //SLAVE 2

  connectClient(serverERP, portERP);
}

void loop() {
  unsigned long millisTime = millis();

  if (millisTime - previousRfidMillis >= intervalRfid) {
    readRFID();
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

  // if (millisTime - previousApiMillis >= intervalApi) {
  //   if (isClientConnected) {
  //     getAPI("/api/timeserver", host);  //api_call.ino
  //   }
  //   receiveResponse();  //api_call.ino
  //   previousApiMillis = millisTime;
  // }
}
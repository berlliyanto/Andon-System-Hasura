#include <SPI.h>
#include <EthernetENC.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

//------------Defined PIN----------------//

#define RXs1 3
#define TXs1 1
#define RXs2 16
#define TXs2 17
#define SS_PIN  4
#define RST_PIN 22  

//------------Defined PIN END----------------//

//------------Variable Declaration----------------//

byte serverERP[] = { 89, 116, 231, 224 };  // name address for erp hasura
byte mac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x32 };

bool isClientConnected = false;
String host = "89.116.231.224";
int portERP = 800;

bool printWebData = true;
unsigned long previousApiMillis = 0;
const long intervalApi = 10000;
unsigned long previousRfidMillis = 0;
const long intervalRfid = 900;

//------------Variable Declaration END----------------//


//------------Instance Object--------------//

EthernetClient client;
MFRC522 rfid(SS_PIN, RST_PIN);
DynamicJsonDocument docSlave2(1024);

//------------Instance Object END--------------//

void setup() {
  SPI.begin();
  rfid.PCD_Init(); 
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXs2, TXs2);

  delay(1000);
  Serial.println("Begin Ethernet");
  Ethernet.init(5);
  ethernetBegin(mac); //ethernet_config.ino

  connectClient(serverERP, portERP);
}

void loop() {
  unsigned long apiMillis = millis();
  unsigned long rfidMillis = millis();
  receiveResponse(); //api_call.ino
  
  if(rfidMillis - previousRfidMillis >= intervalRfid){
    readRFID();
    mainConSerial2();
    previousRfidMillis = rfidMillis;
  }

  if (apiMillis - previousApiMillis >= intervalApi) {
    if (isClientConnected) {
      Serial.println("Test Api Jalan");
      getAPI("/api/timeserver", host); //api_call.ino
    }
    previousApiMillis = apiMillis;
  }
}
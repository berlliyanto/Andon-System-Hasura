
void mainConSerial2() {
  if (SerialSlave2.available() > 0) {
    if(printSerial2State){
      Serial.println("Serial 2 Ready");
      printSerial2State = false;
    }
    String jsonString = SerialSlave2.readString();
    // const char* result = deserialize(docSlave2, jsonString.c_str());
    Serial.println(jsonString);
  }
}
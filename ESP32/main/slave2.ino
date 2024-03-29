void hopperStatus(int hopper) {
  Serial.println(hopper);
}

// SEND JSON TO SLAVE 2
void writeToSerial2(String json) {
  SerialSlave2.write(json.c_str());
  SerialSlave1.println();
}

//FUNCTION TO PROCESS DATA FROM SLAVE 2
void dataFromSlave2(String json) {
  deserializeJson(docSlave2, json);
  stateFromSlave2 = docSlave2["state"];
  typeFromSlave2 = docSlave2["type"];
  if (strcmp(typeFromSlave2, "hopper") == 0) {
    int hopper = docSlave2["hopper"];
    hopperStatus(hopper);
  }
}

// MAIN SERIAL COMMUNICATION MASTER - SLAVE 2
void mainConSerial2() {
  if (SerialSlave2.available() > 0) {
    if (printSerial2State) {
      Serial.println("Serial 2 Ready");
      printSerial2State = false;
    }
    String jsonString = SerialSlave2.readStringUntil('\n');
    dataFromSlave2(jsonString);
  }
}
void callOperation() {
  if (strcmp(stateFromSlave1, "call") == 0) {
    Serial.println("call");
    if (strcmp(typeFromSlave1, "MAINTENANCE") == 0) {
      Serial.println("MAINTENANCE");
    } else if (strcmp(typeFromSlave1, "MOLD") == 0) {
      Serial.println("MOLD");
    } else if (strcmp(typeFromSlave1, "TEKNISI") == 0) {
      Serial.println("TEKNISI");
    } else if (strcmp(typeFromSlave1, "MATERIAL") == 0) {
      Serial.println("MATERIAL");
    } else if (strcmp(typeFromSlave1, "QUALITY") == 0) {
      Serial.println("QUALITY");
    }
  }
}

void writeToSerial2(String json) {
  SerialSlave2.write(json.c_str());
}

void dataFromSlave1(String json) {
  deserializeJson(docSlave1, json);
  stateFromSlave1 = docSlave1["state"];
  
  if (strcmp(stateFromSlave1, "standby") != 0 || strcmp(stateFromSlave1, "end") != 0 || strcmp(stateFromSlave1, "shot") != 0) {
    typeFromSlave1 = docSlave1["type"];
    callOperation();
  } else if (strcmp(stateFromSlave1, "standby") == 0) {
    Serial.println("standby");
  } else if (strcmp(stateFromSlave1, "end") == 0) {
    Serial.println("end");
  } else if (strcmp(stateFromSlave1, "shot") == 0) {
    Serial.println("shot");
  }
}

void mainConSerial1() {
  if (SerialSlave1.available() > 0) {
    if (printSerial1State) {
      Serial.println("Serial 1 Ready");
      printSerial1State = false;
    }
    String jsonString = SerialSlave1.readStringUntil('\n');
    writeToSerial2(jsonString);
    dataFromSlave1(jsonString);
  }
}

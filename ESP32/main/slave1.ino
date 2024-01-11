void writeToSerial1(String json) {
  SerialSlave1.write(json.c_str());
  SerialSlave1.println();
}

void sendLoadingState(String type) {
  docSlave1Loading["state"] = "loading";
  docSlave1Loading["type"] = type;
  String loadingData;
  serializeJson(docSlave1Loading, loadingData);
  delay(50);
  SerialSlave1.write(loadingData.c_str());
  SerialSlave1.println();
}

void callOperation() {
  if (strcmp(typeFromSlave1, "MAINTENANCE") == 0) {
    postAPI("/api/workcentercall", hostDev, makeBody("call", "maintenance"));
    Serial.println("CALL MAINTENANCE");
  } else if (strcmp(typeFromSlave1, "MOLD") == 0) {
    postAPI("/api/workcentercall", hostDev, makeBody("call", "mold"));
    Serial.println("CALL MOLD");
  } else if (strcmp(typeFromSlave1, "TEKNISI") == 0) {
    postAPI("/api/workcentercall", hostDev, makeBody("call", "technician"));
    Serial.println("CALL TEKNISI");
  } else if (strcmp(typeFromSlave1, "MATERIAL") == 0) {
    postAPI("/api/workcentercall", hostDev, makeBody("call", "material"));
    Serial.println("CALL MATERIAL");
  } else if (strcmp(typeFromSlave1, "QUALITY") == 0) {
    postAPI("/api/workcentercall", hostDev, makeBody("call", "quality"));
    Serial.println("CALL QUALITY");
  }
}

void dataFromSlave1(String json) {
  deserializeJson(docSlave1, json);
  stateFromSlave1 = docSlave1["state"];

  if (strcmp(stateFromSlave1, "change") == 0 || strcmp(stateFromSlave1, "serve") == 0) {
    typeFromSlave1 = docSlave1["type"];
  } else if (strcmp(stateFromSlave1, "call") == 0) {
    typeFromSlave1 = docSlave1["type"];
    sendLoadingState("isLoading");
    delay(1000);
    callOperation();
  } else if (strcmp(stateFromSlave1, "standby") == 0) {
    sendLoadingState("isLoading");
    delay(1000);
    postAPI("/api/workcenterstatus/endstatus", hostDev, makeBody("end", ""));
    Serial.println("standby");
  } else if (strcmp(stateFromSlave1, "shot") == 0) {
    postAPI("/api/workcenterstatus/addshot", hostDev, makeBody("shot", ""));
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
    holdingJson = jsonString;
    Serial.println(holdingJson);
    dataFromSlave1(jsonString);
  }
}

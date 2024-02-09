// SEND JSON TO SLAVE 1
void writeToSerial1(String json) {
  SerialSlave1.write(json.c_str());
  SerialSlave1.println();
}

// SEND LOADING STATE TO SLAVE 1
void sendLoadingState(String type) {
  docSlave1Loading["state"] = "loading";
  docSlave1Loading["type"] = type;
  String loadingData;
  serializeJson(docSlave1Loading, loadingData);
  delay(50);
  SerialSlave1.write(loadingData.c_str());
  SerialSlave1.println();
}

// FUNCTION FOR CALL OPERATION FROM SLAVE 1
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

// FUNCTION TO PROCESS DATA FROM SLAVE 1
void dataFromSlave1(String json) {
  deserializeJson(docSlave1, json);
  stateFromSlave1 = docSlave1["state"];

  if (strcmp(stateFromSlave1, "change") == 0) {
    typeFromSlave1 = docSlave1["type"];

  } else if (strcmp(stateFromSlave1, "serve") == 0) {
    typeFromSlave1 = docSlave1["type"];
  } else if (strcmp(stateFromSlave1, "call") == 0) {
    typeFromSlave1 = docSlave1["type"];
    callOperation();
  } else if (strcmp(stateFromSlave1, "standby") == 0) {
    sendLoadingState("isLoading");
    delay(1000);
    postAPI("/api/workcenterstatus/endstatus", hostDev, makeBody("end", ""));
    Serial.println("standby");
  } else if (strcmp(stateFromSlave1, "shot") == 0) {
    postAPI("/api/workcenterstatus/addshot", hostDev, makeBody("shot", ""));
    Serial.println("shot");
  } else if (strcmp(stateFromSlave1, "ng") == 0) {
    String ng = "";
    serializeJson(docSlave1["type"], ng);
    const char* itemId = docSlave1["item_id"];

    postAPI("/api/workcenterstatus/addng", hostDev, makeBody("ng", ng, "", String(itemId)));
  } else if (strcmp(stateFromSlave1, "qr") == 0) {
    typeFromSlave1 = docSlave1["type"];
    if (strcmp(typeFromSlave1, "MOLD") == 0 || strcmp(typeFromSlave1, "TEKNISI") == 0 || strcmp(typeFromSlave1, "MATERIAL") == 0 || strcmp(typeFromSlave1, "QUALITY") == 0) {
      Serial.println("dari qr");
      getAPI("/api/iotreport/andonmonitoring?is_andon=1&wc=" + wcCode, hostDev);
    } else {
      isListenStatus = false;
    }
  }
}

// MAIN SERIAL COMMUNICATION MASTER - SLAVE 1
void mainConSerial1() {
  if (SerialSlave1.available() > 0) {
    if (printSerial1State) {
      Serial.println("Serial 1 Ready");
      printSerial1State = false;
    }
    String jsonString = SerialSlave1.readStringUntil('\n');
    holdingJson = jsonString;
    Serial.print("Holding Json : ");
    Serial.println(holdingJson);
    dataFromSlave1(jsonString);
  }
}

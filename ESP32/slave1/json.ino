// SEND JSON
void sendJson(String state = "standby", String type = "none", int shot = 0) {
  docSlave1.clear();
  docSlave1["state"] = state;

  if (state == "shot") {
    docSlave1["shot"] = shot;
  }

  if (state != "standby" || state != "none" || state != "shot") {
    docSlave1["type"] = type;
  }
  String jsonString;

  serializeJson(docSlave1, jsonString);
  SerialMaster.println(jsonString); // USE THIS TO SEND DATA TO MASTER
  Serial.println(jsonString);
}

// RECEIVE JSON FROM MASTER
void getJson(String json) {
  docSlave1.clear(); // ALWAYS CLEAR THE JSON BEFORE ADD NEW VALUE
  deserializeJson(docSlave1, json);
  const char* state = docSlave1["state"];
  const char* type = docSlave1["type"];
  delay(50);
  if (strcmp(state, "loading") == 0) {
    if (strcmp(type, "isLoading") == 0) {
      isLoading = true;
    } else {
      isLoading = false;
      isChangedView = true;
    };
  } else if (strcmp(state, "change") == 0) {
    isLoading = false;
    manPower = manPower + 1;

    serializeJson(docSlave1["items"], listItems);
    if(docSlave1.containsKey("work_order_no")) {
      workOrderNo = docSlave1["work_order_no"];
    }

    if (strcmp(type, "MAINTENANCE") == 0) HOME_STATUS = "MAINTENANCE";
    else if (strcmp(type, "QDC") == 0) HOME_STATUS = "QDC";
    else if (strcmp(type, "SETTING") == 0) HOME_STATUS = "SETTING";
    else if (strcmp(type, "DRYING") == 0) HOME_STATUS = "DRYING";
    else if (strcmp(type, "TRIAL") == 0) HOME_STATUS = "TRIAL";
    else if (strcmp(type, "RUNNING") == 0) HOME_STATUS = "RUNNING";
    else if (strcmp(type, "STANDBY") == 0) HOME_STATUS = "STANDBY";
    else if (strcmp(type, "RFID") == 0) {
      const char* status = docSlave1["status"];
      const char* value = docSlave1["value"];
      const char* woCode = "1WCINJ-009";
      const char* workCenterId = docSlave1["work_center_id"];
      Serial.println(workCenterId);
      size_t combinedLength = strlen(status) + strlen(value) + strlen(woCode) + strlen(workCenterId)  + 5;  //+5 Supaya aman
      char combinedString[combinedLength];

      snprintf(combinedString, combinedLength, "%s|%s|%s|%s", status, value, woCode, workCenterId);
      QR_VALUE = strdup(combinedString);

      CURRENT_VIEW = "QR";
    }

    if(strcmp(type, "RFID") != 0){
      CURRENT_VIEW = "HOME";
    }

    isChangedView = true;
  } else if (strcmp(state, "standby") == 0) {
    isLoading = false;
    manPower = 0;
    HOME_STATUS = "STANDBY";
    CURRENT_VIEW = "HOME";
    isChangedView = true;
  } else if (strcmp(state, "serve") == 0) {
    isLoading = false;
    callCount = callCount - 1;
    CURRENT_VIEW = "HOME";
    isChangedView = true;
  } else if (strcmp(state, "add_mp") == 0) {
    isLoading = false;
    manPower = manPower + 1;
    CURRENT_VIEW = "HOME";
    isChangedView = true;
  } else if (strcmp(state, "call") == 0) {
    isLoading = false;
    callCount = callCount + 1;
    CURRENT_VIEW = "HOME";
    isChangedView = true;
  } else if (strcmp(state, "connection") == 0) {
    if (strcmp(type, "connected") == 0) {
      connectionStatus = true;
    } else {
      connectionStatus = false;
    }
  }

  Serial.print("callCount ");
  Serial.println(callCount);
}
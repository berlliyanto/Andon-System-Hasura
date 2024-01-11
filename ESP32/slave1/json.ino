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
  SerialMaster.println(jsonString);
  Serial.println(jsonString);
}

void getJson(String json) {
  docSlave1.clear();
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
    if (strcmp(type, "RUNNING") == 0) {
      manPower = manPower + 1;
    }

    if (strcmp(type, "MAINTENANCE") == 0) HOME_STATUS = "MAINTENANCE";
    else if (strcmp(type, "MOLD") == 0) HOME_STATUS = "QDC";
    else if (strcmp(type, "TEKNISI") == 0) HOME_STATUS = "SETTING";
    else if (strcmp(type, "MATERIAL") == 0) HOME_STATUS = "DRYING";
    else if (strcmp(type, "QUALITY") == 0) HOME_STATUS = "TRIAL";
    else if (strcmp(type, "RUNNING") == 0) HOME_STATUS = "RUNNING";
    else if (strcmp(type, "STANDBY") == 0) HOME_STATUS = "STANDBY";

    CURRENT_VIEW = "HOME";
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
  }

  Serial.print("callCount ");
  Serial.println(callCount);
}
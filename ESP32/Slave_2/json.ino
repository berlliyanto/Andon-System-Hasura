void sendJson(String state, String type, int hopper = 0) {
  docSlave2["state"] = state;
  docSlave2["type"] = type;

  if (type == "hopper") {
    docSlave2["hopper"] = hopper;
  }

  String jsonString;
  serializeJson(docSlave2, jsonString);
  Serial.println(jsonString);
}

void getJson(String json) {
  docSlave2.clear();
  deserializeJson(docSlave2, json);
  const char* state = docSlave2["state"];
  delay(2000);
  if (strcmp(state, "call") == 0) {
    Serial.println("call");
    const char* type = docSlave2["type"];
    if (strcmp(type, "MAINTENANCE") == 0) addElementToListCall("MAINTENANCE");
    else if (strcmp(type, "MOLD") == 0) addElementToListCall("MOLD");
    else if (strcmp(type, "TEKNISI") == 0) addElementToListCall("TEKNISI");
    else if (strcmp(type, "MATERIAL") == 0) addElementToListCall("MATERIAL");
    else if (strcmp(type, "QUALITY") == 0) addElementToListCall("QUALITY");

    delay(500);
  } else if (strcmp(state, "change") == 0 || strcmp(state, "qr") == 0) {
    Serial.println("change");
    const char* type = docSlave2["type"];
    if (strcmp(type, "RUNNING") == 0) firstLine = "RUNNING";
    else if (strcmp(type, "MAINTENANCE") == 0) firstLine = "MAINTENANCE";
    else if (strcmp(type, "QDC") == 0) firstLine = "QDC";
    else if (strcmp(type, "SETTING") == 0) firstLine = "SETTING";
    else if (strcmp(type, "DRYING") == 0) firstLine = "DRYING";
    else if (strcmp(type, "TRIAL") == 0) firstLine = "TRIAL";
    else if (strcmp(type, "STANDBY") == 0) firstLine = "STANDBY";
  } else if (strcmp(state, "shot") == 0) {
    int shotValue = docSlave2["shot"];
    shotCounter += shotValue;
  } else if (strcmp(state, "serve") == 0) {
    Serial.println("serve");
    const char* type = docSlave2["type"];
    removeElementFromListCall(type);
  } else if(strcmp(state, "ng") == 0) {
    Serial.println("Nothing To do");
  }
}

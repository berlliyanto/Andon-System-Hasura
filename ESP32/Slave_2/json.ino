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
  deserializeJson(docSlave2, json);
  const char* state = docSlave2["state"];
  if (strcmp(state, "call") == 0) {
    CURRENT_TYPE = docSlave2["type"];
    CURRENT_STATE = state;
  } else if (strcmp(state, "change") == 0) {
    const char* type = docSlave2["type"];
    if (strcmp(type, "RUNNING") == 0) {
      CURRENT_STATE = "running";
    } else {
      CURRENT_STATE = state;
      CURRENT_TYPE = docSlave2["type"];
    }
  } else if (strcmp(state, "shot") == 0) {
    int shotValue = docSlave2["shot"];
    shotCounter += shotValue;
  } else if (strcmp(state, "standby") == 0) {
    CURRENT_STATE = "standby";
  } else if (strcmp(state, "serve") == 0) {
    CURRENT_STATE = "running";
  }
}
void sendJson(String type, bool hopper) {
  docSlave2["type"] = type;
  docSlave2["hopper"] = hopper;
  serializeJson(docSlave2, Serial);
  Serial.println();
}

void getJson(String json) {
  deserializeJson(docSlave2, json);
  const char* state = docSlave2["state"];
  if (strcmp(state, "call") == 0) {
    CURRENT_TYPE = docSlave2["type"];
    CURRENT_STATE = state;
    delay(100);
  } else if (strcmp(state, "change") == 0) {
    const char* type = docSlave2["type"];
    delay(100);
    if (strcmp(type, "RUNNING") == 0) {
      CURRENT_STATE = "running";
    }
  } else if (strcmp(state, "shot") == 0) {
    int shotValue = docSlave2["shot"];
    shotCounter += shotValue;
    delay(100);
  }
}
void sendJson(String state = "standby", String type = "none", int shot = 0) {
  docSlave1.clear();
  docSlave1["state"] = state;
  delay(100);
  if (state == "shot") {
    docSlave1["shot"] = shot;
  }
  if (state != "standby" || state != "end" || state != "shot") {
    docSlave1["type"] = type;
  }
  String jsonString;
  delay(100);
  serializeJson(docSlave1, jsonString);
  Serial.println(jsonString);
}
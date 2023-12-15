const char* deserialize(DynamicJsonDocument& doc, const char* json) {
  deserializeJson(doc, json);
  const char* sensor = doc["sensor"];
  return sensor;
}

void mainConSerial2() {
  if (Serial2.available() > 0) {
    Serial.print("sensor : ");
    String jsonString = Serial2.readString();
    const char* result = deserialize(docSlave2, jsonString.c_str());
    Serial.println(result);
  }
}
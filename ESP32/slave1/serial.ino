void mainConSerial() {
  if (SerialMaster.available() > 0) {
    String jsonString = SerialMaster.readStringUntil('\n');
    if (stringContainsWord(jsonString, "{\"state\":")) {
      Serial.print("Data from Master : ");
      Serial.println(jsonString);
      getJson(jsonString);
    }
  }
}

bool stringContainsWord(String str, String target) {
  return str.indexOf(target) != -1;
}
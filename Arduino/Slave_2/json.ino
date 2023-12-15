void readSensor(){
  doc["sensor"] = "gps";
  serializeJson(doc, Serial);
  Serial.println();
}
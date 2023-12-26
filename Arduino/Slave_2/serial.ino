void mainConSerial(){
  if(Serial.available() > 0){
    String jsonString = Serial.readStringUntil('\n');
    getJson(jsonString);
  }
}
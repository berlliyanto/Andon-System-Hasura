void readProximity(){
  int prox = digitalRead(proximityPin);
  if(prox == 0){
    proximityState = 0;
    Serial.println(prox);
  } else {
    proximityState = 1;
    Serial.println(prox);
  }
  delay(1000);
}
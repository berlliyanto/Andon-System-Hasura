void readProximity(){
  int prox = digitalRead(proximityPin);
  sendJson("running", "hopper", prox);
  if(prox == 1){
    proximityState = 1;
  } else {
    proximityState = 0;
  }
  delay(1000);
}
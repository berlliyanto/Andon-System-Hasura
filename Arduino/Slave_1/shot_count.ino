void shotCount() {
  int stateShotCount = 1;
  if (digitalRead(shotCountPin) == LOW && stateShotCount == 1) {
    sendJson("shot", "none", 1);
    stateShotCount = 0;
    delay(1000);
  }
}
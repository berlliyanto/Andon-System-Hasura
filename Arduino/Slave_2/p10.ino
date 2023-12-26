void displayRunning(int count, const char* hopper) {
  char buffer[10];
  sprintf(buffer, "%d", count);
  const char* counter = buffer;
  const char* hop = "H:";
  const char* countText = "S:";
  int hopLength = strlen(hop) + strlen(hopper) + 1;
  int countLength = strlen(counter) + strlen(countText) + 1;
  char newHopper[hopLength];
  char newCounter[countLength];

  strcpy(newCounter, countText);
  strcat(newCounter, counter);

  strcpy(newHopper, hop);
  strcat(newHopper, hopper);

  dmd.clearScreen(true);
  dmd.drawString(0, 0, newHopper, hopLength, GRAPHICS_NORMAL);
  dmd.drawString(0, 9, newCounter, countLength, GRAPHICS_NORMAL);
  delay(1000);
}

void displayCalling(const char* type) {
  const char* calling = "CALLING";
  char* newtype = type;

  dmd.clearScreen(true);
  for (int i = 0; i < 52; i++) {
    dmd.drawString(-32 + i, 0, calling, strlen(calling), GRAPHICS_NORMAL);
    dmd.drawString(32 - i, 9, newtype, strlen(newtype), GRAPHICS_NORMAL);
    delay(100);
    dmd.clearScreen(true);
  }
  delay(1000);
}

void displayStandby() {
  dmd.clearScreen(true);
  delay(2000);
  dmd.drawString(0, 0, "STANDBY", 7, GRAPHICS_NORMAL);
  dmd.drawString(0, 9, "CALLING MAINTENANCE", 19, GRAPHICS_NORMAL);
  delay(2000);
  // for (int i = 0; i < 120; i++) {
  //   delay(100);
  //   dmd.clearScreen(true);
  // }
  dmd.clearScreen(true);
}

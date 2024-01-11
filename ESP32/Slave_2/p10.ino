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
  dmd.drawString(0, -2, newHopper, hopLength, GRAPHICS_NORMAL);
  dmd.drawString(0, 7, newCounter, countLength, GRAPHICS_NORMAL);
  delay(1000);
}

void displayCalling(const char* type) {
  const char* calling = "CALLING";
  const char* newtype = type;
  int slideX = 0;
  int startX1 = 0;
  int startX2 = 0;

  if (strcmp(newtype, "MAINTENANCE") == 0) {
    slideX = 120;
    startX1 = 55;
    startX2 = 40;
  } else if (strcmp(newtype, "MOLD") == 0) {
    slideX = 80;
    startX1 = 27;
    startX2 = 35;
  } else if (strcmp(newtype, "TEKNISI") == 0) {
    slideX = 80;
    startX1 = 35;
    startX2 = 35;
  } else if (strcmp(newtype, "MATERIAL") == 0) {
    slideX = 85;
    startX1 = 40;
    startX2 = 35;
  } else if (strcmp(newtype, "QUALITY") == 0) {
    slideX = 85;
    startX1 = 37;
    startX2 = 35;
  }


  dmd.clearScreen(true);
  for (int i = 0; i < slideX; i++) {
    dmd.drawString(startX1 - i, -2, calling, strlen(calling), GRAPHICS_NORMAL);
    dmd.drawString(startX2 - i, 7, newtype, strlen(newtype), GRAPHICS_NORMAL);
    delay(50);
    dmd.clearScreen(true);
  }
  delay(1000);
}

void displayStandby() {
  dmd.clearScreen(true);
  delay(1000);
  for (int i = 0; i < 90; i++) {
    dmd.drawString(40 - i, -2, "STANDBY", 7, GRAPHICS_NORMAL);
    delay(50);
    dmd.clearScreen(true);
  }
  delay(1000);
}

void displayChange(const char* type) {
  int slideX = 0;
  int startX1 = 0;
  if (strcmp(type, "MAINTENANCE") == 0) {
    slideX = 120;
    startX1 = 40;
  } else if (strcmp(type, "MOLD") == 0) {
    slideX = 80;
    startX1 = 35;
  } else if (strcmp(type, "TEKNISI") == 0) {
    slideX = 80;
    startX1 = 35;
  } else if (strcmp(type, "MATERIAL") == 0) {
    slideX = 85;
    startX1 = 35;
  } else if (strcmp(type, "QUALITY") == 0) {
    slideX = 85;
    startX1 = 35;
  }
  dmd.clearScreen(true);
  for (int i = 0; i <= slideX; i++) {
    dmd.drawString(startX1 - i, 2, type, strlen(type), GRAPHICS_NORMAL);
    delay(50);
    dmd.clearScreen(true);
  }
  delay(1000);
}

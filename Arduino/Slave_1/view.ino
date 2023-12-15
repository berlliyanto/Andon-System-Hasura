//----------------------------------------------HOME
void showHomeScreen(const char* title) {
  tft.fillScreen(BLACK);

  //title
  int titleX = (tft.width() - strlen(title) * 18) / 2;
  int titleY = 20;
  tft.setTextSize(3);
  tft.setTextColor(YELLOW);

  tft.setCursor(titleX, titleY);
  tft.println(title);

  //line
  char* line = "============";
  int lineX = (tft.width() - strlen(line) * 18) / 2;
  int lineY = 50;
  tft.setCursor(lineX, lineY);
  tft.println(line);

  //buttons
  BTN_START_Y = (tft.height() - (BTN_HOME_HEIGHT * BTN_HOME_QTY + BTN_HOME_MARGIN * BTN_HOME_QTY));
  int buttonY = BTN_START_Y;
  for (int i = 0; i < BTN_HOME_QTY; i++ ) {
    drawButton(BTN_START_X, buttonY, BTN_WIDTH, BTN_HOME_HEIGHT, pgm_read_dword_near(BTN_HOME_COLOR + i), pgm_read_dword_near(BTN_HOME_LABEL + i), BLACK);
    buttonY += BTN_HOME_HEIGHT + BTN_HOME_MARGIN;
  }
  ACTIVE_SCREEN = "HOME";
}

//----------------------------------------------RFID
void showRfidScreen() {
  tft.fillScreen(BLACK);

  makeHeader("TAP");

  tft.setTextColor(YELLOW);
  tft.setTextSize(4);
  const int textHeight = 36;
  int descriptionX = (tft.width() - 8 * 24) / 2;  // 8 is length of string
  int descriptionY = (tft.height() - 3 * textHeight) / 2;
  tft.setCursor(descriptionX, descriptionY);
  tft.println(F("SILAHKAN"));

  descriptionX = (tft.width() - 8 * 24) / 2;  // 8 is length of string
  descriptionY = descriptionY + textHeight;
  tft.setCursor(descriptionX, descriptionY);
  tft.println(F("TAP RFID"));

  descriptionX = (tft.width() - 4 * 24) / 2;  // 4 is length of string
  descriptionY = descriptionY + textHeight;
  tft.setCursor(descriptionX, descriptionY);
  tft.println(F("ANDA"));

  ACTIVE_SCREEN = "RFID";
}

//----------------------------------------------CALL
void showCallScreen() {
  tft.fillScreen(BLACK);

  if (IS_SERVE) {
    makeHeader("SERVE");
  } else {
    makeHeader("CALL");
  }

  BTN_START_Y = (tft.height() - (BTN_CALL_HEIGHT * BTN_CALL_QTY + BTN_CALL_MARGIN * (BTN_CALL_QTY - 1))) / 2 + 25;
  int buttonY = BTN_START_Y;
  for (int i = 0; i < BTN_CALL_QTY; i++ ) {
    drawButton(BTN_START_X, buttonY, BTN_WIDTH, BTN_CALL_HEIGHT, pgm_read_dword_near(BTN_CALL_COLOR + i), pgm_read_dword_near(BTN_CALL_LABEL + i), BLACK);
    buttonY += BTN_CALL_HEIGHT + BTN_CALL_MARGIN;
  }
  ACTIVE_SCREEN = "CALL";
}

//----------------------------------------------CHANGE
void showScanScreen() {
  tft.fillScreen(BLACK);

  makeHeader("SCAN");

  BTN_START_Y = (tft.height() - (BTN_SCAN_HEIGHT * BTN_SCAN_QTY + BTN_SCAN_MARGIN * BTN_SCAN_QTY)) / 2 + 25;
  int buttonY = BTN_START_Y;
  for (int i = 0; i < BTN_SCAN_QTY; i++ ) {
    drawButton(BTN_START_X, buttonY, BTN_WIDTH, BTN_SCAN_HEIGHT, pgm_read_dword_near(BTN_SCAN_COLOR + i), pgm_read_dword_near(BTN_SCAN_LABEL + i), BLACK);
    buttonY += BTN_SCAN_HEIGHT + BTN_SCAN_MARGIN;
  }
  ACTIVE_SCREEN = "SCAN";
}

void showChangeScreen() {
  tft.fillScreen(BLACK);

  makeHeader("CHANGE");

  BTN_START_Y = (tft.height() - (BTN_CHANGE_HEIGHT * BTN_CHANGE_QTY + BTN_CHANGE_MARGIN * (BTN_CHANGE_QTY - 1))) / 2 + 25;
  int buttonY = BTN_START_Y;
  for (int i = 0; i < BTN_CHANGE_QTY; i++ ) {
    drawButton(BTN_START_X, buttonY, BTN_WIDTH, BTN_CHANGE_HEIGHT, pgm_read_dword_near(BTN_CHANGE_COLOR + i), pgm_read_dword_near(BTN_CHANGE_LABEL + i), BLACK);
    buttonY += BTN_CHANGE_HEIGHT + BTN_CHANGE_MARGIN;
  }

  ACTIVE_SCREEN = "CHANGE";
}
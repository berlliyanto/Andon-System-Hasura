void HomeView(char* title) {
  tft.fillScreen(BLACK);

  Text(130, 30, 3, GREEN, title);
  Text(30, 65, 4, YELLOW, "===========");

  callBtn.initButton(&tft, 160, 180, 280, 70, WHITE, RED, BLACK, "CALL", 3);
  scanBtn.initButton(&tft, 160, 270, 280, 70, WHITE, GREEN, BLACK, "SCAN", 3);
  endBtn.initButton(&tft, 160, 360, 280, 70, WHITE, BLUE, BLACK, "END", 3);

  callBtn.drawButton(false);
  scanBtn.drawButton(false);
  endBtn.drawButton(false);

  CURRENT_VIEW = "HOME";
}

void CallView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  ButtonList(130, 200, 270, 340, 410, 60);
  CURRENT_VIEW = "CALL";
}

void ScanView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  CHScanBtn.initButton(&tft, 160, 180, 280, 70, WHITE, RED, BLACK, "CHANGE", 3);
  AddScanBtn.initButton(&tft, 160, 270, 280, 70, WHITE, CYAN, BLACK, "ADD MP", 3);
  SerScanBtn.initButton(&tft, 160, 360, 280, 70, WHITE, YELLOW, BLACK, "SERVE", 3);

  CHScanBtn.drawButton(false);
  AddScanBtn.drawButton(false);
  SerScanBtn.drawButton(false);

  CURRENT_VIEW = "SCAN";
}

void ChangeView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  RunChangeBtn.initButton(&tft, 160, 130, 280, 50, WHITE, GREEN, BLACK, "RUNNING", 3);
  RunChangeBtn.drawButton(false);
  ButtonList(190, 250, 310, 370, 430, 50);

  CURRENT_VIEW = "CHANGE";
}

void ServeView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);
  ButtonList(130, 200, 270, 340, 410, 60);

  CURRENT_VIEW = "SERVE";
}

void RFIDView(char* title) {
  tft.fillScreen(BLACK);

  Header(title);

  Text(60, 120, 4, YELLOW, "SILAHKAN");
  Text(60, 170, 4, YELLOW, "TAP RFID");
  Text(110, 220, 4, YELLOW, "ANDA");

  int xStatus = 0;
  if (RFID_STATUS == "MAINTENANCE") {
    xStatus = 65;
  } else if (RFID_STATUS == "MOLD") {
    xStatus = 120;
  } else if (RFID_STATUS == "TEKNISI") {
    xStatus = 95;
  } else if (RFID_STATUS == "MATERIAL") {
    xStatus = 90;
  } else if (RFID_STATUS == "QUALITY" || RFID_STATUS == "RUNNING") {
    xStatus = 100;
  } else if (RFID_STATUS == "MP") {
    xStatus = 140;
  }

  Text(xStatus, 270, 3, GREEN, RFID_STATUS);

  CURRENT_VIEW = "RFID";
}